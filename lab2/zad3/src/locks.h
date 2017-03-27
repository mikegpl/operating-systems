#ifndef LOCKS_H
#define LOCKS_H

typedef struct flock flock;
typedef struct SelfLock SelfLock;
typedef enum AppAction AppAction;
struct SelfLock
{
	long charIndex;
	short type;
};

flock getFlockOnChar(int fileDesc, short lockType, off_t charIndex);
void printLocks(int fileDesc, SelfLock *selfLocks, long selfLocksCount);
void removeLock(SelfLock *selfLocks, long selfLocksCount, off_t charIndex);

#endif
