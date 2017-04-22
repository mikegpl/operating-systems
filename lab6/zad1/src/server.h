#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <time.h>
#include "protocol.h"

void processMessage(Message *msg);
void handleLogin(Message *msg);
void handleLogout(Message *msg);
void handleEcho(Message *msg);
void handleUpper(Message *msg);
void handleTime(Message *msg);
void handleTerminate(Message *msg);
int stringToInt(char *number);
int getClientQid(pid_t clientpid);

static const char *TIME_FORMAT = "%Y-%m-%d %H:%M:%S";
static const char *SERVER_FULL = "Unable to connect: server is full";
#endif