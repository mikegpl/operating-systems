#ifndef INTERPRETER_H
#define INTERPRETER_H

typedef int bool;
#define true 1
#define false 0

typedef struct Command Command;
typedef struct EnvVarAction EnvVarAction;
typedef struct AppData AppData;
typedef struct rusage rusage;
typedef struct rlimit rlimit;

struct Command{
	char *cmd;
	char **argv;
};

struct EnvVarAction{
	char type;
	char *varName;
	char *varValue;
};

#endif