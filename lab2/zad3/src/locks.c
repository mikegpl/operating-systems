#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "locks.h"

flock getFlockOnChar(int fileDesc, short lockType, off_t charIndex){
	flock lock;
	lock.l_type = lockType;
	lock.l_whence = SEEK_SET;
	lock.l_start = charIndex;
	lock.l_len = 1;
	return lock;
}

void printLocks(int fileDesc, SelfLock *selfLocks, long selfLocksCount){
	long fileSize = lseek(fileDesc, 0, SEEK_END);
	if(fileSize == -1){
		perror("Error while browsing file contents");
		exit(1);
	}
	flock lock;
	long readLockCount = 0;
	long writeLockCount = 0;
	for(long i = 0; i < fileSize; i++) {
		lock = getFlockOnChar(fileDesc, F_WRLCK, i);
		long lockStart, lockEnd;
		if(fcntl(fileDesc, F_GETLK, &lock) == -1)
			perror("Printing locks - error:");
		else{
			lockStart = lseek(fileDesc, lock.l_start, lock.l_whence);
			lockEnd = lockStart + lock.l_len;

			if(lock.l_type == F_RDLCK) {
				printf("External read lock found\n\tStart byte: %ld\n\tEnd byte: %ld\n\tLock owner PID: %d\n", lockStart, lockEnd, lock.l_pid);
				readLockCount++;
			}
			else if(lock.l_type == F_WRLCK) {
				printf("External write lock found\n\tStart byte: %ld\n\tEnd byte: %ld\n\tLock owner PID: %d\n", lockStart, lockEnd, lock.l_pid);
				writeLockCount++;
			}
			i = lockEnd;
		}
	}
	for(long i = 0; i < selfLocksCount; i++){
		if(selfLocks[i].charIndex != -1){
			if(selfLocks[i].type == F_RDLCK){
				printf("Internal read lock found\n\tStart byte: %ld\n\tEndByte: %ld\n\t\n", selfLocks[i].charIndex, selfLocks[i].charIndex + 1);
				readLockCount++;
			}
			else if(selfLocks[i].type == F_WRLCK){
				printf("Internal write lock found\n\tStart byte: %ld\n\tEndByte: %ld\n\t\n", selfLocks[i].charIndex, selfLocks[i].charIndex + 1);
				writeLockCount++;
			}
		}
	}
	printf("Found %ld locks in this file: %ld read locks and %ld write locks\n", readLockCount + writeLockCount, readLockCount, writeLockCount);
}

void removeLock(SelfLock *selfLocks, long selfLocksCount, off_t charIndex){
	for(long i = 0; i < selfLocksCount; i++){
		if(selfLocks[i].charIndex == charIndex){
			selfLocks[i].charIndex = -1;
			break;
		}
	}
}
