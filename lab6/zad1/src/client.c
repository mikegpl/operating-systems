#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "protocol.h"

// todo -> pack to header file
void getKeyAndQueue(key_t *key, int *qid, int flags);
void deleteQueue(int qid);
void processLine(char *line, ssize_t lineLength); 
void processMessage(Message *message);
void sendToServer(Message *msg);
int stringToInt(char *number);

// todo -> think about global variables
pid_t mypid;
int shutdown;
int serverQID, myQID;

int main(int argc, char *argv[]){
	key_t serverKey;
	mypid = getpid();

	// todo -> refactor this
    if((myQID = msgget (IPC_PRIVATE, 0660)) == -1) {
        fprintf(stderr, "msgget error: %s\n", strerror(errno)); 
        exit(1);
    }

    // todo -> refactor this
    getKeyAndQueue(&serverKey, &serverQID, 0);

    // todo -> refactor whole login part
    Message msg;

    msg.type = LOGIN;
    msg.originpid = mypid;
    sprintf(msg.contents, "%d", myQID);

    sendToServer(&msg);

    if(msgrcv(myQID, &msg, MESSAGE_SIZE, 0, 0) == -1) {
    	fprintf(stderr, "msgrcv error: %s\n", strerror(errno));
    	exit(0);
    }
    if(msg.type != ACK){
    	printf("%s\n", msg.contents);
    	exit(0);
    }
    int clientID = stringToInt(msg.contents);
    printf("Connected to server. ClientID: %d\n", clientID);
    // \todo end

    char *line = NULL;
    size_t size = 0;
    shutdown = 0;
    while(!shutdown){
    	ssize_t charsRead = getline(&line, &size, stdin);
		if(charsRead != -1){
			processLine(line, charsRead);
		}
		else{
			printf("Finished reading lines from stdin\n");
			if(line != NULL)
				free(line);
			msg.type = LOGOUT;
			msg.originpid = mypid;
			strcpy(msg.contents, "I must go now, my planet needs me");
			sendToServer(&msg);
		}
	}
	deleteQueue(myQID);
    return 0;
}

// todo -> refactor this
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
		fprintf(stderr, "msgctl (delete) error: %s\n", strerror(errno));
	}
}


// todo -> parsing etc.
void processLine(char *line, ssize_t lineLength){
	return;
}

// todo -> refine this
void sendToServer(Message *msg){
	if(msgsnd(serverQID, msg, MESSAGE_SIZE, 0) == -1) {
        fprintf(stderr, "msgsnd error: %s\n", strerror(errno));
        exit(1);
    }
    switch(msg->type){
    	case LOGOUT:
    	case TERMINATE:
    		printf("Exitting now\n");
    		shutdown = 1;
    		break;
    	default:
    		break;
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