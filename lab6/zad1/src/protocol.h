#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_CLIENTS 8
#define MAX_MSG_LEN 64
#define QUEUE_ACCESS 0666

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
	
static const char *PATH_SOURCE = "HOME";
static const char *DEFAULT_PATH = "/etc/hosts";
static const char PROJECT_ID = (char) 42;
static const int MESSAGE_SIZE = sizeof(Message) - sizeof(long);

#endif