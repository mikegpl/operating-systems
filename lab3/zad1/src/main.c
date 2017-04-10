#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include "interpreter.h"

static const char *HELP = "./main pathToFile \nInterprets file at pathToFile";
static const char *DELIMITERS = "\n\t ";
static const int MAX_ARGV_LEN = 5;

static long lineSize = 0;
static long lineCount = 1;
static long commandCount = 0;

void interpretFile(char *source);

void execLine(char *line, int length);
ssize_t fileGetLine(char **linePtr, ssize_t *lineLen, FILE *file);
EnvVarAction parseEnvCommand(char *line);
Command parseExecCommand(char *line);
void envCommand(EnvVarAction action);
void execCommand(Command command);

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("%s\n", HELP);
        return 0;
    }
    else if(access(argv[1], F_OK) == -1){
        printf("File %s does not exist\n", argv[1]);
        return 1;
    }

    interpretFile(argv[1]);
    return 0;
}

void interpretFile(char *source){
    char *line;
    FILE *file = fopen(source, "r");
    if(file == NULL){
        char *error = strerror(errno);
        fprintf(stderr, "Error opening file %s: %s\n", source, error);
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
            printf("Executing command from line %ld\n", lineCount);
            if(execvp(command.cmd, command.argv) == -1){
                fprintf(stderr, "Cannot execute command %s: %s\n", command.cmd, strerror(errno));
                exit(1);
            }
        }   
        else if(pid > 0){
            pid = wait3(&status, 0, &r);
            fprintf(stderr, "Child process ended with exit code %d\n\n", WEXITSTATUS(status));

            if(WEXITSTATUS(status) != 0){
                fprintf(stderr, "Error while executing command at line %ld\n", lineCount);
                exit(WEXITSTATUS(status));
            }
            free(command.argv);
        }
        else{
            fprintf(stderr, "Error while forking\n");
            exit(1);
        }
        commandCount++;
    }
}
