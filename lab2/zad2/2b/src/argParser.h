#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <stddef.h>

static const size_t PATH_MAX_LEN = 256;
typedef struct AppData AppData;
struct AppData{
	char *path;
	long fileSize;
};

void parse(int, char**, AppData*);
void inputError(const char*);

AppData *AppData_new();
void AppData_delete(AppData*);
#endif