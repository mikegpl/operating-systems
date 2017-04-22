#include "client.h"

int shutdown;
int serverQueue, myQueue;

int main(int argc, char *argv[]){
	printf("Client\n");

	key_t serverKey;
	serverKey = ipc_getKey(getenv(PATH_SOURCE));
	myQueue = ipc_queueOpen(IPC_PRIVATE, QUEUE_ACCESS);
	serverQueue = ipc_queueOpen(serverKey, 0);

    Message msg;
    shutdown = 0;
    int myID = registerClient();

    char *line;
    size_t size;
    while(!shutdown){
    	printf("Client (%d)> ", myID);
    	ssize_t charsRead = getline(&line, &size, stdin);
		if(-1 == charsRead){
			printf("Received EOF, quitting\n");
			break;
		}
		
		int awaitResponse;
		if(-1 == (awaitResponse = parseLine(line, charsRead, &msg))){
			continue;
		}

		ipc_queueSend(serverQueue, &msg);
		if(awaitResponse){
			ipc_queueReceive(myQueue, &msg);
			if(!shutdown)
				printf("server> %s\n", msg.contents);
		}
		else{
			printf("Exiting\n");
			break;
		}
		
	}
	ipc_queueDelete(myQueue);
    return 0;
}

int registerClient(){
	int id;
	Message msg;
    msg.type = LOGIN;
    msg.originpid = getpid();
    sprintf(msg.contents, "%d", myQueue);

    ipc_queueSend(serverQueue, &msg);
    ipc_queueReceive(myQueue, &msg);
    if(ACK == msg.type){
    	printf("Connected to the server. My id: %s\n", msg.contents);
    	id = stringToInt(msg.contents);
    }
    else{
    	printf("Connection refused. Reason: %s\n", msg.contents);
    	shutdown = 1;
    	id = -1;
    }
    return id;
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


// queue interface
key_t ipc_getKey(char *path){
	key_t key;
	if(NULL == path){
		fprintf(stderr, "%s\n", ERROR_FILEPATH);
		exit(1);
	}	
	
	if(-1 == (key = ftok(path, PROJECT_ID))){
		fprintf(stderr, "%s\n", ERROR_FTOK);	
		exit(1);
	}
	return key;
}

int ipc_queueOpen(key_t key, int flags){
	int qid;
	if(-1 == (qid = msgget(key, flags))){
		fprintf(stderr, "%s\n", ERROR_IPC_OPEN);
		exit(1);
	}
	return qid;
}

void ipc_queueDelete(int qid){
	if(-1 == (msgctl(qid, IPC_RMID, NULL))){
		fprintf(stderr, "%s\n", ERROR_IPC_DELETE);
		exit(1);
	}
}

void ipc_queueSend(int qid, Message *msg){
	if(-1 == msgsnd(qid, msg, MESSAGE_SIZE, 0)) {
        fprintf(stderr, "%s\n", ERROR_IPC_SEND);
        exit(1);
    }
}

Message *ipc_queueReceive(int qid, Message *msg){
 	if(-1 == msgrcv(qid, msg, MESSAGE_SIZE, 0, 0)) {
  	 	fprintf(stderr, "%s\n", ERROR_IPC_RECV);
        exit(1);
    }
    return msg;
}

int ipc_queueEmpty(int qid){
	struct msqid_ds stat;	
	if(-1 == msgctl(qid, IPC_STAT, &stat)){
		fprintf(stderr, "%s\n", ERROR_IPC_STAT);
		exit(1);
	}
	return (stat.msg_qnum == 0);
}
