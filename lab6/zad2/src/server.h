#ifndef SERVER_H
#define SERVER_H
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

void processMessage(Message *msg);
void handleLogin(Message *msg);
void handleTime(Message *msg);
void handleUpper(Message *msg);
mqd_t getClientQueue(pid_t clientPid);

static const char *TIME_FORMAT = "%Y-%m-%d %H:%M:%S";
static const char *SERVER_FULL = "Unable to connect: server is full";
#endif