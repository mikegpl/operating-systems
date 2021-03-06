#ifndef ZADA_UTILS_H
#define ZADA_UTILS_H

#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>

#define DESSERT(FUN, VAL, COMP, ERR) \
  if(VAL == COMP) {\
  fprintf(stderr, "%s: %s\n", FUN, ERR); \
  exit(EXIT_FAILURE); \
  } \

#define ASSERT(FUN, VAL, COMP, ERR) \
  if(VAL != COMP) {\
  fprintf(stderr, "%s: %s\n", FUN, ERR); \
  exit(EXIT_FAILURE); \
  } \


void takeSemaphore(sem_t *semaphore);

void giveSemaphore(sem_t *semaphore);

sem_t *createSemaphore(const char *name, int defaultValue);

void deleteSemaphore(sem_t *semaphore, const char *name);

void *createSharedMemory(int *memoryId, const char *name, size_t size);

void deleteSharedMemory(void *memory, const char *name, size_t size);

long getTimeStamp();

#endif //ZADA_UTILS_H
