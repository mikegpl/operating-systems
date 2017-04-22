#include "server.h"

// pid / queueKey
unsigned int clientData[MAX_CLIENTS][2];
int clientCount; 
int idCount;
int shutdown;

int main(int argc, char *argv[]){
	printf("Server\n");
	key_t serverKey = ipc_getKey(getenv(PATH_SOURCE));
	int myQueue = ipc_queueOpen(serverKey, IPC_CREAT | QUEUE_ACCESS);
	shutdown = clientCount = idCount = 0;
	Message msg;

	while(!shutdown || !ipc_queueEmpty(myQueue)){
        processMessage(ipc_queueReceive(myQueue, &msg));
	}

	ipc_queueDelete(myQueue);
    return 0;
}

void processMessage(Message *msg){
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
			printf("I must go, my planet needs me.\n");
			shutdown = 1;
			break;
		default:
			fprintf(stderr, "Received illegal cmd\n");
			break;
	}
}

void handleLogin(Message *msg){
	pid_t clientpid = msg->originpid;
	printf("%d is trying to connect\n", clientpid);
	int qid = stringToInt(msg->contents);
	printf("client's queue %d\n", qid);
	msg->originpid = getpid();

	if(clientCount < MAX_CLIENTS){
		printf("%d has connected successfuly\n", clientpid);
		msg->type = ACK;
		sprintf(msg->contents, "%d", idCount++);
		clientData[clientCount][0] = clientpid;
		clientData[clientCount][1] = qid;
		clientCount++;
	}
	else{
		printf("%d - connection refused\n", clientpid);
		msg->type = NACK;
		strcpy(msg->contents, SERVER_FULL);
	}
	ipc_queueSend(qid, msg);
}

void handleLogout(Message *msg){
	int i;
	for(i = 0; i < MAX_CLIENTS; i++){
		if(msg->originpid == clientData[i][0]){
			printf("%d has exited\n", msg->originpid);
			break;
		}
	}
	while(i < MAX_CLIENTS-1){
		clientData[i][0] = clientData[i+1][0];
		clientData[i][1] = clientData[i+1][1];
		i++;
	}
	clientCount--;
}

void handleEcho(Message *msg){
	int qid;
	if(-1 != (qid = getClientQid(msg->originpid))){
		msg->originpid = getpid();
		ipc_queueSend(qid, msg);
	}
}

void handleUpper(Message *msg){
	int qid;
	if(-1 != (qid = getClientQid(msg->originpid))){
		msg->originpid = getpid();
		int length = strlen(msg->contents);
		for(int i = 0; i < length; i++)
			msg->contents[i] = toupper(msg->contents[i]);
		ipc_queueSend(qid, msg);
	}
}

void handleTime(Message *msg){
	int qid;
	if(-1 != (qid = getClientQid(msg->originpid))){
		msg->originpid = getpid();
		time_t timer;
		struct tm *tm_info;
		time(&timer);
		tm_info = localtime(&timer);
		strftime(msg->contents, MAX_MSG_LEN, TIME_FORMAT, tm_info);
		ipc_queueSend(qid, msg);		
	}
}

int getClientQid(pid_t clientpid){
	for(int i = 0; i < clientCount; i++){
		if(clientpid == clientData[i][0]){
			return clientData[i][1];
		}
	}
	return -1;
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

Message *ipc_queueReceive(int queue, Message *msg){
 	if(-1 == msgrcv(queue, msg, MESSAGE_SIZE, 0, 0)) {
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

