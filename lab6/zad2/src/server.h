#ifndef SERVER_H
#define SERVER_H
#include "protocol.h"

void processMessage(char *buffer, ssize_t bufferLength);

static const char *ERROR_MSG_LEN = "Error - received message is too short";

#endif