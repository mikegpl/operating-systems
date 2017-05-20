#include <pthread.h>
#include <stdbool.h>
#include "lib/utils.h"
#include "config.h"

void getResources();

void freeResources();

void *writerJob(void *);

void *readerJob(void *);


static unsigned int readersCount = 0, writersCount = 0;
static unsigned int readersWaiting = 0;
static unsigned int reads = 0, writes = 0;
static unsigned int divisor = 1;
static bool verbose = false, wait = true;
static sem_t *readers, *writers, *resource;
static int memoryId;
static int *memory;
static pthread_t *threadIds;

int main(int argc, char *argv[]) {
    if (argc == 6 && (strcmp(argv[1], "-i") == 0)) {
        verbose = true;
    } else if (argc != 5) {
        printf("Invalid arguments.\nUsage: ./zada [-i] writersCount readersCount writes reads\n\t-i - print info about operations\n");
        exit(EXIT_FAILURE);
    }
    int offset = verbose ? 1 : 0;
    writersCount = (unsigned int) atoi(argv[1 + offset]);
    readersCount = (unsigned int) atoi(argv[2 + offset]);
    writes = (unsigned int) atoi(argv[3 + offset]);
    reads = (unsigned int) atoi(argv[4 + offset]);

    srand((unsigned int) time(NULL));
    divisor = 2 + (unsigned int) rand() % MAX_DIVISOR;

    getResources();

    for (int i = 0; i < writersCount; i++) {
        ASSERT("pthread_create", pthread_create(&threadIds[i], NULL, writerJob, NULL), 0, "Couldn't create thread");
    }

    for (int i = writersCount; i < writersCount + readersCount; i++) {
        ASSERT("pthread_create", pthread_create(&threadIds[i], NULL, readerJob, (void *) &divisor), 0,
               "Couldn't create thread");
    }

    wait = false;
    for (int i = 0; i < writersCount + readersCount; i++) {
        ASSERT("pthread_join", pthread_join(threadIds[i], NULL), 0, "Couldn't join threads")
    }
    freeResources();
    return 0;
}

void getResources() {
    readers = createSemaphore(READERS_SEM, 1);
    writers = createSemaphore(WRITERS_SEM, 1);
    resource = createSemaphore(RESOURCES_SEM, 1);
    memory = (int *) createSharedMemory(&memoryId, SHARED_MEM, ARRAY_SIZE * sizeof(int));
    threadIds = (pthread_t *) calloc(writersCount + readersCount, sizeof(pthread_t));
    DESSERT("calloc", threadIds, NULL, "Error wile allocating memory");
}

void freeResources() {
    deleteSharedMemory((void *) memory, SHARED_MEM, ARRAY_SIZE * sizeof(int));
    deleteSemaphore(resource, RESOURCES_SEM);
    deleteSemaphore(readers, READERS_SEM);
    deleteSemaphore(writers, WRITERS_SEM);
    free(threadIds);
}

void *writerJob(void *args) {
    while (wait);
    if (verbose)
        printf("[%ld] Created writer\n", getTimeStamp());
    for (int i = 0; i < writes; i++) {
        takeSemaphore(resource);
        int toModify = rand() % ARRAY_SIZE;
        for (int j = 0; j < toModify; j++) {
            int index = rand() % ARRAY_SIZE;
            int value = rand() % MAX_NUMBER;
            if (verbose)
                printf("[%ld] Writer saving %d at [%d]\n", getTimeStamp(), value, index);
            memory[index] = value;
        }
        printf("[%ld] Writer done, modified %d numbers\n", getTimeStamp(), toModify);
        giveSemaphore(resource);
    }
    return (void *) 0;
}

void *readerJob(void *args) {
    int divisor = *((int *) args);
    while (wait);
    if (verbose)
        printf("[%ld] Created reader with divisor %d\n", getTimeStamp(), divisor);
    for (int i = 0; i < reads; i++) {
        takeSemaphore(readers);
        readersWaiting++;
        if (readersWaiting == 1)
            takeSemaphore(resource);
        giveSemaphore(readers);

        int theDivisibles = 0;
        for (int j = 0; j < ARRAY_SIZE; j++) {
            if (memory[j] % divisor == 0) {
                theDivisibles++;
                if (verbose)
                    printf("[%ld] Reader found %d at [%d]\n", getTimeStamp(), memory[j], j);
            }
        }
        printf("[%ld] Reader done, found %d numbers\n", getTimeStamp(), theDivisibles);

        takeSemaphore(readers);
        readersWaiting--;
        if (readersWaiting == 0)
            giveSemaphore(resource);
        giveSemaphore(readers);
    }

    return (void *) 0;
}


