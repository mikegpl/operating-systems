#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "commons.h"

// todo -> add error handling

void sigHandler(int);

void createFIFO(unsigned int chairsCount);

void createSemaphores();

void freeResources();

void sleepAndWork();

void takeSemaphore(int semArrayId, struct sembuf *buffer, unsigned short semIndex);

void giveSemaphore(int semArrayId, struct sembuf *buffer, unsigned short semIndex);

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
    // todo -> add raise() for errors -> ERROR + errno + freeResources()
    unsigned int chairsCount = (unsigned int) atol(argv[1]);
    signal(SIGINT, sigHandler);

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
        takeSemaphore(semId, &buffer, BARBER);
        printf("Barbershop quartet: barber is now awake\n");
        pid_t cloneId = getFirstClone(&buffer);
        giveHaircut(cloneId);
        while (1) {
            takeSemaphore(semId, &buffer, FIFO);
            cloneId = PidQueue_get(queue);
            if (cloneId != 0) {
                printf("Barbershop quartet: barber is giving haircuts to clients from queue\n");
                giveSemaphore(semId, &buffer, FIFO);
                giveHaircut(cloneId);
            } else {
                takeSemaphore(semId, &buffer, BARBER);
                giveSemaphore(semId, &buffer, FIFO);
                printf("Barbershop quartet: barber is falling asleep\n");
                break;
            }
        }
    }
}

void giveHaircut(pid_t cloneId) {
    printf("Barbershop quartet: %d has taken the seat, giving haircut - start\n", cloneId);
    kill(cloneId, SIGUSR1);
    printf("Barbershop quartet: %d has been given a haircut\n", cloneId);
}

pid_t getFirstClone(struct sembuf *buffer) {
    takeSemaphore(semId, buffer, FIFO);
    pid_t cloneId = queue->chair;
    giveSemaphore(semId, buffer, FIFO);
    return cloneId;
}

void giveSemaphore(int semArrayId, struct sembuf *buffer, unsigned short semIndex) {
    buffer->sem_num = semIndex;
    buffer->sem_op = 1;
    buffer->sem_flg = 0;
    semop(semArrayId, buffer, 1);
}

void takeSemaphore(int semArrayId, struct sembuf *buffer, unsigned short semIndex) {
    buffer->sem_num = semIndex;
    buffer->sem_op = -1;
    buffer->sem_flg = 0;
    semop(semArrayId, buffer, 1);
}

void freeResources() {
    shmdt(queue);
    shmctl(queueId, IPC_RMID, NULL);
    semctl(semId, 0, IPC_RMID, 0);
}

void createSemaphores() {
    semId = semget(projectKey, 2, IPC_CREAT | 0666);
    semctl(semId, BARBER, SETVAL, 0);
    semctl(semId, FIFO, SETVAL, 1);
    printf("%d, %d\n", semctl(semId, BARBER, GETVAL), semctl(semId, FIFO, GETVAL));

}

void createFIFO(unsigned int chairsCount) {
    projectKey = ftok(KEY_PATH, PROJECT_ID);
    queueId = shmget(projectKey, sizeof(PidQueue), IPC_CREAT | 0666);
    queue = (PidQueue *) shmat(queueId, NULL, 0);
    queue->size = 0;
    queue->chair = 0;
    queue->capacity = chairsCount;
}

void sigHandler(int signo) {
    if (signo == SIGINT) {
        printf("\nBarber, interrupted\n");
        closeShop = 1;
        freeResources();
        exit(EXIT_SUCCESS);
    }
}

