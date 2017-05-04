#include "commons.h"
#include <sys/ipc.h>
#include <sys/sem.h>

int main(int argc, char *argv[]) {
    printf("Debug Under Problem Analysis - Kamino\n");
    key_t barberKey = ftok(KEY_PATH, BARBER_KEY_ID);
    int barberId = semget(barberKey, 0, IPC_CREAT | 0666);

    // come, try to take seat
    // if taken first seat, try to get barber with NOWAIT
    // if not -> inf loop, else leave the queue, get cut, release barber, leave barbershop

    struct sembuf buffer;
    buffer.sem_num = 0;
    buffer.sem_op = -1;
    buffer.sem_flg = 0;
    semop(barberId, &buffer, 1);
    printf("Tost\n");
    return 0;
}