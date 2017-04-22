#include "client.h"


mqd_t myQueue;
mqd_t serverQueue;
int shutdown;

int main(int argc, char const *argv[]){
	printf("Client\n");
	char myQueueName[MAX_NAME_LEN+1];
	getQueueName(myQueueName);
	printf("'%s'\n", myQueueName);

	myQueue = queueOpen(myQueueName, 'r');
	serverQueue = queueOpen(SERVER_NAME, 'w');

	Message msg;
	char myID[4];
	shutdown = 0;

	registerClient(&msg, myQueueName, myID);

	size_t readSize;	
	char *line;
	while(!shutdown){ 
		printf("Client (%s)> ", myID);
		ssize_t charsRead = getline(&line, &readSize, stdin);
		if(-1 == charsRead){
			printf("Received EOF, quitting\n");
			break;
		}

		int awaitResponse;
		if(-1 == (awaitResponse = parseLine(line, charsRead, &msg))){
			continue;
		}

		queueSend(serverQueue, &msg);
		if(awaitResponse){
			shutdown = queueTimedReceive(myQueue, &msg, SERVER_TIMEOUT);
			if(!shutdown)
				printf("server> %s\n", msg.contents);
		}
		else{
			printf("Exiting\n");
			break;
		}
	}

	queueClose(serverQueue);
	queueClose(myQueue);
	queueDelete(myQueueName);
	return 0;
}

void registerClient(Message *msg, char *queueName, char *clientID){
	msg->type = LOGIN;
	msg->originpid = getpid();
	strcpy(msg->contents, queueName);
	queueSend(serverQueue, msg);
	queueReceive(myQueue, msg);
	if(msg->type == ACK){
		printf("Connected to the server. My id: %s\n", msg->contents);
		sprintf(clientID, "%s", msg->contents);
	}
	else{
		printf("Connection refused. Reason: %s\n", msg->contents);
		shutdown = 1;
	}
}

void getQueueName(char *buffer){
	srand(time(NULL));
	if(NULL == buffer){
		exit(1);
	}
	buffer[0] = '/';
	for(int i = 1; i < MAX_NAME_LEN; i++){
		buffer[i] = LEGAL_CHARS[rand() % 36];
	}
	buffer[MAX_NAME_LEN] = '\0';
}

int parseLine(char *line, ssize_t lineLength, Message *msg){
	int awaitResponse;
	msg->type = 0;	
	msg->originpid = getpid();
	if(lineLength > MAX_MSG_LEN){
		printf("%s\n", ERROR_MAX_LEN);
		return -1;
	}
	else{
		char *tmp = strtok(line, WSPACE_DELIMITERS);
		if(tmp == NULL){
			return -1;
		}
		else{
			int i;
			for(i = 0; i < PARSE_ARRAY_LEN; i++){
				if(strcmp(tmp, COMMANDS[i]) == 0){
					msg->type = TYPES[i];
					if(LOGOUT == TYPES[i] || TERMINATE == TYPES[i])
						awaitResponse = 0;
					else
						awaitResponse = 1;
					break;
				}
			}
			if(i == PARSE_ARRAY_LEN){
				printf("%s\n", HELP_INFO);
				return -1;
			}
		}
		tmp = strtok(NULL, "");
		if(tmp != NULL){
			tmp[strlen(tmp) - 1] = '\0';
			strcpy(msg->contents, tmp);
		}
		return awaitResponse;
	}
}







// pack procedures below into lib
void queueSend(mqd_t queue, Message *msg){
	if(-1 == (mq_send(queue, (char *) msg, MESSAGE_SIZE, 0))){
		fprintf(stderr, "%s\n", ERROR_MQ_SEND);
	}
}

Message *queueReceive(mqd_t queue, Message *msg){
	if(-1 == (mq_receive(queue, (char *) msg, MESSAGE_SIZE, 0))){
		fprintf(stderr, "%s\n", ERROR_MQ_RECV);
		exit(1);
	}
	return msg;
}

mqd_t queueOpen(const char *name, char mode){
	mqd_t queue;
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = MSGS_LIMIT;
	attr.mq_msgsize = MESSAGE_SIZE;

	switch(mode){
		case 'r':
			if(-1 == (queue = mq_open(name, O_CREAT | O_RDONLY, ACCESS_RIGHTS, &attr))){
				fprintf(stderr, "%s\n", ERROR_MQ_OPEN);
				exit(1);
			}
		break;
		case 'w':
			if(-1 == (queue = mq_open(name, O_WRONLY))){
				fprintf(stderr, "%s\n", ERROR_MQ_OPEN);
				exit(1);
			}
		break;
		default:
			fprintf(stderr, "Creating queue: %s\n", ERROR_WRONGOPT);
			exit(1);
		break;
	}
	return queue;
}

void queueClose(mqd_t queue){
	if(-1 == (mq_close(queue))){
		fprintf(stderr, "%s\n", ERROR_MQ_CLOSE);
		exit(1);
	}
}

void queueDelete(const char *name){
	if(-1 == mq_unlink(name)){
		fprintf(stderr, "%s\n", ERROR_MQ_UNLINK);
		exit(1);
	}
}

int queueEmpty(mqd_t queue){
	struct mq_attr attr;
	if(-1 == mq_getattr(queue, &attr)){
		fprintf(stderr, "%s\n", ERROR_MQ_GETATTR);
		exit(1);
	}
	return (attr.mq_curmsgs == 0);
}

int queueTimedReceive(mqd_t queue, Message *msg, int delayInSeconds){
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME, &tm);
	tm.tv_sec += delayInSeconds;
	if(-1 == mq_timedreceive(queue, (char *) msg, MESSAGE_SIZE, 0, &tm)){
		printf("%s\n", ERROR_TIMEOUT);
		return 1;
	}
	return 0;
}