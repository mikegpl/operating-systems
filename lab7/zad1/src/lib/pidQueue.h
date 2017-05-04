#ifndef PIDQUEUE_H
#define PIDQUEUE_H

#include <stdlib.h>
#include <stdio.h>

#define QUEUE_SIZE 512

typedef struct PidQueue PidQueue;
struct PidQueue {
    unsigned int size;
    pid_t array[QUEUE_SIZE];
};

int PidQueue_isEmpty(PidQueue *queue);

pid_t PidQueue_get(PidQueue *queue);

void PidQueue_put(PidQueue *queue, pid_t elem);

const char *ERROR_QUEUE_FULL;

#endif /* PIDQUEUE_H */