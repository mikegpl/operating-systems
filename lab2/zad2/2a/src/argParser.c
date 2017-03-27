#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "argParser.h"

static const char *HELP_INFO = "Use '--help' for usage information";
static const char *USAGE_INFO = "Usage: ./main path fileSize";

static const char *INVALID_FILESIZE = "Entered incorrect fileSize";
static const char *INVALID_FILE = "Filepath doesn't exist";
static const char *INVALID_ARGCOUNT = "Incorrect number of args (should be 2)";

void parse(int argc, char** argv, AppData *data){
	assert(data != NULL);
	if(argc == 2 && strcmp(argv[1], "--help") == 0){
		printf("%s\n", USAGE_INFO);
		exit(0);
	}
	else if(argc != 3) {
		inputError(INVALID_ARGCOUNT);
	}
	else{
		char *t;
		long fileSize = strtol(argv[2], &t, 10);
		if(argv[2] == t || fileSize < 0)
			inputError(INVALID_FILESIZE);
		if(access(argv[1], F_OK) == -1)
			inputError(INVALID_FILE);
		data->fileSize = fileSize;
		strcpy(data->path, argv[1]);
	}
}

void inputError(const char *msg) {
	printf("%s\n", msg);
	printf("%s\n", HELP_INFO);
	exit(1);
}

AppData *AppData_new(){
	AppData *newData = calloc(1, sizeof(AppData));
	newData->path = malloc(PATH_MAX_LEN);
	newData->fileSize = 0;
	return newData;
}

void AppData_delete(AppData* data){
	assert(data != NULL);
	free(data->path);
	free(data);
}