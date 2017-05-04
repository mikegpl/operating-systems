#include "pidQueue.h"

const char *ERROR_QUEUE_FULL = "Error - cannot add element to full queue";

int PidQueue_isEmpty(PidQueue *queue) {
    return queue->size == 0;
}

pid_t PidQueue_get(PidQueue *queue) {
    pid_t first = queue->array[0];
    for (int i = 0; i < queue->size - 1; i++) {
        queue->array[i] = queue->array[i + 1];
    }
    queue->array[--queue->size] = 0;
    return first;
}


void PidQueue_put(PidQueue *queue, pid_t elem) {
    if (QUEUE_SIZE <= queue->size + 1) {
        fprintf(stderr, "%s\n", ERROR_QUEUE_FULL);
        return;
    }
    queue->array[queue->size++] = elem;
}