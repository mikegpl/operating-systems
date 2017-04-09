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

static const int MAX_CMD_COUNT = 20;
static const int MAX_ARGV_LEN = 5;
static const char *PIPE_DELIMITER = "|";
static const char *WSPACE_DELIMITER = " \n\t";
static const char *ERROR_INVALID_PIPE= "Entered incorrect combination of commands and pipe signs";
#endif