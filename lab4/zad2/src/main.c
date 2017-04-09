#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>

typedef int bool;
#define true 1
#define false 0

static int direction = 1;
static const char *SIGNAL_ERROR = "Error occured while setting a signal handler";
static const char *SIGNAL_MESSAGE = "Caught SIGTSTP, reversing";
static const char *EXIT_MESSAGE = "Received SIGINT, quitting";

void signalHandler(int signr);

int main(int argc, char *argv[]){
	struct sigaction action;
	action.sa_handler = signalHandler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	if(sigaction(SIGINT, &action, NULL) == -1){
		fprintf(stderr, "%s\n", SIGNAL_ERROR);
		exit(EXIT_FAILURE);
	}
	if(signal(SIGTSTP, signalHandler) == SIG_ERR){
		fprintf(stderr, "%s\n", SIGNAL_ERROR);
		exit(EXIT_FAILURE);
	}
	pid_t pid = getpid();
	printf("My pid: %d\n", pid);

	for(int i = 0;; i = (i + 26 + direction) % 26){
		printf("%c\n", (char) 65 + i);
		sleep(1);
	}

	return 0;
}

void signalHandler(int signr){
	if(signr == SIGTSTP){
	printf("\n%s\n", SIGNAL_MESSAGE);
		direction *= -1;
	}
	if(signr == SIGINT){
		printf("\n%s\n", EXIT_MESSAGE);
		exit(0);
	}	
}
