#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "server.h"

mqd_t clientQueues[MAX_CLIENTS];
int clientIDs[MAX_CLIENTS];
int clientCount, currentID;
int shutdown;

int main(int argc, char const *argv[]){
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAX_MSG_LEN;
	attr.mq_curmsgs = 0;
	shutdown = 0;
	
	printf("Server\n");
	mqd_t queue = mq_open(SERVER_QUEUE, O_CREAT | O_RDONLY, QUEUE_ACCESS, &attr); // returns -1 on error
	char buffer[MAX_MSG_LEN+1];
	while(!shutdown){
		sleep(1);
		ssize_t bytesRead;
		if((bytesRead = mq_receive(queue, buffer, MAX_MSG_LEN, 0)) == -1){
			fprintf(stderr, "mq_receive error: %s\n", strerror(errno));
		}
		processMessage(buffer, bytesRead);
		printf("Outside: %s\n", buffer);
	}
	mq_close(queue); // returns -1 on error
	return 0;
}

void processMessage(char *buffer, ssize_t bufferLength){
	if(bufferLength < 2){
		fprintf(stderr, "%s\n", ERROR_MSG_LEN);
	}
	MessageType type = (int) buffer[0];
	switch(type){
		case LOGIN:
			printf("LOGIN\n");
			break;
		case LOGOUT:
			printf("LOGOUT\n");
			break;
		case ECHO:
			printf("ECHO\n");
			break;
		case UPPER:
			printf("UPPER\n");
			break;
		case TIME:
			printf("TIME\n");
			break;
		case TERMINATE:
			printf("TERMINATE\n");
			shutdown = 1;
			break;
		default:
			printf("Received illegal cmd\n");
			break;
	}
}