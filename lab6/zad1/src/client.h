#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "protocol.h"

int registerClient();
int parseLine(char *line, ssize_t lineLength, Message *msg);
void processMessage(Message *message);
int stringToInt(char *number);

static const char *HELP_INFO = "Available commands:\nlogout\necho message\nupper message\ntime\nterminate";
static const char *ERROR_MAX_LEN = "Error: message is too long";
static const char *WSPACE_DELIMITERS = "\n\t "; 

static const int PARSE_ARRAY_LEN = 5;
static const char *COMMANDS[] = {"logout", "echo", "upper", "time", "terminate"};
static const MessageType TYPES[] = {LOGOUT, ECHO, UPPER, TIME, TERMINATE};
#endif