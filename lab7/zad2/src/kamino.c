#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include "common.h"
#include "lib/utils.h"
#include "lib/pidQueue.h"

void createClone(unsigned int haircutCount);

void sendCloneToBarber(unsigned int haircutCount);

void sigHandler(int sigNo);

void getFIFO();

void getSemaphores();

void setSignalHandling();

int takeSeat();

void freeResources();

static int queueId;
static sem_t *barberSem, *queueSem;
static PidQueue *queue;
static sigset_t eventMask;
unsigned int haircutCount;

int main(int argc, char *argv[]) {
    printf("Kamino\n");

    if (argc != 3) {
        printf("You should provide clone order details: number of clones and number of haircuts\n");
        exit(EXIT_FAILURE);
    }
    unsigned int cloneCount = (unsigned int) atol(argv[1]);
    unsigned int haircutsToGet = (unsigned int) atol(argv[2]);
    haircutCount = 0;

    getFIFO();
    getSemaphores();
    setSignalHandling();

    int status;
    for (unsigned int i = 0; i < cloneCount; i++) {
        createClone(haircutsToGet);
    }
    while (wait(&status) > 0);
    freeResources();
    return 0;
}

void freeResources() {
    if (munmap((void *) queue, sizeof(PidQueue)) == -1)
        throwAndExit(NULL);
    if (sem_close(barberSem) == -1)
        throwAndExit(NULL);
    if (sem_close(queueSem) == -1)
        throwAndExit(NULL);
}

void setSignalHandling() {
    if (signal(SIGRTMIN, sigHandler) == (void *) -1)
        throwAndExit(freeResources);
    if (sigfillset(&eventMask) == -1)
        throwAndExit(freeResources);
    if (sigdelset(&eventMask, SIGRTMIN) == -1)
        throwAndExit(freeResources);
    sigset_t blockMask;
    if (sigemptyset(&blockMask) == -1)
        throwAndExit(freeResources);
    if (sigaddset(&blockMask, SIGRTMIN) == -1)
        throwAndExit(freeResources);
    if (sigprocmask(SIG_BLOCK, &blockMask, NULL) == -1)
        throwAndExit(freeResources);
}

void getSemaphores() {
    if ((barberSem = sem_open(BARBER_SEM_NAME, O_RDWR)) == SEM_FAILED)
        throwAndExit(NULL);
    if ((queueSem = sem_open(QUEUE_SEM_NAME, O_RDWR)) == SEM_FAILED)
        throwAndExit(NULL);
}

void getFIFO() {
    if ((queueId = shm_open(SHARED_MEM_NAME, O_RDWR, 0666)) == -1)
        throwAndExit(NULL);
    void *shared = mmap(NULL, sizeof(PidQueue), PROT_READ | PROT_WRITE, MAP_SHARED, queueId, 0);
    if (shared == (void *) -1)
        throwAndExit(NULL);
    queue = (PidQueue *) shared;
}

void createClone(unsigned int haircutsToGet) {
    pid_t pid = fork();
    if (pid == 0) {
        sendCloneToBarber(haircutsToGet);
    } else if (pid < 0) {
        throwAndExit(freeResources);
    }
}

void sendCloneToBarber(unsigned int haircutsToGet) {
    pid_t myPid = getpid();
    printf("%d @ %10.ld: START\n", myPid, getTimeStamp());
    while (haircutCount < haircutsToGet) {
        takeSemaphore(queueSem);
        int result = takeSeat();
        giveSemaphore(queueSem);

        if (result >= 0) {
            sigsuspend(&eventMask);
            printf("%d @ %10.ld: Got cut, leaving barbershop\n", myPid, getTimeStamp());
        }
    }
    printf("%d @ %10.ld: END\n", myPid, getTimeStamp());
    exit(EXIT_SUCCESS);
}

int takeSeat() {
    pid_t myPid = getpid();
    int isBarberAwake;
    if (sem_getvalue(barberSem, &isBarberAwake) == -1)
        throwAndExit(freeResources);
    if (!isBarberAwake) {
        giveSemaphore(barberSem);
        printf("%d @ %10.ld: Waking up the barber.\n", myPid, getTimeStamp());
        giveSemaphore(barberSem);
        queue->chair = myPid;
        return 1;
    } else {
        if (PidQueue_put(queue, myPid) == -1) {
            printf("%d @ %10.ld: Leaving barbershop (queue is full)\n", myPid, getTimeStamp());
            return -1;
        } else {
            printf("%d @ %10.ld: Taking place in queue\n", myPid, getTimeStamp());
            return 0;
        }
    }
}

void sigHandler(int sigNo) {
    if (sigNo == SIGRTMIN) {
        haircutCount++;
    }
}