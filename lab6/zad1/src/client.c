#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "client.h"

pid_t mypid;
int shutdown;
int serverQID, myQID;

int main(int argc, char *argv[]){
	key_t serverKey;
	mypid = getpid();

	myQID = getQueue(IPC_PRIVATE, 0660);
	serverKey = getKey();
	serverQID = getQueue(serverKey, 0);

    int clientID = connectToServer();

    Message msg;
    char *line = NULL;
    size_t size = 0;
    shutdown = 0;
    while(!shutdown){
    	while(!queueEmpty(myQID)){
    		receiveMessage(myQID, &msg);
    		if(msg.type == TERMINATE){
    			printf("%s. Exitting\n", msg.contents);
    			shutdown = 1;
    		}
    	}
    	if(shutdown)
    		break;

    	printf("me (id:%3d )> ", clientID);
    	ssize_t charsRead = getline(&line, &size, stdin);
		if(charsRead != -1){
			processLine(line, charsRead);
		}
		else{
			printf("\nFinished reading lines from stdin\n");
			if(line != NULL)
				free(line);
			msg.type = LOGOUT;
			msg.originpid = mypid;
			strcpy(msg.contents, "I must go now, my planet needs me");
			sendToServer(&msg, 0);
		}
	}
	deleteQueue(myQID);
    return 0;
}

key_t getKey(){
	key_t key;
	const char *filePath = (const char *) getenv(PATH_SOURCE);
	if(filePath == NULL){
		fprintf(stderr, "Couldn't get value of %s, setting filepath to '%s'\n", PATH_SOURCE, DEFAULT_PATH);
		filePath = PATH_SOURCE;
	}	
	
	if((key = ftok(filePath, PROJECT_ID)) == -1){
		fprintf(stderr, "ftok error: %s\n", strerror(errno));	
		exit(1);
	}
	return key;
}

int getQueue(key_t key, int flags){
	int qid;
	if((qid = msgget(key, flags)) == -1){
		fprintf(stderr, "msgget error: %s\n", strerror(errno));
		exit(1);
	}
	return qid;
}

void deleteQueue(int qid){
	if((msgctl(qid, IPC_RMID, NULL)) == -1){
		fprintf(stderr, "msgctl (delete) error: %s\n", strerror(errno));
		exit(1);
	}
}


int queueEmpty(int qid){
	struct msqid_ds stat;	
	if(msgctl(qid, IPC_STAT, &stat) == -1){
		fprintf(stderr, "msgctl (stat) error: %s\n", strerror(errno));
		safeExit(1);
	}
	return (stat.msg_qnum == 0);
}


int connectToServer(){
	Message msg;
    msg.type = LOGIN;
    msg.originpid = mypid;
    sprintf(msg.contents, "%d", myQID);
    sendToServer(&msg, ACK);
    int clientID = stringToInt(msg.contents);
    printf("Connected to server. ClientID: %d\n", clientID);
    return clientID;
}

// todo -> bsd_queueReceive
void receiveMessage(int qid, Message *msg){
    if(msgrcv(qid, msg, MESSAGE_SIZE, 0, 0) == -1) {
    	fprintf(stderr, "msgrcv error: %s\n", strerror(errno));
    	safeExit(1);
    }
}

void processLine(char *line, ssize_t lineLength){
	Message msg; 
	parseLine(line, lineLength, &msg);
	if(msg.type > 0)
		sendToServer(&msg, 0);
	else
		printf("%s\n", HELP_INFO);
}

void parseLine(char *line, ssize_t lineLength, Message *msg){
	if(lineLength > MAX_MSG_LEN){
		printf("%s\n", ERROR_MAX_LEN);
		msg->type = 0;
		return;
	}
	else{
		msg->originpid = mypid;
		char *tmp = strtok(line, WSPACE_DELIMITERS);
		if(tmp == NULL){
			msg->type = 0;
			return;
		}
		else{
			int i;
			for(i = 0; i < PARSE_ARRAY_LEN; i++){
				if(strcmp(tmp, COMMANDS[i]) == 0){
					msg->type = TYPES[i];
					break;
				}
			}
			if(i == PARSE_ARRAY_LEN){
				msg->type = 0;
				return;
			}
		}
		tmp = strtok(NULL, "");
		if(tmp != NULL){
			tmp[strlen(tmp) - 1] = '\0';
			strcpy(msg->contents, tmp);
		}
	}
}


// todo -> queueSend
void sendToServer(Message *msg, MessageType expectedType){
	int awaitResponse = 0;
	switch(msg->type){
    	case LOGOUT:
    	case TERMINATE:
    		printf("Exitting now\n");
    		shutdown = 1;
    		break;
    	case ECHO:
    	case UPPER:
    	case TIME:
    	case LOGIN:
    		awaitResponse = 1;
    		break;
    	default:
    		return;
    }
	if(msgsnd(serverQID, msg, MESSAGE_SIZE, 0) == -1) {
        fprintf(stderr, "msgsnd error: %s\n", strerror(errno));
        exit(1);
    } 

    if(awaitResponse){
    	receiveMessage(myQID, msg);
    	if(expectedType > 0 && msg->type != expectedType){
    		printf("Expected different type (%d, got %ld). Contents: %s\n", expectedType, msg->type, msg->contents);
    		safeExit(1);
    	}
		printf("server> %s\n", msg->contents);
    }
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

void safeExit(int exitCode){
	deleteQueue(myQID);
	exit(exitCode);
}