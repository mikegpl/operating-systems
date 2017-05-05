#ifndef PIDQUEUE_H
#define PIDQUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>

#define QUEUE_MAX_SIZE 512

typedef struct PidQueue PidQueue;
struct PidQueue {
    unsigned int size;
    unsigned int capacity;
    pid_t array[QUEUE_MAX_SIZE];
};

int PidQueue_isEmpty(PidQueue *queue);

pid_t PidQueue_get(PidQueue *queue);

int PidQueue_put(PidQueue *queue, pid_t elem);

pid_t sync_PidQueue_isEmpty(PidQueue *queue, int semId, struct sembuf *buffer);

pid_t sync_PidQueue_get(PidQueue *queue, int semId, struct sembuf *buffer);

int sync_PidQueue_put(PidQueue *queue, int semId, struct sembuf *buffer, pid_t pid);

int takeSemaphore(int semId, struct sembuf *buffer, short flag);

int giveSemaphore(int semId, struct sembuf *buffer, short flag);

#endif /* PIDQUEUE_H */