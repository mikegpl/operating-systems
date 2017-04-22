#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_CLIENTS 2
#define MAX_MSG_LEN 64
#define MSGS_LIMIT 10
#define ACCESS_RIGHTS 0666
#define MAX_NAME_LEN 32
#define SERVER_TIMEOUT 3

typedef enum MessageType
{
	LOGIN = 1, LOGOUT, ACK, NACK, ECHO, UPPER, TIME, TERMINATE
} MessageType;

typedef struct Message
{
	char type;
	pid_t originpid;
	char contents[MAX_MSG_LEN];
} Message;
	
mqd_t mq_queueOpen(const char *name, char mode);
void mq_queueClose(mqd_t queue);
void mq_queueDelete(const char *name);
void mq_queueSend(mqd_t queue, Message *msg);
Message *mq_queueReceive(mqd_t queue, Message *msg);
int mq_queueTimedReceive(mqd_t queue, Message *msg, int delayInSeconds);
int mq_queueEmpty(mqd_t queue);

static const int MESSAGE_SIZE = sizeof(Message);
static const char *SERVER_NAME = "/abcdefg";

static const char *ERROR_MQ_OPEN = "Error while opening queue";
static const char *ERROR_MQ_CLOSE = "Error while closing queue";
static const char *ERROR_MQ_RECV = "Error while receiving from queue";
static const char *ERROR_MQ_UNLINK = "Errro while unlinking queue";
static const char *ERROR_MQ_SEND = "Error while sending to queue";
static const char *ERROR_MQ_GETATTR = "Error while requesting queue info";
static const char *ERROR_WRONGOPT = "Error - received illegal option";
#endif