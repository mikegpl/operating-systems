#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "locks.h"

static const int OPTIONS_LEN = 9;
static const char *OPTIONS[] = {"0 - exit", 
								"1 - set waiting read lock", 
							    "2 - set immediate read lock",
								"3 - set waiting write lock",
								"4 - set immediate write lock",
								"5 - print all locks on file",
								"6 - unlock",
								"7 - read char",
								"8 - write char"};
static const char *HELP_INFO = "Use '--help' for example usage";
static const char *HELP = "./main filename";


void mainLoop();
void inputError(char const* msg);
long getIndex();



int main(int argc, char *argv[]) {
	char *fileName;
	if(argc == 2 && strcmp(argv[1], "--help") == 0) {
		printf("%s\n", HELP);
		exit(0);
	} else if(argc != 2) {
		printf("%s\n", HELP_INFO);
		exit(1);
	} else {
		fileName = argv[1];
		if(access(fileName, F_OK) == -1) 
			inputError("file doesn't exist");
		mainLoop(fileName);
	}
	return 0;
}

void mainLoop(char *fileName) {
	char userInput;
	long byteIndex;
	long selfLocksMax = 64;
	long selfLocksCount = 0;
	flock lock;
	SelfLock *selfLocks = malloc(selfLocksMax * sizeof(SelfLock));

	int fileDesc = open(fileName, O_RDWR);
	if(fileDesc == -1)
		inputError("cannot open this file");
	

	int exitFlag = 0;
	while(!exitFlag){
		for(int i = 0; i < OPTIONS_LEN; i++)
			printf("%s\n", OPTIONS[i]);

		userInput = getchar();
		getchar();
		char fileChar;
		switch(userInput){
			case '0':
				exitFlag = 1;
				break;
			case '1':
				printf("DEBUG1\n");
				byteIndex = getIndex();
				lock = getFlockOnChar(fileDesc, F_RDLCK, byteIndex);
				if(fcntl(fileDesc, F_SETLKW, &lock) == -1) {
					printf("Error occured while setting waiting read lock\n");
				} else {
					printf("Waiting read lock is set\n");
					if(selfLocksCount < selfLocksMax - 1){
						SelfLock slock = {byteIndex, F_RDLCK};
						selfLocks[selfLocksCount++] = slock;
					}
					else{
						selfLocks = realloc(selfLocks, selfLocksMax * 2);
						selfLocksMax *= 2;
						SelfLock slock = {byteIndex, F_RDLCK};
						selfLocks[selfLocksCount++] = slock;
					}
				}
				break;
			case '2':
				printf("DEBUG2\n");
				byteIndex = getIndex();
				lock = getFlockOnChar(fileDesc, F_RDLCK, byteIndex);
				if(fcntl(fileDesc, F_SETLK, &lock) == -1) {
					printf("Error while setting immediate read lock\n");
				} else {
					printf("Immediate read lock is set\n");
					if(selfLocksCount < selfLocksMax - 1){
						SelfLock slock = {byteIndex, F_RDLCK};
						selfLocks[selfLocksCount++] = slock;
					}
					else{
						selfLocks = realloc(selfLocks, selfLocksMax * 2);
						selfLocksMax *= 2;
						SelfLock slock = {byteIndex, F_RDLCK};
						selfLocks[selfLocksCount++] = slock;
					}
				}
				break;
			case '3':
				byteIndex = getIndex();
				printf("Index: %ld\n", byteIndex);
				lock = getFlockOnChar(fileDesc, F_WRLCK, byteIndex);
				if(fcntl(fileDesc, F_SETLKW, &lock) == -1) {
					printf("Error occured while setting waiting write lock\n\n");
				} else {
					printf("Waiting write lock is set\n");
					if(selfLocksCount < selfLocksMax - 1){
						SelfLock slock = {byteIndex, F_WRLCK};
						selfLocks[selfLocksCount++] = slock;
					}
					else{
						selfLocks = realloc(selfLocks, selfLocksMax * 2);
						selfLocksMax *= 2;
						SelfLock slock = {byteIndex, F_WRLCK};
						selfLocks[selfLocksCount++] = slock;
					}
				}
				break;
			case '4':
				byteIndex = getIndex();
				printf("Index: %ld\n", byteIndex);
				lock = getFlockOnChar(fileDesc, F_WRLCK, byteIndex);
				if(fcntl(fileDesc, F_SETLK, &lock) == -1) {
					printf("Error occured while setting immediate write lock\n\n");
				} else {
					printf("Immediate write lock is set\n");
					if(selfLocksCount < selfLocksMax - 1){
						SelfLock slock = {byteIndex, F_WRLCK};
						selfLocks[selfLocksCount++] = slock;
					}
					else{
						selfLocks = realloc(selfLocks, selfLocksMax * 2);
						selfLocksMax *= 2;
						SelfLock slock = {byteIndex, F_WRLCK};
						selfLocks[selfLocksCount++] = slock;
					}
				}
				break;
			case '5':
				printLocks(fileDesc, selfLocks, selfLocksCount);
				break;
			case '6':
				byteIndex = getIndex();
				lock = getFlockOnChar(fileDesc, F_UNLCK, byteIndex);
				if(fcntl(fileDesc, F_SETLK, &lock) == -1) {
					printf("Error occured while trying to unlock byte\n");
				} else {
					printf("Byte %ld is now unlocked\n", byteIndex);
					removeLock(selfLocks, selfLocksCount, byteIndex);
				}
				break;
			case '7':
				byteIndex = getIndex();
				lseek(fileDesc, byteIndex, SEEK_SET);
				lock = getFlockOnChar(fileDesc, F_RDLCK, byteIndex);
				if(fcntl(fileDesc, F_GETLK, &lock) == -1) {
					printf("Error occured while trying to read char %ld", byteIndex);
				} else {
					if(lock.l_type != F_RDLCK) {
						if(read(fileDesc, &fileChar, 1) == -1) {
							printf("Read Error\n");
						} else {
							printf("Read successfully");
							printf("Byte nr %ld: %c\n", byteIndex, fileChar);
						}
					} else {
						printf("Can't read byte nr %ld\n", byteIndex);
					}
				}
				break;
			case '8':
				byteIndex = getIndex();
				lseek(fileDesc, byteIndex, SEEK_SET);
				lock = getFlockOnChar(fileDesc, F_WRLCK, byteIndex);
				if(fcntl(fileDesc, F_GETLK, &lock) == -1) {
					printf("Error occured while trying to write char %ld\n", byteIndex);
				} else {
					getchar();
					printf("Enter character to be written\n");
					fileChar = getchar();
					if(lock.l_type != F_WRLCK) {
						if(write(fileDesc, &fileChar, 1) == -1) {
							printf("Write Error\n");
						} else {
							printf("Written successfully\n");
						}
					} else {
						printf("Can't overwrite byte nr %ld\n", byteIndex);
					}
				}
				break;
		}

		if(!exitFlag) {
			printf("\nPress any key\n");
			getchar();
			getchar();
		}
		system("clear");
	}

	free(selfLocks);
	if(close(fileDesc) == -1) {
		inputError("Can't close the file");
	}
}

long getIndex(){
	printf("Enter char index: ");
	long i;
	scanf("%ld", &i);
	if(i < 0) {
		printf("Error: invalid index\n");
		i = getIndex();
	}
	return i;
}

void inputError(char const* msg) {
	printf("Error occured: %s\n", msg);
	exit(1);
}
