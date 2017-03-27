#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "argParser.h"

static const int HELP_SIZE = 7;
static const char *HELP[] = {"Usage: ./prog provider action [Source] Target RecordCount RecordSize",
							"Available providers: lib, sys, default", 
							"Available actions: generate, shuffle, sort",
							"Source (optional): specify source for data generation", 
							"Target: specify path to the target file for operation", 
							"RecordCount: number of records in target file",
							"RecordSize: size of record (in bytes) in target file"};
static const char *HELP_INFO = "Use --help for usage information";
static const char *INVALID_ARGCOUNT = "Entered incorrect number of arguments";
static const char *INVALID_PROVIDER = "Entered invalid provider";
static const char *INVALID_ACTION = "Entered invalid action";
static const char *INVALID_NUMERIC_ARG = "Entered invalid integer as argument";
static const char *INVALID_SOURCE = "Entered invalid path to source file";

static const char *DEFAULT_SOURCE = "/dev/random";

void parseArgs(int argc, char *argv[], AppData *data){
	assert(data != NULL);
	if(argc == 2 && strcmp(argv[1], "--help") == 0){
		for(int i = 0; i < HELP_SIZE; i++)
			printf("%s\n", HELP[i]);
		exit(0);
	}
	else if(argc < 5 || argc > 7){
		printf("%s\n", INVALID_ARGCOUNT);
		printf("%s\n", HELP_INFO);
		exit(1);
	}
	else{
		data->provider = getProvider(argc, argv);
		data->action = getAction(argc, argv);
		data->recordCount = getRecordCount(argc, argv);
		data->recordSize = getRecordSize(argc, argv);
		if(argc == 6){
			strcpy(data->source, DEFAULT_SOURCE);
			strcpy(data->target, argv[3]);
		}
		else{
			if(access(argv[3], F_OK) == -1){
				printf("%s\n", INVALID_SOURCE);
				printf("%s\n", HELP_INFO);
				exit(1);
			}
			strcpy(data->source, argv[3]);
			strcpy(data->target, argv[4]);
		}
	}
}

Provider getProvider(int argc, char *argv[]){
	if(strcmp(argv[1], "lib") == 0)
		return LIBRARY;
	else if(strcmp(argv[1], "sys") == 0)
		return SYSTEM;
	else if(strcmp(argv[1], "default") == 0)
		return DEFAULT;
	else {
		printf("%s\n", INVALID_PROVIDER);
		printf("%s\n", HELP_INFO);
		exit(1);
	}
}

AppAction getAction(int argc, char *argv[]){
	if(strcmp(argv[2], "generate") == 0)
		return GENERATE;
	else if(strcmp(argv[2], "shuffle") == 0)
		return SHUFFLE;
	else if(strcmp(argv[2], "sort") == 0)
		return SORT;
	else{
		printf("%s\n", INVALID_ACTION);
		printf("%s\n", HELP_INFO);
		exit(1);
	}
}

int getRecordCount(int argc, char *argv[]){
	if(argc == 6)
		return argToInt(argc, argv, 4);	
	else
		return argToInt(argc, argv, 5);
}

int getRecordSize(int argc, char *argv[]){
	if(argc == 6)
		return argToInt(argc, argv, 5);
	else
		return argToInt(argc, argv, 6);
}


int argToInt(int argc, char *argv[], int argNumber){
	assert(argNumber < argc);
	char *parsedAt;
	int argument = (int) strtol(argv[argNumber], &parsedAt, 10);
	if(argv[argNumber] == parsedAt || argument <= 0){
		printf("%s\n", INVALID_NUMERIC_ARG);
		printf("%s\n", HELP_INFO);
		exit(1);
	}
	else
		return argument;
}

AppData *AppData_new(){
	AppData *data = malloc(sizeof(AppData));
	data->source = malloc(MAX_FILENAME_LEN);
	data->target = malloc(MAX_FILENAME_LEN);
	data->recordCount = data->recordSize = 0;
	return data;
}

void AppData_delete(AppData *data){
	assert(data != NULL);
	free(data->source);
	free(data->target);
	free(data);
}