#include "server.h"

mqd_t myQueue;
int clientsCount, idCount, shutdown;
pid_t clientPids[MAX_CLIENTS];
mqd_t clientQueues[MAX_CLIENTS];

int main(int argc, char *argv[]){
	printf("Server\n");
	mqd_t myQueue = queueOpen(SERVER_NAME, 'r');
	shutdown = clientsCount = idCount = 0;	
	Message msg;
	while(!shutdown){
		processMessage(queueReceive(myQueue, &msg));
	}

	for(int i = 0; i < clientsCount; i++)
		queueClose(clientQueues[i]);
	queueClose(myQueue);
    return 0;
}

void processMessage(Message *msg){
	if(NULL != msg){
		switch(msg->type){
			case LOGIN:
				handleLogin(msg);
			break;
			default:
				printf("Type %d, contents: %s", (int) msg->type, msg->contents);
				if(strncmp(msg->contents, "xD", 2) == 0)
					shutdown = 1;
			break;
		}
	}
}



// pack procedures below into lib
void handleLogin(Message *msg){
	mqd_t clientQueue = queueOpen(msg->contents, 'w');
	if(clientsCount < MAX_CLIENTS){
		clientPids[clientsCount] = msg->originpid;
		clientQueues[clientsCount] = clientQueue;
		clientsCount++;
		msg->type = ACK;
		msg->originpid = getpid();
		sprintf(msg->contents, "%d", idCount++);
		queueSend(clientQueue, msg);
	}
	else{
		msg->type = NACK;
		msg->originpid = getpid();
		strcpy(msg->contents, SERVER_FULL);
		queueSend(clientQueue, msg);
		queueClose(clientQueue);
	}
	strcpy(msg->contents, "Opened client's queue");
}

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
