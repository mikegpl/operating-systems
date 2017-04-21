#ifndef CLIENT_H
#define CLIENT_H
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "protocol.h"

void getQueueName(char *buffer);
int parseLine(char *line, ssize_t lineLength, Message *msg);

static const char *LEGAL_CHARS = "abcdefghijklmnopqrstuvwxyz0123456789";
static const char *HELP_INFO = "Available commands:\nlogout\necho message\nupper message\ntime\nterminate";
static const char *ERROR_MAX_LEN = "Error: message is too long";
static const char *WSPACE_DELIMITERS = "\n\t "; 

static const int PARSE_ARRAY_LEN = 5;
static const char *COMMANDS[] = {"logout", "echo", "upper", "time", "terminate"};
static const MessageType TYPES[] = {LOGOUT, ECHO, UPPER, TIME, TERMINATE};
#endif