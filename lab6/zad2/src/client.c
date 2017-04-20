#include "client.h"


mqd_t myQueue;
mqd_t serverQueue;
int shutdown;

int main(int argc, char const *argv[]){
	printf("Client\n");
	shutdown = 0;
	char myQueueName[MAX_NAME_LEN+1];
	getQueueName(myQueueName);
	printf("'%s'\n", myQueueName);

	myQueue = queueOpen(myQueueName, 'r');
	serverQueue = queueOpen(SERVER_NAME, 'w');

	Message msg;
	char myID[4];

	// pack this into loginClient
	msg.type = LOGIN;
	msg.originpid = getpid();
	strcpy(msg.contents, myQueueName);
	queueSend(serverQueue, &msg);
	queueReceive(myQueue, &msg);
	if(msg.type == ACK){
		printf("Connected to the server. My id: %s\n", msg.contents);
		sprintf(myID, "%s", msg.contents);
	}
	else{
		shutdown = 1;
	}
	// if NACK -> quit, else enter loop below

	size_t readSize;	
	char *line;
	while(!shutdown){
		printf("Client (%s)> ", myID);
		ssize_t charsRead = getline(&line, &readSize, stdin);
		if(-1 == charsRead){
			printf("Received EOF, quitting\n");
			break;
		}
		msg.type = ECHO;
		msg.originpid = getpid();
		strcpy(msg.contents, line);
		queueSend(serverQueue, &msg);
	}

	queueClose(serverQueue);
	queueClose(myQueue);
	return 0;
}

void getQueueName(char *buffer){
	srand(time(NULL));
	if(buffer == NULL){
		exit(1);
	}
	buffer[0] = '/';
	for(int i = 1; i < MAX_NAME_LEN; i++){
		buffer[i] = LEGAL_CHARS[rand() % 36];
	}
	buffer[MAX_NAME_LEN] = '\0';
}

// void parseLine(char *line, ssize_t lineLength, Message *msg){
// 	msg->type = 0;	
// 	msg->originpid = getpid();
// 	if(lineLength > MAX_MSG_LEN){
// 		printf("%s\n", ERROR_MAX_LEN);
// 		return;
// 	}
// 	else{
// 		char *tmp = strtok(line, WSPACE_DELIMITERS);
// 		if(tmp == NULL){
// 			return;
// 		}
// 		else{
// 			int i;
// 			for(i = 0; i < PARSE_ARRAY_LEN; i++){
// 				if(strcmp(tmp, COMMANDS[i]) == 0){
// 					msg->type = TYPES[i];
// 					break;
// 				}
// 			}
// 			if(i == PARSE_ARRAY_LEN){
// 				printf("%s\n", HELP_INFO);
// 				return;
// 			}
// 		}
// 		tmp = strtok(NULL, "");
// 		if(tmp != NULL){
// 			tmp[strlen(tmp) - 1] = '\0';
// 			strcpy(msg->contents, tmp);
// 			printf("Message %s\n", msg->contents);
// 		}
// 	}
// }


// pack procedures below into lib
void queueSend(mqd_t queue, Message *msg){
	if(-1 == (mq_send(queue, msg, MESSAGE_SIZE, 0))){
		fprintf(stderr, "%s\n", ERROR_MQ_SEND);
	}
}

Message *queueReceive(mqd_t queue, Message *msg){
	if(-1 == (mq_receive(queue, msg, MESSAGE_SIZE, 0))){
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
	attr.mq_curmsgs = 0;

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