#include <signal.h>
#include <sys/shm.h>
#include "common.h"
#include "lib/utils.h"
#include "lib/pidQueue.h"

void sigHandler(int sigNo);

void createFIFO(unsigned int chairsCount);

void createSemaphores();

void freeResources();

void sleepAndWork();

pid_t getFirstClone(struct sembuf *buffer);

void giveHaircut(pid_t cloneId);

static key_t projectKey;
static int queueId, semId;
static PidQueue *queue;
int closeShop = 0;


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
    struct sembuf buffer;
    while (!closeShop) {
        printf("Barbershop quartet @ %10.ld: Barber is going to sleep\n", getTimeStamp());
        takeSemaphore(semId, &buffer, BARBER);
        pid_t cloneId = getFirstClone(&buffer);
        giveHaircut(cloneId);
        while (1) {
            takeSemaphore(semId, &buffer, FIFO);
            cloneId = PidQueue_get(queue);
            if (cloneId != 0) {
                giveSemaphore(semId, &buffer, FIFO);
                giveHaircut(cloneId);
            } else {
                takeSemaphore(semId, &buffer, BARBER);
                giveSemaphore(semId, &buffer, FIFO);
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

pid_t getFirstClone(struct sembuf *buffer) {
    takeSemaphore(semId, buffer, FIFO);
    pid_t cloneId = queue->chair;
    giveSemaphore(semId, buffer, FIFO);
    return cloneId;
}

void freeResources() {
    if (shmdt(queue) == -1)
        throwAndExit(NULL);
    if (shmctl(queueId, IPC_RMID, NULL) == -1)
        throwAndExit(NULL);
    if (semctl(semId, 0, IPC_RMID, 0) == -1)
        throwAndExit(NULL);
}

void createSemaphores() {
    if ((semId = semget(projectKey, 2, IPC_CREAT | 0666)) == -1)
        throwAndExit(freeResources);
    if (semctl(semId, BARBER, SETVAL, 0) == -1)
        throwAndExit(freeResources);
    if (semctl(semId, FIFO, SETVAL, 1) == -1)
        throwAndExit(freeResources);
}

void createFIFO(unsigned int chairsCount) {
    if ((projectKey = ftok(KEY_PATH, PROJECT_ID)) == -1)
        throwAndExit(NULL);
    if ((queueId = shmget(projectKey, sizeof(PidQueue), IPC_CREAT | 0666)) == -1)
        throwAndExit(NULL);
    if ((queue = (PidQueue *) shmat(queueId, NULL, 0)) == (void *) -1)
        throwAndExit(NULL);
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

