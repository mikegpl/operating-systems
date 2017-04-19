#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_CLIENTS 8
#define MAX_MSG_LEN 64
#define MAX_NAME_LEN 32
#define QUEUE_ACCESS 0666

typedef enum MessageType
{
	LOGIN = 1, LOGOUT, ACK, NACK, ECHO, UPPER, TIME, TERMINATE
} MessageType;

static const char *SERVER_QUEUE = "/serverqueue1";
#endif