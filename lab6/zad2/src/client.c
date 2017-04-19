#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "client.h"

int myID;

int main(int argc, char const *argv[]){
	srand(time(NULL));
	printf("Client\n");
	size_t readSize = 0;
	char *line = NULL;
	char message[MAX_MSG_LEN];
	char myQueueName[MAX_NAME_LEN+1];
	myID = 42;

	getQueueName(myQueueName);


	mqd_t myQueue;
	mqd_t serverQueue = mq_open(SERVER_QUEUE, O_WRONLY);
	printf("Opened server's queue\n");
	while(1){
		printf("me>");
		ssize_t charsRead = getline(&line, &readSize, stdin);
		parseLine(line, charsRead, message);
		if(charsRead != -1){
			mq_send(serverQueue, message, MAX_MSG_LEN, 0);
		}
		else
			break;
	}
	mq_close(serverQueue);
	return 0;
}

void getQueueName(char *buffer){
	if(buffer == NULL){
		exit(1);
	}
	buffer[0] = '/';
	for(int i = 1; i < MAX_NAME_LEN; i++){
		buffer[i] = LEGAL_CHARS[rand() % 36];
	}
	buffer[MAX_NAME_LEN] = '\0';
	printf("%s\n", buffer);
}

void parseLine(char *line, ssize_t lineLength, char *message){
	if(lineLength > MAX_MSG_LEN){
		printf("%s\n", ERROR_MAX_LEN);
		message[0] = 0;
		return;
	}
	else{
		char *tmp = strtok(line, WSPACE_DELIMITERS);
		if(tmp == NULL){
			message[0] = 0;
			return;
		}
		else{
			int i;
			for(i = 0; i < PARSE_ARRAY_LEN; i++){
				if(strcmp(tmp, COMMANDS[i]) == 0){
					message[0] = TYPES[i];
					break;
				}
			}
			if(i == PARSE_ARRAY_LEN){
				message[0] = 0;	
				printf("%s\n", HELP_INFO);
				return;
			}
		}
		tmp = strtok(NULL, "");
		if(tmp != NULL){
			tmp[strlen(tmp) - 1] = '\0';
			sprintf(message+1, "%4.d%s", myID, tmp);
			printf("Message %s\n", message+1);
		}
	}
}
