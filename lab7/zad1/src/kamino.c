#include "commons.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/shm.h>

void createClone(unsigned int haircutCount);

void sendCloneToBarber(unsigned int haircutCount);

void sigHandler(int sigNo);

void getHaircut();

const char *ERROR_FORK = "Error occured while forking";
int barberSemId, memoryId, queueSemId, seatFreeSemId;
PidQueue *queue;

int waitInQueue;

int main(int argc, char *argv[]) {
    printf("Debug Under Problem Analysis - Kamino\n");

    if (argc != 3) {
        fprintf(stderr, "You should provide clone order details: number of clones and number of haircuts\n");
        exit(EXIT_FAILURE);
    }
    unsigned int cloneCount = (unsigned int) atol(argv[1]);
    unsigned int haircutCount = (unsigned int) atol(argv[2]);
    signal(SIGUSR1, sigHandler);

    key_t barberSemKey = ftok(KEY_PATH, BARBER_KEY_ID);
    barberSemId = semget(barberSemKey, 0, IPC_CREAT | 0666);

    key_t memoryKey = ftok(KEY_PATH, MEMORY_KEY_ID);
    memoryId = shmget(memoryKey, sizeof(PidQueue), IPC_CREAT | 0666);
    queue = (PidQueue *) shmat(memoryId, NULL, 0);

    key_t queueSemKey = ftok(KEY_PATH, QUEUE_KEY_ID);
    queueSemId = semget(queueSemKey, 1, IPC_CREAT | 0666);

    key_t seatFreeSemKey = ftok(KEY_PATH, SEAT_FREE_ID);
    seatFreeSemId = semget(seatFreeSemKey, 1, IPC_CREAT | 0666);

    int status;
    for (unsigned int i = 0; i < cloneCount; i++) {
        createClone(haircutCount);
    }
    while (wait(&status) > 0);

    return 0;
}

void createClone(unsigned int haircutCount) {
    pid_t pid = fork();
    if (pid == 0) {
        sendCloneToBarber(haircutCount);
    } else if (pid < 0) {
        fprintf(stderr, "%s\n", ERROR_FORK);
        exit(EXIT_FAILURE);
    }
}

void sendCloneToBarber(unsigned int haircutCount) {
    printf("%d: Entering the town to get %d haircuts.\n", getpid(), haircutCount);
    struct sembuf buffer;
    while (haircutCount) {
        waitInQueue = 1;
        if (sync_PidQueue_put(queue, queueSemId, &buffer, getpid()) != -1) {
            printf("%d: Entered the barbershop and got seat\n", getpid());
            getHaircut();
            haircutCount--;
        } else {
            printf("%d: There were no free seats\n", getpid());
        }
    }
    printf("%d: Leaving the town with my hair cut.\n", getpid());
    exit(0);
}

void getHaircut() {
    struct sembuf buffer;
    takeSemaphore(queueSemId, &buffer, 0);
    if (queue->array[0] == getpid()) {
        if (takeSemaphore(barberSemId, &buffer, IPC_NOWAIT) != -1) {
            giveSemaphore(seatFreeSemId, &buffer, 0);
            giveSemaphore(queueSemId, &buffer, 0);
            printf("%d: Getting cut\n", getpid());
            giveSemaphore(barberSemId, &buffer, 0);
            takeSemaphore(seatFreeSemId, &buffer, 0);
            return;
        }
        giveSemaphore(queueSemId, &buffer, 0);
        printf("Something wrong happened\n");
    } else {
        giveSemaphore(queueSemId, &buffer, 0);
        waitInQueue = 1;
        while (waitInQueue);
        printf("Received signal to get cut\n");

        if (takeSemaphore(barberSemId, &buffer, IPC_NOWAIT) != -1) {
            printf("%d: Getting cut\n", getpid());
            // sleep waiting for cut with another semaphore
            giveSemaphore(barberSemId, &buffer, 0);
            return;
        }
        printf("Something wrong happened\n");
    }
}

void sigHandler(int sigNo) {
    if (sigNo == SIGUSR1) {
        waitInQueue = 0;
    }
}

