#ifndef CLIENT_H
#define CLIENT_H
#include "protocol.h"

key_t getKey();
int getQueue(key_t key, int flags);
void deleteQueue(int qid);
int queueEmpty(int qid);
int connectToServer();
void processLine(char *line, ssize_t lineLength); 
void parseLine(char *line, ssize_t lineLength, Message *msg);
void sendToServer(Message *msg, MessageType expectedType);
void receiveMessage(int qid, Message *msg);
void processMessage(Message *message);

void safeExit(int exitCode);
int stringToInt(char *number);

static const char *HELP_INFO = "Available commands:\nlogout\necho message\nupper message\ntime\nterminate";
static const char *ERROR_MAX_LEN = "Error: message is too long";
static const char *WSPACE_DELIMITERS = "\n\t "; 

static const int PARSE_ARRAY_LEN = 5;
static const char *COMMANDS[] = {"logout", "echo", "upper", "time", "terminate"};
static const MessageType TYPES[] = {LOGOUT, ECHO, UPPER, TIME, TERMINATE};
#endif