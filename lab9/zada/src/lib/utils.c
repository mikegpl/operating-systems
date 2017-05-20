#include "utils.h"

void giveSemaphore(sem_t *semaphore) {
    DESSERT("sem_post", sem_post(semaphore), -1, "Error while giving semaphore");
}

void takeSemaphore(sem_t *semaphore) {
    DESSERT("sem_wait", sem_wait(semaphore), -1, "Error while taking semaphore");
}

sem_t *createSemaphore(const char *name, int defaultValue) {
    sem_t *semaphore = sem_open(name, O_CREAT | O_RDWR, 0666, defaultValue);
    DESSERT("sem_open", semaphore, SEM_FAILED, "Error while opening semaphore");
    return semaphore;
}

void deleteSemaphore(sem_t *semaphore, const char *name) {
    ASSERT("sem_close", sem_close(semaphore), 0, "Error while closing semaphore");
    ASSERT("sem_unlink", sem_unlink(name), 0, "Error while unlinking semaphore");
}

void *createSharedMemory(int *memoryId, const char *name, size_t size) {
    *memoryId = shm_open(name, O_CREAT | O_RDWR, 0666);
    DESSERT("shm_open", *memoryId, -1, "Error while creating shared memory");
    DESSERT("ftruncate", ftruncate(*memoryId, size), -1, "Error while truncating memory");
    void *shared = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, *memoryId, 0);
    DESSERT("mmap", shared, (void *) -1, "Error while mapping shared memory");
    return shared;
}

void deleteSharedMemory(void *memory, const char *name, size_t size) {
    DESSERT("munmap", munmap(memory, size), -1, "Error while unmapping shared memory");
    DESSERT("shm_unlink", shm_unlink(name), -1, "Error while unlinking shared memory");
}

long getTimeStamp() {
    struct timespec timeStamp;
    int retval = clock_gettime(CLOCK_MONOTONIC, &timeStamp);
    DESSERT("clock_gettime", retval, -1, "Error while getting timestamp");
    return timeStamp.tv_sec * (int) 1e6 + timeStamp.tv_nsec / 1000;
}