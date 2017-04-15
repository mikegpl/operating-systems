#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <time.h>
#include "protocol.h"

// todo -> pack to header file
void getKeyAndQueue(key_t *key, int *qid, int flags);
void deleteQueue(int qid);
void processMessage(Message *msg);
void handleLogin(pid_t clientpid, int qid);
void handleLogout(int clientpid);
void handleEcho(Message *msg);
void handleUpper(Message *msg);
void handleTime(Message *msg);
int stringToInt(char *number);
int getClientID(pid_t clientpid);
void sendToClient(int qid, Message *msg);

// todo -> think about global variables (i.e. serverpid)
unsigned int clientsID[MAX_CLIENTS][2];
int clientCount; 
int clientID;
int shutDown;

int main(int argc, char *argv[]){
	key_t serverKey;
	int queueID;
	shutDown = 0;
	clientCount = 0;

	// todo -> refactor this
	getKeyAndQueue(&serverKey, &queueID, IPC_CREAT | QUEUE_ACCESS);

	printf("Server is now working\n");
	Message msg;
	while(!shutDown){
	  	if(msgrcv(queueID, &msg, MESSAGE_SIZE, 0, 0) == -1) {
	  	 	fprintf(stderr, "msgrcv error: %s\n", strerror(errno));
            exit(1);
        }
        processMessage(&msg);
	}
	printf("Shutting down server\n");
	deleteQueue(queueID);

    return 0;
}

void processMessage(Message *msg){
	assert(msg != NULL);
	switch(msg->type){
		case LOGIN:
			handleLogin(msg->originpid, stringToInt(msg->contents));
			handleTime(msg);
			break;
		case LOGOUT:
			handleLogout(msg->originpid);
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
			printf("Received order to kill myself.\n");
			shutDown = 1;
			break;
		default:
			fprintf(stderr, "Received illegal cmd\n");
			break;
	}
}

void getKeyAndQueue(key_t *key, int *qid, int flags){
	const char *filePath = (const char *) getenv(PATH_SOURCE);
	if(filePath == NULL){
		fprintf(stderr, "Couldn't get value of %s, setting filepath to '%s'\n", PATH_SOURCE, DEFAULT_PATH);
		filePath = PATH_SOURCE;
	}	
	
	if((*key = ftok(filePath, PROJECT_ID)) == -1){
		fprintf(stderr, "ftok error: %s\n", strerror(errno));	
		exit(1);
	}

	if((*qid = msgget(*key, flags)) == -1){
		fprintf(stderr, "msgget error: %s\n", strerror(errno));
		exit(1);
	}
}

void deleteQueue(int qid){
	if((msgctl(qid, IPC_RMID, NULL)) == -1){
		fprintf(stderr, "queue deletion error: %s\n", strerror(errno));
	}
}

void handleLogin(pid_t clientpid, int qid){
	printf("%d is trying to connect\n", clientpid);
	Message msg;
	msg.originpid = getpid();
	if(clientCount == MAX_CLIENTS){
		msg.type = NACK;
		strcpy(msg.contents, "Connection refused - server is full");
		printf("%d - connection refused\n", clientpid);
		printf("%d clientcount\n", clientCount);
	}
	else{
		msg.type = ACK;
		sprintf(msg.contents, "%d", clientID++);
		clientsID[clientCount][0] = clientpid;
		clientsID[clientCount][1] = qid;
		clientCount++;
		printf("%d has successfuly connected\n", clientpid);
	}
	sendToClient(qid, &msg);
}

void handleLogout(int clientpid){
	int i;
	for(i = 0; i < MAX_CLIENTS; i++){
		if(clientpid == clientsID[i][0]){
			printf("Client with pid %d has exited\n", clientpid);
		}
	}
	while(i < MAX_CLIENTS-1){
		clientsID[i][0] = clientsID[i+1][0];
		clientsID[i][1] = clientsID[i+1][1];
		i++;
	}
	clientCount--;
}

void handleEcho(Message *msg){
	int qid = getClientID(msg->originpid);
	msg->originpid = getpid();
	sendToClient(qid, msg);
}

void handleUpper(Message *msg){
	int qid = getClientID(msg->originpid);
	int length = strlen(msg->contents);
	for(int i = 0; i < length; i++)
		msg->contents[i] = toupper(msg->contents[i]);
	msg->originpid = getpid();
	sendToClient(qid, msg);
}

void handleTime(Message *msg){
	int qid = getClientID(msg->originpid);
	msg->originpid = getpid();
	time_t timer;
	struct tm *tm_info;
	time(&timer);
	tm_info = localtime(&timer);
	strftime(msg->contents, MAX_MSG_LEN, "%Y-%m-%d %H:%M:%S", tm_info);
	printf("%s\n", msg->contents);
	sendToClient(qid, msg);
}

int stringToInt(char *number){
    assert(number != NULL);
    char *stopAt;
    int value = (int) strtol(number, &stopAt, 10);
    if(number == stopAt || value < 0){
        printf("Error while parsing str to int\n");
        exit(1);
    }
    return value;
}

void sendToClient(int qid, Message *msg){
	if(msgsnd(qid, msg, MESSAGE_SIZE, 0) == -1) {
        fprintf(stderr, "msgsnd error: %s\n", strerror(errno));
        exit(1);
    }
}

int getClientID(pid_t clientpid){
	for(int i = 0; i < clientCount; i++){
		if(clientpid == clientsID[i][0]){
			return clientsID[i][1];
		}
	}
	return -1;
}