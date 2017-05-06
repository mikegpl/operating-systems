#include "utils.h"

void giveSemaphore(sem_t *semaphore) {
    if (sem_post(semaphore) == -1)
        throwAndExit(NULL);
}

void takeSemaphore(sem_t *semaphore) {
    if (sem_wait(semaphore) == -1)
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