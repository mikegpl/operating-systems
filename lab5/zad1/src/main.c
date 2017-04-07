#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "executor.h"

static const int MAX_CMD_COUNT = 20;
static const int MAX_ARGV_LEN = 5;
static const char *PIPE_DELIMITER = "|";
static const char *WSPACE_DELIMITER = " \n\t";
static const char *ERROR_INVALID_PIPE= "Entered incorrect combination of commands and pipe signs";

int main(int argc, char *argv[]){
    char *line = NULL;
    size_t size = 0;
    while(1){
        ssize_t charsRead = getline(&line, &size, stdin);
        if(charsRead != -1){
            processLine(line, charsRead);
        }
        else{
            printf("Finished reading lines from stdin\n");
            if(line != NULL)
                free(line);
            exit(0);
        }
        printf("\nChars read %li\n", charsRead);
        printf("Size %li\n", size);
    }
    return 0;
}

void processLine(char *line, ssize_t length){
    char **lineArray = calloc(MAX_CMD_COUNT, sizeof(char *));
    Command **cmdArray = calloc(MAX_CMD_COUNT, sizeof(Command*));
    char *command;
    int cmdCount = 1;

    lineArray[0] = strtok(line, PIPE_DELIMITER);
    while((command = strtok(NULL, PIPE_DELIMITER)) != NULL && cmdCount < MAX_CMD_COUNT){
        lineArray[cmdCount++] = command;
    }

    for(int i = 0; i < cmdCount; i++){
        cmdArray[i] = parseCommand(lineArray[i]);
    }

    int toNextCmd[2];
    int in = SOURCE;
    for (int i = 0; i < cmdCount - 1; i++){
        pipe(toNextCmd);
        executeCmd(in, toNextCmd[1], cmdArray[i]);
        close(toNextCmd[TARGET]);
        in = toNextCmd[SOURCE];
    }

    if(in != SOURCE){
        dup2(in, SOURCE);
        close(in);
    }

    pid_t last = fork();
    if(last == 0){
        execvp(cmdArray[cmdCount-1]->cmd, cmdArray[cmdCount-1]->argv);
    }
    else{
        int status;
        wait(&status);
    }


    for(int i = 0; i < cmdCount; i++)
        Command_delete(cmdArray[i]);
    free(cmdArray);
    free(lineArray);
}

pid_t executeCmd(int in, int out, Command *command){
    assert(command != NULL);
    pid_t pid = fork();
    if(pid == 0){
        if(in != SOURCE){
            dup2(in, SOURCE);
            close(in);
        }
        if(out != TARGET){
            dup2(out, TARGET);
            close(out);
        }
        if(execvp(command->cmd, command->argv) == -1){
            fprintf(stderr, "Could not execute command '%s': %s\n", command->cmd, strerror(errno));
            exit(1);
        }
    }
    else{
        int status;
        wait(&status);
    }
    return pid;
}

Command *parseCommand(char *line){
    Command *cmd = Command_new();
    cmd->cmd = strtok(line, WSPACE_DELIMITER);
    if(cmd->cmd == NULL){
        fprintf(stderr, "%s\n", ERROR_INVALID_PIPE);
        exit(1);
    }

    int argCount = 1;
    char *arg;
    cmd->argv = calloc(MAX_ARGV_LEN, sizeof(char *));
    cmd->argv[0] = cmd->cmd;
    while((arg = strtok(NULL, WSPACE_DELIMITER)) != NULL && argCount < MAX_ARGV_LEN){
        cmd->argv[argCount++] = arg;
    }

    return cmd;
}

Command *Command_new(char *line){
    Command *newCommand = malloc(sizeof(Command));
    if(newCommand == NULL){
        fprintf(stderr, "Couldn't allocate memory for new command: %s\n", strerror(errno));
        exit(1);
    }
    return newCommand;
}

void Command_delete(Command *cmd){
    assert(cmd != NULL);
    free(cmd->argv);
    free(cmd);
}