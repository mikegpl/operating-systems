#include "server.h"

mqd_t myQueue;
int clientsCount, idCount, shutdown;
pid_t clientPids[MAX_CLIENTS];
mqd_t clientQueues[MAX_CLIENTS];

int main(int argc, char *argv[]){
	printf("Server\n");
	mqd_t myQueue = mq_queueOpen(SERVER_NAME, 'r');
	shutdown = clientsCount = idCount = 0;	
	Message msg;

	while(!shutdown || !mq_queueEmpty(myQueue)){
		processMessage(mq_queueReceive(myQueue, &msg));
	}

	for(int i = 0; i < clientsCount; i++)
		mq_queueClose(clientQueues[i]);
	mq_queueClose(myQueue);
	mq_queueDelete(SERVER_NAME);
    return 0;
}

void processMessage(Message *msg){
	printf("Origin: %d, type: %d, contents: %s\n", msg->originpid, (int) msg->type, msg->contents);
	if(NULL != msg){
		switch(msg->type){
			case LOGIN:
				handleLogin(msg);
				break;
			case LOGOUT:
				handleLogout(msg);
				break;
			case ECHO:
				handleEcho(msg);
				break;
			case UPPER:
				handleUpper(msg);
				break;
			case TIME:
				handleTime(msg);
				break;
			case TERMINATE:
				printf("I must go, my planet needs me\n");
				shutdown = 1;
				break;
			default:
				fprintf(stderr, "Received illegal command\n");
				break;
		}
	}
}

void handleLogin(Message *msg){
	printf("%d is trying to connect\n", msg->originpid);
	mqd_t clientQueue = mq_queueOpen(msg->contents, 'w');
	
	if(clientsCount < MAX_CLIENTS){
		printf("%d has connected successfully\n", msg->originpid);
		clientPids[clientsCount] = msg->originpid;
		clientQueues[clientsCount] = clientQueue;
		clientsCount++;
		msg->type = ACK;
		msg->originpid = getpid();
		sprintf(msg->contents, "%d", idCount++);
		mq_queueSend(clientQueue, msg);
	}
	else{
		printf("%d - connection refused\n", msg->originpid);
		msg->type = NACK;
		msg->originpid = getpid();
		strcpy(msg->contents, SERVER_FULL);
		mq_queueSend(clientQueue, msg);
		mq_queueClose(clientQueue);
	}
}

void handleLogout(Message *msg){
	mqd_t clientQueue = getClientQueue(msg->originpid);
	int i;
	for(i = 0; i < MAX_CLIENTS; i++){
		if(msg->originpid == clientPids[i]){
			printf("%d has exited\n", msg->originpid);
			break;
		}
	}
	mq_queueClose(clientQueue);
	while(i < MAX_CLIENTS - 1){
		clientPids[i] = clientPids[i+1];
		clientQueues[i] = clientQueues[i+1];
		i++;
	}
	clientsCount--;
}

void handleEcho(Message *msg){
	mqd_t client;
	if(-1 != (client = getClientQueue(msg->originpid))){
		msg->originpid = getpid();
		mq_queueSend(client, msg);
	}
}

void handleUpper(Message *msg){
	mqd_t client;
	if(-1 != (client = getClientQueue(msg->originpid))){
		msg->originpid = getpid();
		int length = strlen(msg->contents);
		for(int i = 0; i < length; i++)
			msg->contents[i] = toupper(msg->contents[i]);
		mq_queueSend(client, msg);
	}
}

void handleTime(Message *msg){
	mqd_t client;
	if(-1 != (client = getClientQueue(msg->originpid))){
		msg->originpid = getpid();
		time_t timer;
		struct tm *tm_info;
		time(&timer);
		tm_info = localtime(&timer);
		strftime(msg->contents, MAX_MSG_LEN, TIME_FORMAT, tm_info);
		mq_queueSend(client, msg);
	}	
}

mqd_t getClientQueue(pid_t clientPid){
	for(int i = 0; i < clientsCount; i++){
		if(clientPid == clientPids[i])
			return clientQueues[i];
	}
	return -1;
}

// pack procedures below into lib
void mq_queueSend(mqd_t queue, Message *msg){
	if(-1 == (mq_send(queue, (char *) msg, MESSAGE_SIZE, 0))){
		fprintf(stderr, "%s\n", ERROR_MQ_SEND);
	}
}

Message *mq_queueReceive(mqd_t queue, Message *msg){
	if(-1 == (mq_receive(queue, (char *) msg, MESSAGE_SIZE, 0))){
		fprintf(stderr, "%s\n", ERROR_MQ_RECV);
		exit(1);
	}
	return msg;
}

mqd_t mq_queueOpen(const char *name, char mode){
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

void mq_queueClose(mqd_t queue){
	if(-1 == mq_close(queue)){
		fprintf(stderr, "%s\n", ERROR_MQ_CLOSE);
		exit(1);
	}
}

void mq_queueDelete(const char *name){
	if(-1 == mq_unlink(name)){
		fprintf(stderr, "%s\n", ERROR_MQ_UNLINK);
		exit(1);
	}
}

int mq_queueEmpty(mqd_t queue){
	struct mq_attr attr;
	if(-1 == mq_getattr(queue, &attr)){
		fprintf(stderr, "%s\n", ERROR_MQ_GETATTR);
		exit(1);
	}
	return (attr.mq_curmsgs == 0);
}
