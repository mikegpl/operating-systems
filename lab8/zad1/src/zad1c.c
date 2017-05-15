#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "common.h"
#include "utils.h"

FILE *fileHandle;
char *phrase;
int recordCount;
int threadCount;
pthread_t *threadIds;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_attr_t attributes;

void *threadJob(void *arg) {
    pthread_t myId = pthread_self();
    bool found = false;

    Record *recordArray = (Record *) malloc(recordCount * sizeof(Record));
    TRY("malloc", recordArray, NULL, "Couldn't alloc memory for record array");

    TRYSSERT("mutex_lock", pthread_mutex_lock(&mutex), 0, "Couldn't lock mutex");
    size_t recordsRead = fread(recordArray, sizeof(Record), (size_t) recordCount, fileHandle);
    TRYSSERT("ferror", ferror(fileHandle), 0, "Couldn't read from file");
    TRYSSERT("mutex_unlock", pthread_mutex_unlock(&mutex), 0, "Couldn't unlock mutex");

    while (recordsRead > 0) {
        for (int i = 0; i < recordsRead; i++) {
            if (recordArray == NULL)
                break;
            if (strstr(recordArray[i].text, phrase) != NULL) {
                printf("Found phrase '%s'\t thread id: [%ld]\t record id: [%d]\n", phrase, myId,
                       recordArray[i].id);
                found = true;
                for (int j = 0; j < threadCount; j++) {
                    if (threadIds[j] != pthread_self()) {
                        TRYSSERT("pthread_cancel", pthread_cancel(threadIds[j]), 0, "Couldn't stop thread");
                    }
                }
            }
        }
        TRYSSERT("mutex_lock", pthread_mutex_lock(&mutex), 0, "Couldn't lock mutex");
        recordsRead = fread(recordArray, sizeof(Record), (size_t) recordCount, fileHandle);
        TRYSSERT("ferror", ferror(fileHandle), 0, "Couldn't read from file");
        TRYSSERT("mutex_unlock", pthread_mutex_unlock(&mutex), 0, "Couldn't unlock mutex");
    }
    if (!found)
        printf("Thread id [%ld]: phrase '%s' not found\n", myId, phrase);
    return (void *) 0;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Invalid number of arguments. Usage: ./zad1a threadCount fileName recordCount phrase\n");
        exit(EXIT_FAILURE);
    }
    threadCount = atoi(argv[1]);
    char *filename = argv[2];
    recordCount = atoi(argv[3]);
    phrase = argv[4];

    fileHandle = fopen(filename, "r");
    TRY("fopen", fileHandle, NULL, "Couldn't open the file");
    threadIds = malloc(threadCount * sizeof(pthread_t));
    TRY("malloc", threadIds, NULL, "Couldn't alloc memory for thread ids");

    pthread_attr_init(&attributes);
    pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
    for (int i = 0; i < threadCount; i++) {
        TRYSSERT("pthread_create", pthread_create(&threadIds[i], &attributes, threadJob, NULL), 0,
                 "Couldn't create thread");
    }
    pthread_exit((void *) 0);
}