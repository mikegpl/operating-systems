#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "commons.h"

void sigHandler(int);

static int closeShop = 0;

int main(int argc, char *argv[]) {
    printf("Debug Under Problem Analysis\n");
    signal(SIGINT, sigHandler);

    key_t memoryKey = ftok(KEY_PATH, MEMORY_KEY_ID);
    int memoryId = shmget(memoryKey, sizeof(PidQueue), IPC_CREAT | 0600);
    PidQueue *queue = (PidQueue *) shmat(memoryId, NULL, 0);
    queue->size = 0;

    key_t barberKey = ftok(KEY_PATH, BARBER_KEY_ID);
    int barberId = semget(barberKey, 1, IPC_CREAT | 0666);

    // pack this to barberToSleep();
    semctl(barberId, 0, SETVAL, 1);
    struct sembuf buffer;
    buffer.sem_num = 0;
    buffer.sem_op = 0;
    buffer.sem_flg = 0;

    while (!closeShop) {
        // sleep until client comes
        // cut him (get his pid from queue)
        semop(barberId, &buffer, 1);
        printf("Wake me up inside, (can't wake up)\n");
        break;
    }


    shmdt(queue);
    semctl(barberId, 0, IPC_RMID, 0);
    shmctl(memoryId, IPC_RMID, NULL);
    return 0;
}

void sigHandler(int signo) {
    if (signo == SIGINT) {
        printf("Barber, interrupted\n");
        closeShop = 1;
    }
}