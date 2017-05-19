#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
bool wait = true;

void *threadJob(void *arg) {
    pthread_t myId = pthread_self();
    DESSERT("setcanceltype", pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL), 0, "Couldn't set cancel type");

    while (wait);
    pthread_testcancel();
    Record *recordArray = (Record *) malloc(recordCount * sizeof(Record));
    ASSERT("malloc", recordArray, NULL, "Couldn't alloc memory for record array");

    DESSERT("mutex_lock", pthread_mutex_lock(&mutex), 0, "Couldn't lock mutex");
    size_t recordsRead = fread(recordArray, sizeof(Record), (size_t) recordCount, fileHandle);
    DESSERT("ferror", ferror(fileHandle), 0, "Couldn't read from file");
    DESSERT("mutex_unlock", pthread_mutex_unlock(&mutex), 0, "Couldn't unlock mutex");

    while (recordsRead > 0) {
        for (int i = 0; i < recordsRead; i++) {
            if (strstr(recordArray[i].text, phrase) != NULL) {
                printf("Found phrase '%s'\t thread id: [%ld]\t record id: [%d]\n", phrase, myId,
                       recordArray[i].id);
                for (int j = 0; j < threadCount; j++) {
                    if (threadIds[j] != pthread_self()) {
                        DESSERT("pthread_cancel", pthread_cancel(threadIds[j]), 0, "Couldn't stop thread");
                    }
                }
                return (void *) 0;
            }
        }
        pthread_testcancel();
        usleep(80000);
        DESSERT("mutex_lock", pthread_mutex_lock(&mutex), 0, "Couldn't lock mutex");
        recordsRead = fread(recordArray, sizeof(Record), (size_t) recordCount, fileHandle);
        DESSERT("ferror", ferror(fileHandle), 0, "Couldn't read from file");
        DESSERT("mutex_unlock", pthread_mutex_unlock(&mutex), 0, "Couldn't unlock mutex");
    }
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
    ASSERT("fopen", fileHandle, NULL, "Couldn't open the file");
    threadIds = malloc(threadCount * sizeof(pthread_t));
    ASSERT("malloc", threadIds, NULL, "Couldn't alloc memory for thread ids");

    for (int i = 0; i < threadCount; i++) {
        DESSERT("pthread_create", pthread_create(&threadIds[i], NULL, threadJob, NULL), 0, "Couldn't create thread");
    }

    wait = false;
    for (int i = 0; i < threadCount; i++) {
        DESSERT("pthread_join", pthread_join(threadIds[i], NULL), 0, "Couldn't join threads")
    }
    fclose(fileHandle);
    free(threadIds);
    return 0;
}