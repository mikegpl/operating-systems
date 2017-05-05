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

pid_t sync_PidQueue_isEmpty(PidQueue *queue, int semId, struct sembuf *buffer) {
    takeSemaphore(semId, buffer, 0);
    int retVal = PidQueue_isEmpty(queue);
    giveSemaphore(semId, buffer, 0);
    return retVal;
}

pid_t sync_PidQueue_get(PidQueue *queue, int semId, struct sembuf *buffer) {
    takeSemaphore(semId, buffer, 0);
    pid_t retVal = PidQueue_get(queue);
    giveSemaphore(semId, buffer, 0);
    return retVal;
}

int sync_PidQueue_put(PidQueue *queue, int semId, struct sembuf *buffer, pid_t pid) {
    takeSemaphore(semId, buffer, 0);
    int retVal = PidQueue_put(queue, pid);
    giveSemaphore(semId, buffer, 0);
    return retVal;
}

int giveSemaphore(int semId, struct sembuf *buffer, short flag) {
    buffer->sem_num = 0;
    buffer->sem_op = 1;
    buffer->sem_flg = flag;
    return semop(semId, buffer, 1);
}

int takeSemaphore(int semId, struct sembuf *buffer, short flag) {
    buffer->sem_num = 0;
    buffer->sem_op = -1;
    buffer->sem_flg = flag;
    return semop(semId, buffer, 1);
}