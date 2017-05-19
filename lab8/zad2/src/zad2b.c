#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>
#include "utils.h"

bool wait = true;

void signalHandler(int sigNr) {
    printf("Pid: [%d], Thread id: [%ld], sigNr: [%d]\n", getpid(), pthread_self(), sigNr);
}

void *threadJob1(void *arg) {
    for (int i = 0; i < 10000; i++) {
        printf("Inside loop\n");
        sleep(1);
    }
    return (void *) 0;
}

void *threadJob2(void *arg) {
    printf("This is ZERO ZERO thread\nThread id: [%ld]\n", pthread_self());
    while (wait);
    int test = 5 / 0;
    return (void *) 0;
}

int main(int argc, char *argv[]) {
    signal(SIGFPE, signalHandler);

    pthread_t *threadIds = (pthread_t *) malloc(sizeof(pthread_t) * 200);
    for (int i = 0; i < 9; i++) {
        DESSERT("pthread_create", pthread_create(&threadIds[i], NULL, threadJob1, NULL), 0, "Couldn't create thread");
    }

    DESSERT("pthread_create", pthread_create(&threadIds[199], NULL, threadJob2, NULL), 0, "Couldn't create thread");
    sleep(2);
    wait = false;

    for (int i = 0; i < 10; i++) {
        DESSERT("pthread_join", pthread_join(threadIds[i], NULL), 0, "Couldn't join threads")
    }
    free(threadIds);
    return 0;
}