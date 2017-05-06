#include "utils.h"

void giveSemaphore(int semArrayId, struct sembuf *buffer, unsigned short semIndex) {
    buffer->sem_num = semIndex;
    buffer->sem_op = 1;
    buffer->sem_flg = 0;
    if (semop(semArrayId, buffer, 1) == -1)
        throwAndExit(NULL);
}

void takeSemaphore(int semArrayId, struct sembuf *buffer, unsigned short semIndex) {
    buffer->sem_num = semIndex;
    buffer->sem_op = -1;
    buffer->sem_flg = 0;
    if (semop(semArrayId, buffer, 1) == -1)
        throwAndExit(NULL);
}

long getTimeStamp() {
    struct timespec timeStamp;
    if (clock_gettime(CLOCK_MONOTONIC, &timeStamp) == -1)
        throwAndExit(NULL);
    return timeStamp.tv_sec * (int) 1e6 + timeStamp.tv_nsec / 1000;
}

void throwAndExit(void (*func)(void)) {
    fprintf(stderr, "Error: %s", strerror(errno));
    if (func != NULL)
        func();
    exit(EXIT_FAILURE);
}