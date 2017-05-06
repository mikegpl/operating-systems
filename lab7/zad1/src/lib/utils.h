#ifndef ZAD1_UTILS_H
#define ZAD1_UTILS_H

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>

void takeSemaphore(int semArrayId, struct sembuf *buffer, unsigned short semIndex);

void giveSemaphore(int semArrayId, struct sembuf *buffer, unsigned short semIndex);

long getTimeStamp();

void throwAndExit(void (*func)(void));

#endif //ZAD1_UTILS_H
