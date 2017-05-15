#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "common.h"
#include "lib/utils.h"
#include "lib/pidQueue.h"

void createClone(unsigned int haircutCount);

void sendCloneToBarber(unsigned int haircutCount);

void sigHandler(int sigNo);

void getFIFO();

void getSemaphores();

void setSignalHandling();

int takeSeat(struct sembuf *buffer);

void freeResources();

static key_t projectKey;
static int queueId, semId;
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

    return 0;
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
    if ((semId = semget(projectKey, 0, 0)) == -1)
        throwAndExit(NULL);
}

void getFIFO() {
    if ((projectKey = ftok(KEY_PATH, PROJECT_ID)) == -1)
        throwAndExit(NULL);
    if ((queueId = shmget(projectKey, 0, 0)) == -1)
        throwAndExit(NULL);
    if ((queue = (PidQueue *) shmat(queueId, NULL, 0)) == (void *) -1)
        throwAndExit(NULL);
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
    struct sembuf buffer;
    pid_t myPid = getpid();
    printf("%d @ %10.ld: START\n", myPid, getTimeStamp());
    while (haircutCount < haircutsToGet) {
        takeSemaphore(semId, &buffer, FIFO);
        int result = takeSeat(&buffer);
        giveSemaphore(semId, &buffer, FIFO);

        if (result >= 0) {
            sigsuspend(&eventMask);
            printf("%d @ %10.ld: Got cut, leaving barbershop\n", myPid, getTimeStamp());
        }
    }
    printf("%d @ %10.ld: END\n", myPid, getTimeStamp());
    exit(EXIT_SUCCESS);
}

int takeSeat(struct sembuf *buffer) {
    pid_t myPid = getpid();
    int isBarberAwake = semctl(semId, 0, GETVAL);
    if (!isBarberAwake) {
        giveSemaphore(semId, buffer, BARBER);
        printf("%d @ %10.ld: Waking up the barber.\n", myPid, getTimeStamp());
        giveSemaphore(semId, buffer, BARBER);
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

void freeResources() {
    if (shmdt(queue) == -1)
        throwAndExit(NULL);
}

void sigHandler(int sigNo) {
    if (sigNo == SIGRTMIN) {
        haircutCount++;
    }
}