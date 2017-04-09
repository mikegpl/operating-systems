#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <string.h>

typedef enum Type{
	KILL, SIGQUEUE, REALTIME
} Type;

int stringToInt(char *number);
void parentTask(Type type, int signalsToSend, pid_t child);
void childTask();
void printSignalStats(char *who, int sent, int received);
void setChildSigHandlers();
void setParentSigHandlers();
void parentSigHandler(int signr);
void childSigHandler(int signr);

static const char *USAGE = "Usage: ./main signalCount type\n\tType - {0 - kill, 1 - sigqueue, 2 - realtime kill}";
static const char *ERROR_ARGNUM = "Error - entered invalid number of args";
static const char *ERROR_STR_INT = "Error while converting string to int";
static const char *ERROR_SIGMASK = "Error while setting signal mask";
static const char *ERROR_SIGACT = "Error while setting sigactions";
static const char *ERROR_CHILD_SIG_PARENT = "Error while sending signal from child to parent";
static const char *ERROR_PARENT_SIG_CHILD = "Error while sending signal from parent to child";
static const char *ERROR_WRONG_TYPE = "Error - used incorrect enum value (of enum 'Type')";

int receivedCount;
int sentCount;
int exitFlag;
pid_t parent;

int main(int argc, char *argv[]){
	if(argc == 2 && strcmp("--help", argv[1]) == 0) {
		printf("%s\n", USAGE);
		exit(0);
	}
	else if(argc != 3) {
		fprintf(stderr, "%s.\nFor usage use '--help' opt\n", ERROR_ARGNUM);
		exit(1);
	}

	int signalsToSend = stringToInt(argv[1]);
	Type type = (Type) stringToInt(argv[2]);
	receivedCount = sentCount = exitFlag = 0;
	parent = getpid();

	setChildSigHandlers();

	pid_t pid = fork();
	if(pid > 0){
		setParentSigHandlers();
		parentTask(type, signalsToSend, pid);
	}
	else if(pid == 0){
		childTask();
	}
	else{
		fprintf(stderr, "Error while forking: %s\n", strerror(errno));
	}
	return 0;
}

int stringToInt(char *number){
	assert(number != NULL);
	char *stopAt;
	int value = (int) strtol(number, &stopAt, 10);
	if(number == stopAt || value < 0){
		printf("%s\n", ERROR_STR_INT);
		exit(1);
	}
	return value;
}

void parentTask(Type type, int signalsToSend, pid_t child){
	for(int i = 0; i < signalsToSend; i++){
		switch(type){
			case KILL:
			if(kill(child, SIGUSR1) == -1){
				fprintf(stderr, "%s\n", ERROR_PARENT_SIG_CHILD);
				exit(1);
			}
			break;
			case SIGQUEUE:
			if(sigqueue(child, SIGUSR1, (union sigval) {0}) == -1){
				fprintf(stderr, "%s\n", ERROR_PARENT_SIG_CHILD);
				exit(1);
			}
			break;
			case REALTIME:
			if(kill(child, SIGRTMIN) == -1){
				fprintf(stderr, "%s\n", ERROR_PARENT_SIG_CHILD);
				exit(1);
			}
			break;
			default:
				fprintf(stderr, "%s\n", ERROR_WRONG_TYPE);
				exit(1);
		}
		sentCount++;
		struct timespec toSleep = {0, (int) 1e9 / 10};
		nanosleep(&toSleep, &toSleep);
	}
	switch(type){
			case KILL:
			if(kill(child, SIGUSR2) == -1){
				fprintf(stderr, "%s\n", ERROR_PARENT_SIG_CHILD);
				exit(1);
			}
			break;
			case SIGQUEUE:
			if(sigqueue(child, SIGUSR2, (union sigval) {0}) == -1){
				fprintf(stderr, "%s\n", ERROR_PARENT_SIG_CHILD);
				exit(1);
			}
			break;
			case REALTIME:
			if(kill(child, SIGRTMIN + 1) == -1){
				fprintf(stderr, "%s\n", ERROR_PARENT_SIG_CHILD);
				exit(1);
			}
			break;
	}
	struct timespec sleepUntilResponds = {0, (int) 1e9 / 10};
	nanosleep(&sleepUntilResponds, &sleepUntilResponds);

	printSignalStats("Parent", sentCount, receivedCount);
}

void childTask(){
	while(exitFlag != 1)
		pause();
	printSignalStats("Child", receivedCount, sentCount);
}

void printSignalStats(char *who, int sent, int received){
	printf("%s sent: %5d \t received: %5d\n", who, sent, received);
}

void setChildSigHandlers(){
	sigset_t sigmask;
	int sigs[4] = {SIGUSR1, SIGUSR2, SIGRTMIN, SIGRTMIN+1};
	if(sigemptyset(&sigmask) == -1){
		fprintf(stderr, "%s\n", ERROR_SIGMASK);
		exit(1);
	}
	for(int i = 0; i < 4; i++){
		if(sigaddset(&sigmask, sigs[i]) == -1){
			fprintf(stderr, "%s\n", ERROR_SIGMASK);
			exit(1);
		}
	}

	struct sigaction act;
	act.sa_handler = childSigHandler;
	act.sa_mask = sigmask;
	act.sa_flags = 0;
	for(int i = 0; i < 4; i++){
		if(sigaction(sigs[i], &act, NULL) == -1){
			fprintf(stderr, "Child: %s\n", ERROR_SIGACT);
		}
	}
}

void setParentSigHandlers(){
	sigset_t sigmask;
	if(sigemptyset(&sigmask) == -1){
		fprintf(stderr, "%s\n", ERROR_SIGMASK);
		exit(1);
	}
	int sigs[2] = {SIGUSR1, SIGRTMIN};

	struct sigaction act;
	act.sa_handler = parentSigHandler;
	act.sa_mask = sigmask;
	act.sa_flags = 0;
	for(int i = 0; i < 2; i++){
		if(sigaction(sigs[i], &act, NULL) == -1){
			fprintf(stderr, "Parent: %s\n", ERROR_SIGACT);
		}
	}

}

void parentSigHandler(int signr){
	receivedCount++;
}

void childSigHandler(int signr){
	if(signr == SIGUSR2 || signr == SIGRTMIN+1) {
		exitFlag = 1;
		return;
	}
	else{
		receivedCount++;
		if(kill(parent, signr) == -1){
			fprintf(stderr, "%s\n", ERROR_CHILD_SIG_PARENT);
			printSignalStats("Child", receivedCount, sentCount);
			exit(1);
		}
		sentCount++;
	}
}