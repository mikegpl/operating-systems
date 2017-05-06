#ifndef ZAD1_UTILS_H
#define ZAD1_UTILS_H

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <semaphore.h>

void takeSemaphore(sem_t *semaphore);

void giveSemaphore(sem_t *semaphore);

long getTimeStamp();

void throwAndExit(void (*func)(void));

#endif //ZAD1_UTILS_H
