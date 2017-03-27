#define _XOPEN_SOURCE 500

#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ftw.h>
#include "argParser.h"

void printFileStats(char *path, char *fileName);
int checkFileSize(const char *filePath, long fileSize);
int find(const char *path, const struct stat * ptr, int flags, struct FTW *s);

static int fileCounter = 0;
static long fileSize = 0;

int main(int argc, char *argv[]){
	AppData* data = AppData_new();
	parse(argc, argv, data);
	fileSize = data->fileSize;
	nftw(data->path, find, 1, FTW_PHYS);
	printf("Files found: %d\n", fileCounter);
	AppData_delete(data);
	return 0;
}

int find(const char *path, const struct stat * ptr, int flags, struct FTW *s){
	if(checkFileSize(path, fileSize) == 0 && flags == FTW_F){
		char *filePath = malloc(PATH_MAX_LEN);
		strcpy(filePath, path);
		char *fileName = strrchr(filePath, '/') + 1;
		strrchr(filePath, '/')[0] = '\0';
		printFileStats(filePath, fileName);
		fileCounter++;
		free(filePath);
	}
	return 0;
}

int checkFileSize(const char *filePath, long fileSize){
	struct stat s;
	lstat(filePath, &s);

	if((long) s.st_size <= fileSize)
		return 0;
	else
		return 1;
}

void printFileStats(char *path, char *fileName){
	char *fileRights = calloc(9, sizeof(char));
	char *filePath = malloc(PATH_MAX_LEN);
	struct stat s;

	snprintf(filePath, PATH_MAX_LEN, "%s/%s", path, fileName);
	lstat(filePath, &s);

	strcpy(fileRights, "---------");
    if (s.st_mode & S_IRUSR) fileRights[0] = 'r';
    if (s.st_mode & S_IWUSR) fileRights[1] = 'w';
    if (s.st_mode & S_IXUSR) fileRights[2] = 'x';
    if (s.st_mode & S_IRGRP) fileRights[3] = 'r';
    if (s.st_mode & S_IWGRP) fileRights[4] = 'w';
    if (s.st_mode & S_IXGRP) fileRights[5] = 'x';
    if (s.st_mode & S_IROTH) fileRights[6] = 'r';
    if (s.st_mode & S_IWOTH) fileRights[7] = 'w';
    if (s.st_mode & S_IXOTH) fileRights[8] = 'x';

    chdir(path);
    getcwd(filePath, PATH_MAX_LEN);

    printf("File: %s\n", fileName);
    printf("Path: %s\n", filePath);
    printf("Size: %d\nLast modified: %sRights: %s\n\n", (int) s.st_size, ctime(&s.st_mtime), fileRights);
	free(fileRights);
}