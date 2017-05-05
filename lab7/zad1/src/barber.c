#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include "commons.h"

void sigHandler(int);

void barberToSleep(int semId, struct sembuf *buffer);

static int closeShop = 0;

int main(int argc, char *argv[]) {
    printf("Debug Under Problem Analysis - barber\n");
    if (argc != 2) {
        fprintf(stderr, "You should provide number of seats, so master Kenobi can choose one of them\n");
        exit(EXIT_FAILURE);
    }
    unsigned int chairsCount = (unsigned int) atol(argv[1]);

    signal(SIGINT, sigHandler);

    // create queue in shared memory
    key_t memoryKey = ftok(KEY_PATH, MEMORY_KEY_ID);
    int memoryId = shmget(memoryKey, sizeof(PidQueue), IPC_CREAT | 0666);
    PidQueue *queue = (PidQueue *) shmat(memoryId, NULL, 0);
    queue->size = 0;
    queue->capacity = chairsCount + 1;

    // create semaphore for waking the barber up
    key_t barberSemKey = ftok(KEY_PATH, BARBER_KEY_ID);
    int barberSemId = semget(barberSemKey, 1, IPC_CREAT | 0666);
    semctl(barberSemId, 0, SETVAL, 1);

    // create semaphore for operations on queue
    key_t queueSemKey = ftok(KEY_PATH, QUEUE_KEY_ID);
    int queueSemId = semget(queueSemKey, 1, IPC_CREAT | 0666);
    semctl(queueSemId, 0, SETVAL, 1);

    key_t seatFreeSemKey = ftok(KEY_PATH, SEAT_FREE_ID);
    int seatFreeSemId = semget(seatFreeSemKey, 1, IPC_CREAT | 0666);
    semctl(seatFreeSemId, 0, SETVAL, 0);

    struct sembuf buffer;
    while (!closeShop) {
        printf("Falling asleep\n");
        barberToSleep(barberSemId, &buffer);
        pid_t client = sync_PidQueue_get(queue, queueSemId, &buffer);
        if (client > 0) {
            printf("Cutting %d\n", client);
            barberToSleep(seatFreeSemId, &buffer);
        } else {
            printf("Test\n");
            continue;
        }
        while (!sync_PidQueue_isEmpty(queue, queueSemId, &buffer)) {
            printf("Invited client to get cut\n");
            kill(queue->array[0], SIGUSR1);
            barberToSleep(barberSemId, &buffer);
            printf("Cutting %d\n", sync_PidQueue_get(queue, queueSemId, &buffer));
            barberToSleep(seatFreeSemId, &buffer);
        }
    }


    shmdt(queue);
    semctl(queueSemId, 0, IPC_RMID, 0);
    semctl(barberSemId, 0, IPC_RMID, 0);
    shmctl(memoryId, IPC_RMID, NULL);
    return 0;
}

void barberToSleep(int semId, struct sembuf *buffer) {
    buffer->sem_num = 0;
    buffer->sem_op = 0;
    buffer->sem_flg = 0;
    semop(semId, buffer, 1);
}

void sigHandler(int signo) {
    if (signo == SIGINT) {
        printf("\nBarber, interrupted\n");
        closeShop = 1;
    }
}

