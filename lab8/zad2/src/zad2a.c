#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include "common.h"
#include "utils.h"


FILE *fileHandle;
char *phrase;
int recordCount;
int threadCount;
int testOption;
int signalNumber;
pthread_t *threadIds;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool wait = true;

void signalHandler(int sigNr) {
    printf("Pid: [%d], Thread id: [%ld], sigNr: [%d]\n", getpid(), pthread_self(), sigNr);
}

void *threadJob(void *arg) {
    pthread_t myId = pthread_self();
    switch (testOption) {
        case 4:
            signal(signalNumber, SIG_IGN);
            break;
        case 3:
        case 5:
            signal(signalNumber, signalHandler);
            break;
        default:
            break;
    }

    while (wait);

    Record *recordArray = (Record *) malloc(recordCount * sizeof(Record));
    TRY("malloc", recordArray, NULL, "Couldn't alloc memory for record array");

    TRYSSERT("mutex_lock", pthread_mutex_lock(&mutex), 0, "Couldn't lock mutex");
    size_t recordsRead = fread(recordArray, sizeof(Record), (size_t) recordCount, fileHandle);
    TRYSSERT("ferror", ferror(fileHandle), 0, "Couldn't read from file");
    TRYSSERT("mutex_unlock", pthread_mutex_unlock(&mutex), 0, "Couldn't unlock mutex");

    while (recordsRead > 0) {
        for (int i = 0; i < recordsRead; i++) {
            if (strstr(recordArray[i].text, phrase) != NULL) {
                printf("Found phrase '%s'\t thread id: [%ld]\t record id: [%d]\n", phrase, myId,
                       recordArray[i].id);
                return (void *) 0;
            }
        }
        TRYSSERT("mutex_lock", pthread_mutex_lock(&mutex), 0, "Couldn't lock mutex");
        recordsRead = fread(recordArray, sizeof(Record), (size_t) recordCount, fileHandle);
        TRYSSERT("ferror", ferror(fileHandle), 0, "Couldn't read from file");
        TRYSSERT("mutex_unlock", pthread_mutex_unlock(&mutex), 0, "Couldn't unlock mutex");
    }
    printf("Thread id [%ld]: phrase '%s' not found\n", myId, phrase);
    return (void *) 0;
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        printf("Invalid number of arguments. Usage: ./zad1a threadCount fileName recordCount phrase sigNr testOpt\n");
        exit(EXIT_FAILURE);
    }
    threadCount = atoi(argv[1]);
    char *filename = argv[2];
    recordCount = atoi(argv[3]);
    phrase = argv[4];
    signalNumber = atoi(argv[5]);
    testOption = atoi(argv[6]);

    fileHandle = fopen(filename, "r");
    TRY("fopen", fileHandle, NULL, "Couldn't open the file");
    threadIds = malloc(threadCount * sizeof(pthread_t));
    TRY("malloc", threadIds, NULL, "Couldn't alloc memory for thread ids");

    //  TESTS
    switch (testOption) {
        case 2:
            signal(signalNumber, SIG_IGN);
            break;
        case 3:
            signal(signalNumber, signalHandler);
            break;
        default:
            break;
    }
    // \TESTS


    for (int i = 0; i < threadCount; i++) {
        TRYSSERT("pthread_create", pthread_create(&threadIds[i], NULL, threadJob, NULL), 0, "Couldn't create thread");
    }

    wait = false;
    switch (testOption) {
        case 1:
        case 2:
        case 3:
            kill(getpid(), signalNumber);
            break;
        case 4:
        case 5:
            printf("Signaling 1st thread [%ld] with %d\n", threadIds[0], signalNumber);
            pthread_kill(threadIds[0], signalNumber);
            break;
        default:
            break;
    }
    //  TESTS 2
    if (testOption == 2)
        kill(getpid(), signalNumber);
    // \TESTS 2
    for (int i = 0; i < threadCount; i++) {
        TRYSSERT("pthread_join", pthread_join(threadIds[i], NULL), 0, "Couldn't join threads")
    }
    fclose(fileHandle);
    free(threadIds);
    return 0;
}