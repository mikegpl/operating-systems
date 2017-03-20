#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "argParser.h"

static const char *HELP_INFO = "Use '--help' for usage";
static const char *USAGE_INFO = "Usage: ./main path fileSize";
static const char *FILESIZE_ERROR = "Entered incorrect fileSize";
static const char *FIlENONEXISTENT_ERROR = "Filepath doesn't exist";
static const char *ARGNUMBER_ERROR = "Incorrect number of args (should be 2)";

void parse(int argc, char** argv, AppData *data){
	if(argc == 2 && strcmp(argv[1], "--help") == 0){
		printf("%s\n", USAGE_INFO);
	}
	else if(argc != 3) {
		inputError(ARGNUMBER_ERROR);
	}
	else{
		char *t;
		long fileSize = strtol(argv[2], &t, 10);
		if(argv[2] == t || fileSize < 0)
			inputError(FILESIZE_ERROR);
		if(access(argv[1], F_OK) == -1)
			inputError(FIlENONEXISTENT_ERROR);
		data->fileSize = fileSize;
		strcpy(data->path, argv[1]);
	}
}

void inputError(const char *msg) {
	printf("Error: %s\n", msg);
	printf("%s\n", HELP_INFO);
	exit(-1);
}

AppData *AppData_new(){
	AppData *newData = calloc(1, sizeof(AppData));
	newData->path = malloc(PATH_MAX);
	newData->fileSize = 0;
	return newData;
}

void AppData_delete(AppData* data){
	free(data->path);
	free(data);
}