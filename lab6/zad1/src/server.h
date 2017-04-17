#ifndef SERVER_H
#define SERVER_H
#include "protocol.h"

key_t getKey();
int getQueue(key_t key, int flags);
void deleteQueue(int qid);
void processMessage(Message *msg);
void handleLogin(Message *msg);
void handleLogout(Message *msg);
void handleEcho(Message *msg);
void handleUpper(Message *msg);
void handleTime(Message *msg);
void handleTerminate(Message *msg);
int stringToInt(char *number);
int getClientID(pid_t clientpid);
void sendToClient(int qid, Message *msg);

static const char *TIME_FORMAT = "%Y-%m-%d %H:%M:%S";

#endif