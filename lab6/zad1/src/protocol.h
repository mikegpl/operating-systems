#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_CLIENTS 8
#define MAX_MSG_LEN 64
#define QUEUE_ACCESS 0666
#define RECV_DELAY 100

typedef enum MessageType
{
	LOGIN = 1, LOGOUT, ACK, NACK, ECHO, UPPER, TIME, TERMINATE
} MessageType;

typedef struct Message
{
	long type;
	pid_t originpid;
	char contents[MAX_MSG_LEN];
} Message;

key_t ipc_getKey(char *path);
int ipc_queueOpen(key_t key, int flags);
void ipc_queueDelete(int qid);
void ipc_queueSend(int qid, Message *msg);
Message *ipc_queueReceive(int queue, Message *msg);
int ipc_queueEmpty(int qid);

	
static const char *PATH_SOURCE = "HOME";
static const char PROJECT_ID = (char) 42;
static const int MESSAGE_SIZE = sizeof(Message) - sizeof(long);

static const char *ERROR_FILEPATH = "Error: incorrect filepath for ftok";
static const char *ERROR_IPC_OPEN = "Error while opening queue";
static const char *ERROR_IPC_DELETE = "Error while deleting queue";
static const char *ERROR_IPC_SEND = "Error while sending to queue";
static const char *ERROR_IPC_RECV = "Error while receiving from queue";
static const char *ERROR_IPC_STAT = "Error while getting queue stats";
static const char *ERROR_FTOK = "Error while generating key using ftok";

#endif