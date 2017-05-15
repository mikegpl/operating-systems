#define _GNU_SOURCE

#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include "common.h"
#include "lib/utils.h"
#include "lib/pidQueue.h"

void sigHandler(int sigNo);

void createFIFO(unsigned int chairsCount);

void createSemaphores();

void freeResources();

void sleepAndWork();

pid_t getFirstClone();

void giveHaircut(pid_t cloneId);

static int queueId;
static sem_t *barberSem, *queueSem;
static PidQueue *queue;
static int closeShop = 0;

int main(int argc, char *argv[]) {
    printf("Barber\n");
    if (argc != 2) {
        printf("You should provide number of seats, so master Kenobi can choose one of them\n");
        exit(EXIT_FAILURE);
    }
    unsigned int chairsCount = (unsigned int) atol(argv[1]);
    if (signal(SIGINT, sigHandler) == SIG_ERR)
        throwAndExit(NULL);

    createFIFO(chairsCount);
    createSemaphores();
    sleepAndWork();
    freeResources();
    return 0;
}

void sleepAndWork() {
    printf("Barbershop quartet: barber is going to work\n");
    while (!closeShop) {
        printf("Barbershop quartet @ %10.ld: Barber is going to sleep\n", getTimeStamp());
        takeSemaphore(barberSem);
        pid_t cloneId = getFirstClone();
        giveHaircut(cloneId);
        while (1) {
            takeSemaphore(queueSem);
            cloneId = PidQueue_get(queue);
            if (cloneId != 0) {
                giveSemaphore(queueSem);
                giveHaircut(cloneId);
            } else {
                takeSemaphore(barberSem);
                giveSemaphore(queueSem);
                break;
            }
        }
    }
}

void giveHaircut(pid_t cloneId) {
    printf("Barbershop quartet @ %10.ld: Barber started cutting  %d hair\n", getTimeStamp(), cloneId);
    if (kill(cloneId, SIGRTMIN) == -1)
        throwAndExit(freeResources);
    printf("Barbershop quartet @ %10.ld: Barber finished cutting %d hair\n", getTimeStamp(), cloneId);
}

pid_t getFirstClone() {
    takeSemaphore(queueSem);
    pid_t cloneId = queue->chair;
    giveSemaphore(queueSem);
    return cloneId;
}

void freeResources() {
    if (munmap((void *) queue, sizeof(PidQueue)) == -1)
        throwAndExit(NULL);
    if (shm_unlink(SHARED_MEM_NAME) == -1)
        throwAndExit(NULL);
    if (sem_close(barberSem) == -1)
        throwAndExit(NULL);
    if (sem_unlink(BARBER_SEM_NAME) == -1)
        throwAndExit(NULL);
    if (sem_close(queueSem) == -1)
        throwAndExit(NULL);
    if (sem_unlink(QUEUE_SEM_NAME) == -1)
        throwAndExit(NULL);
}

void createSemaphores() {
    if ((barberSem = sem_open(BARBER_SEM_NAME, O_CREAT | O_RDWR, 0666, 0)) == SEM_FAILED)
        throwAndExit(NULL);
    if ((queueSem = sem_open(QUEUE_SEM_NAME, O_CREAT | O_RDWR, 0666, 1)) == SEM_FAILED)
        throwAndExit(NULL);
}

void createFIFO(unsigned int chairsCount) {
    if ((queueId = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666)) == -1)
        throwAndExit(NULL);
    if (ftruncate(queueId, sizeof(PidQueue)) == -1)
        throwAndExit(NULL);
    void *shared = mmap(NULL, sizeof(PidQueue), PROT_READ | PROT_WRITE, MAP_SHARED, queueId, 0);
    if (shared == (void *) -1)
        throwAndExit(NULL);
    queue = (PidQueue *) shared;
    queue->size = 0;
    queue->chair = 0;
    queue->capacity = chairsCount;
}

void sigHandler(int sigNo) {
    if (sigNo == SIGINT) {
        printf("\nBarber, interrupted\n");
        closeShop = 1;
        freeResources();
        exit(EXIT_SUCCESS);
    }
}

