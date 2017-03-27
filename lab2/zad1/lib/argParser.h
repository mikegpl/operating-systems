#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <stddef.h>

static const int MAX_FILENAME_LEN = 64;

typedef struct AppData AppData;
typedef enum Provider Provider;
typedef enum AppAction AppAction;

enum Provider{
	LIBRARY, SYSTEM, DEFAULT
};

enum AppAction{
	GENERATE, SHUFFLE, SORT
};

struct AppData{
	Provider provider;
	AppAction action;
	char *source;
	char *target;
	int recordCount;
	int recordSize;
};



AppData *AppData_new();
void AppData_delete(AppData *data);
void parseArgs(int argc, char *argv[], AppData *data);
Provider getProvider(int argc, char *argv[]);
AppAction getAction(int argc, char *argv[]);
int getRecordCount(int argc, char *argv[]);
int getRecordSize(int argc, char *argv[]);
int argToInt(int argc, char *argv[], int argNumber);
#endif