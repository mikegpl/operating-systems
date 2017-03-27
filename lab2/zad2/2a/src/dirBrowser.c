#include "argParser.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void listFiles(char *path, long fileSize);
void printFileStats(char *path, char *fileName);
int checkFileSize(char *filePath, long fileSize);

static int fileCounter = 0;

int main(int argc, char *argv[]){
	AppData* data = AppData_new();
	char *buffer = malloc(PATH_MAX);
	parse(argc, argv, data);
	chdir(data->path);
	getcwd(buffer, PATH_MAX);
	
	listFiles(buffer, data->fileSize);

	printf("Files found: %d\n", fileCounter);
	AppData_delete(data);
	free(buffer);
	return 0;
}

void listFiles(char *path, long fileSize){
	struct stat s;
	struct dirent* file;
	char filePath[PATH_MAX];
	char *fileName;

	DIR *root = opendir(path);
	if(!root){
		fprintf(stderr, "Cannot open directory '%s': %s\n", filePath, strerror (errno));
		return;
	}

	while((file = readdir(root)) != NULL){
		fileName = file->d_name;
		snprintf(filePath, PATH_MAX, "%s/%s", path, fileName);
		lstat(filePath, &s);

		if(S_ISDIR(s.st_mode)){
			if(strcmp(fileName, "..") != 0 && strcmp(fileName, ".") != 0) {
				int pathLength;
				char directoryPath[PATH_MAX];

				pathLength = snprintf(directoryPath, PATH_MAX, "%s/%s", path, fileName);
				if(pathLength >= PATH_MAX) {
					printf("Path length is too long\n");
					exit(-1);
				}

				directoryPath[pathLength] = '\0';
				listFiles(directoryPath, fileSize);
			}
		}
		else if(S_ISREG(s.st_mode)) {
			if(checkFileSize(filePath, fileSize) == 0){
				printFileStats(filePath, fileName);
				fileCounter++;
			} 
		}
	}
	closedir(root);
}

int checkFileSize(char *filePath, long fileSize){
	struct stat s;
	lstat(filePath, &s);

	if(fileSize >= (long) s.st_size)
		return 0;
	else
		return 1;
}

void printFileStats(char *filePath, char *fileName){
	char *fileRights = calloc(9, sizeof(char));

	struct stat s;
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

    printf("File: %s\n", fileName);
    printf("Path: %s\n", filePath);
    printf("Size: %d\nLast modified: %sRights: %s\n\n", (int) s.st_size, ctime(&s.st_mtime), fileRights);
	free(fileRights);
}