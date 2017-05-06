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
    pid_t chair;
    pid_t array[QUEUE_MAX_SIZE];
};

int PidQueue_isEmpty(PidQueue *queue);

pid_t PidQueue_get(PidQueue *queue);

int PidQueue_put(PidQueue *queue, pid_t elem);

#endif /* PIDQUEUE_H */