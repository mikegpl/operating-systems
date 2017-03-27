#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include "interpreter.h"

static const char *HELP = "Usage: \n./main pathToFile memoryLimit timeLimit \nInterprets file at pathToFile \nmemoryLimit [MB], timeLimit[s]";
static const char *DELIMITERS = "\n\t ";
static const int MAX_ARGV_LEN = 8;

static long lineSize = 0;
static long lineCount = 1;
static long commandCount = 0;

static rlimit memoryLimit = {RLIMIT_AS};
static rlimit timeLimit = {RLIMIT_CPU};

void interpretFile(char *source);
void getResourceLimits(int argc, char *argv[]);
static void setResourceLimits();
void execLine(char *line, int length);
ssize_t fileGetLine(char **linePtr, ssize_t *lineLen, FILE *file);
EnvVarAction parseEnvCommand(char *line);
Command parseExecCommand(char *line);
void envCommand(EnvVarAction action);
void execCommand(Command command);
void printExecutionStats(pid_t pid, rusage r);

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("%s\n", HELP);
		return 0;
	}
	else if(access(argv[1], F_OK) == -1){
		printf("File %s does not exist\n", argv[1]);
		return 1;
	}

	getResourceLimits(argc, argv);
	interpretFile(argv[1]);
	return 0;
}


void getResourceLimits(int argc, char *argv[]){
	if(argc == 4){
		memoryLimit.rlim_cur = memoryLimit.rlim_max = (rlim_t) 1000000*atoll(argv[2]);
		timeLimit.rlim_cur = timeLimit.rlim_max = (rlim_t) atoll(argv[3]);
	}
	else{
		if(getrlimit(RLIMIT_AS, &memoryLimit) == -1){
			fprintf(stderr, "Error while getting memory limit: %s\n", strerror(errno));
			exit(1);
		}
		if(getrlimit(RLIMIT_CPU, &timeLimit) == -1){
			fprintf(stderr, "Error while gettin cpu time lmit: %s\n", strerror(errno));
			exit(1);
		}
	}
}

void setResourceLimits(){
	if(setrlimit(RLIMIT_AS, &memoryLimit) == -1){
		fprintf(stderr, "Error while setting memory limit%s\n", strerror(errno));
		exit(1);
	}
	if(setrlimit(RLIMIT_CPU, &timeLimit) == -1){
		fprintf(stderr, "Error while setting time limit%s\n", strerror(errno));
		exit(1);
	}
}

void interpretFile(char *source){
	char *line;
	FILE *file = fopen(source, "r");
	if(file == NULL){
		fprintf(stderr, "Error opening file %s: %s\n", source, strerror(errno));
		exit(1);
	}	

	while(fileGetLine(&line, &lineSize, file) != EOF){
		execLine(line, lineSize);
		lineCount++;
	}

	printf("Interpreted %ld and executed %ld lines in %s\n", lineCount - 1, commandCount, source);

	int status = fclose(file);
	if(status == EOF){
		fprintf(stderr, "Error while closing the file \n");
		exit(1);
	}
}

ssize_t fileGetLine(char **linePtr, ssize_t *lineLen, FILE *file){
	*lineLen = getline(linePtr, (size_t *) lineLen, file);
	if(*lineLen != -1)
		return *lineLen;
	else if(ferror(file) != 0){
		fprintf(stderr, "Error while reading line: %s\n", strerror(errno));
		exit(1);
	}
	else if(feof(file) != 0)
		return EOF;
	else
		exit(1);
}

void execLine(char *line, int length) {
	if(length == 0)
		return;
	else if(line[0] == '#'){
		envCommand(parseEnvCommand(line));
	}
	else{
		execCommand(parseExecCommand(line));
	}
}

EnvVarAction parseEnvCommand(char *line){
	EnvVarAction action;
	action.varName = strtok(line + 1, DELIMITERS);
	action.varValue = strtok(NULL, DELIMITERS);
	if(action.varValue)
		action.type = 's';
	else
		action.type = 'u';
	return action;
}

Command parseExecCommand(char *line){
	Command command;
	command.cmd = strtok(line, DELIMITERS);
	command.argv = calloc(MAX_ARGV_LEN, sizeof(char *));
	command.argv[0] = command.cmd;

	char *arg;
	int argCount = 1;
	while((arg = strtok(NULL, DELIMITERS)) != NULL && argCount < MAX_ARGV_LEN){
		if(arg[0] == '$')
			arg = getenv(arg +1);
		command.argv[argCount++] = arg;
	}
	return command; 
}

void envCommand(EnvVarAction action){
	if(action.varName != NULL){
		switch(action.type){
			case 's':
				if(setenv(action.varName, action.varValue, true) == -1){
					fprintf(stderr, "Error setting environment variable: %s\n", strerror(errno));
					exit(1);
				}
				break;
			case 'u':
				if(unsetenv(action.varName) == -1){
					fprintf(stderr, "Error unsetting environment variable %s\n", strerror(errno));
					exit(1);
				}
				break;
		}
		commandCount++;
	}
}

void execCommand(Command command){
	if(command.cmd != NULL){
		rusage r;
		int status = 0;
		pid_t pid = fork();
		if(pid == 0){
			setResourceLimits();
			printf("Executing command from line %ld\n", lineCount);
			if(execvp(command.cmd, command.argv) == -1){
				fprintf(stderr, "Cannot execute command %s: %s\n", command.cmd, strerror(errno));
				exit(1);
			}
		}	
		else if(pid > 0){
			pid = wait3(&status, 0, &r);
			fprintf(stderr, "Child process ended with exit code %d\n", WEXITSTATUS(status));

			if(WEXITSTATUS(status) != 0){
				fprintf(stderr, "Error while executing command at line %ld\n", lineCount);
				exit(WEXITSTATUS(status));
			}

       		printExecutionStats(pid, r);
			free(command.argv);
		}
		else{
			fprintf(stderr, "Error while forking\n");
			exit(1);
		}
		commandCount++;
	}
}

void printExecutionStats(pid_t pid, rusage r){
	printf("%s: %d\t", "PID", pid);
	printf("%s: %6.likB\t", "Max resident mem", r.ru_maxrss);
	double time1 = (double) r.ru_utime.tv_sec * 1000 + r.ru_utime.tv_usec / 1000.0;
	double time2 = (double) r.ru_stime.tv_sec * 1000 + r.ru_stime.tv_usec / 1000.0;
	printf("%s: %8.3fms\t", "System time", time1);
	printf("%s: %8.3fms\n\n", "User time", time2);
	
}

