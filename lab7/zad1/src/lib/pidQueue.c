#include "pidQueue.h"

int PidQueue_isEmpty(PidQueue *queue) {
    return queue->size == 0;
}

pid_t PidQueue_get(PidQueue *queue) {
    if (PidQueue_isEmpty(queue))
        return 0;
    pid_t first = queue->array[0];
    for (int i = 0; i < queue->size - 1; i++) {
        queue->array[i] = queue->array[i + 1];
    }
    queue->size--;
    return first;
}


int PidQueue_put(PidQueue *queue, pid_t elem) {
    if (queue->capacity <= queue->size) {
        return -1;
    }
    queue->array[queue->size++] = elem;
    return 0;
}