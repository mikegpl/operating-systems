#include "commons.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>


void createClone(unsigned int haircutCount);

void sendCloneToBarber(unsigned int haircutCount);

void sigHandler(int sigNo);

void getFIFO();

void getSemaphores();

void setSignalHandling();

void takeSemaphore(int semArrayId, struct sembuf *buffer, unsigned short semIndex);

void giveSemaphore(int semArrayId, struct sembuf *buffer, unsigned short semIndex);

int takeSeat(struct sembuf *buffer);

const char *ERROR_FORK = "Error occured while forking";

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

void setSignalHandling() {
    signal(SIGUSR1, sigHandler);
    sigfillset(&eventMask);
    sigdelset(&eventMask, SIGUSR1);
    sigset_t blockMask;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &blockMask, NULL);
}

void getSemaphores() {
    semId = semget(projectKey, 0, 0);
    printf("%d, %d\n", semctl(semId, BARBER, GETVAL), semctl(semId, FIFO, GETVAL));
}

void getFIFO() {
    projectKey = ftok(KEY_PATH, PROJECT_ID);
    queueId = shmget(projectKey, 0, 0);
    queue = (PidQueue *) shmat(queueId, NULL, 0);
}

void createClone(unsigned int haircutsToGet) {
    pid_t pid = fork();
    if (pid == 0) {
        sendCloneToBarber(haircutsToGet);
    } else if (pid < 0) {
        fprintf(stderr, "%s\n", ERROR_FORK);
        exit(EXIT_FAILURE);
    }
}

void sendCloneToBarber(unsigned int haircutsToGet) {
    printf("%d: Entering the town to get %d haircuts.\n", getpid(), haircutsToGet);
    struct sembuf buffer;
    while (haircutCount < haircutsToGet) {
        takeSemaphore(semId, &buffer, FIFO);
        int result = takeSeat(&buffer);
        giveSemaphore(semId, &buffer, FIFO);

        if (result >= 0) {
            sigsuspend(&eventMask);
            printf("%d: got cut\n", getpid());
        }

    }
    printf("%d: Leaving the town with my hair cut.\n\n", getpid());
    exit(EXIT_SUCCESS);
}

int takeSeat(struct sembuf *buffer) {
    pid_t myPid = getpid();
    int isBarberAwake = semctl(semId, 0, GETVAL);
    if (!isBarberAwake) {
        giveSemaphore(semId, buffer, BARBER);
        printf("%d: Waking up the barber.\n", myPid);
        giveSemaphore(semId, buffer, BARBER);
        queue->chair = myPid;
        return 1;
    } else {
        if (PidQueue_put(queue, myPid) == -1) {
            printf("%d: couldn't find a free seat\n", myPid);
            return -1;
        } else {
            printf("%d: I have taken place in the queue\n", myPid);
            return 0;
        }
    }
}

void sigHandler(int sigNo) {
    if (sigNo == SIGUSR1) {
        haircutCount++;
    }
}