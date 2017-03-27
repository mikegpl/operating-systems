#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "argParser.h"

void generate(AppData *data);
void shuffle(AppData *data);
void sort(AppData *data);

void shuffleLib(char *target, int recordCount, int recordSize);
void shuffleSys(char *target, int recordCount, int recordSize);

void sortLib(char *target, int recordCount, int recordSize);
void sortSys(char *target, int recordCount, int recordSize);

FILE *file_fopen(char *filePath, char *flags);
int file_fclose(FILE *file);
size_t file_fread(void *target, size_t recordSize, size_t recordCount, FILE *source);
size_t file_fwrite(void *source, size_t recordSize, size_t recordCount, FILE *target);
int file_fseek(FILE* file, int offset, int whence);

int file_open(char *filePath, int flag);
int file_close(int fdesc);
ssize_t file_read(int fdesc, void *target, size_t readSize);
ssize_t file_write(int fdesc, void *source, size_t writeSize);
int file_lseek(int fdesc, int offset, int whence);


#endif