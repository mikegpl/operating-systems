#ifndef EXECUTOR_H
#define EXECUTOR_H

#define TARGET 1
#define SOURCE 0

typedef struct Command Command;

struct Command{
	char *cmd;
	char **argv;
};

void processLine(char *line, ssize_t length);
Command *parseCommand(char *line);
Command *Command_new();
void Command_delete(Command *cmd);
void executeCmd(int index, int in, int out, Command *command);
#endif