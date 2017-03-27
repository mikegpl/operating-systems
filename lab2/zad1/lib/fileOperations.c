#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "fileOperations.h"

void generate(AppData *data){
	assert(data != NULL);

	unsigned char **records = calloc(data->recordCount, data->recordSize);
	
	FILE *source = file_fopen(data->source, "rb");
	file_fread(records, data->recordSize, data->recordCount, source);
	file_fclose(source);

	FILE *target = file_fopen(data->target, "w");
	file_fwrite(records, data->recordSize, data->recordCount, target);
	file_fclose(target);

	printf("Successfully generated data to %s\n", data->target);
	free(records);
}

void shuffle(AppData *data){
	assert(data != NULL);
	switch(data->provider){
		case DEFAULT:
		case LIBRARY:
			shuffleLib(data->target, data->recordCount, data->recordSize);
			break;
		case SYSTEM:
			shuffleSys(data->target, data->recordCount, data->recordSize);
			break;
		default:
			printf("Error: invalid provider");
			exit(1);
	}
	printf("Successfully shuffled data in %s\n", data->target);
}

void sort(AppData *data){
	assert(data != NULL);
	switch(data->provider){
		case DEFAULT:
		case LIBRARY:
			sortLib(data->target, data->recordCount, data->recordSize);
			break;
		case SYSTEM:
			sortSys(data->target, data->recordCount, data->recordSize);
			break;
		default:
			printf("Error: invalid provider");
			exit(1);
	}
	printf("Successfully sorted data in %s\n", data->target);

}

void shuffleLib(char *target, int recordCount, int recordSize){
	srand(time(NULL));
	FILE *file = file_fopen(target, "r+b");
	unsigned char *a = malloc(recordSize);
	unsigned char *b = malloc(recordSize);

	for(int i = 0; i < recordCount - 1; i++){
		int j = i + (int) ((rand() / (double) RAND_MAX) * (recordCount - i - 1));

		if(i == j)
			continue;

		file_fseek(file, recordSize * i, SEEK_SET);
		file_fread(a, recordSize, 1, file);
		file_fseek(file, recordSize * (j - i - 1), SEEK_CUR);
		file_fread(b, recordSize, 1, file);

		file_fseek(file, recordSize * i, SEEK_SET);
		file_fwrite(b, recordSize, 1, file);
		file_fseek(file, recordSize * (j - i - 1), SEEK_CUR);
		file_fwrite(a, recordSize, 1, file);
	}


	free(a);
	free(b);
	file_fclose(file);
	return;
}

void shuffleSys(char *target, int recordCount, int recordSize){
	srand(time(NULL));
	int fdesc = file_open(target, O_RDWR);
	unsigned char *a = malloc(recordSize);
	unsigned char *b = malloc(recordSize);

	for(int i = 0; i < recordCount - 1; i++){
		int j = i + (int) ((rand() / (double) RAND_MAX) * (recordCount - i - 1));

		if(i == j)
			continue;

		file_lseek(fdesc, recordSize * i, SEEK_SET);
		file_read(fdesc, a, recordSize);
		file_lseek(fdesc, recordSize * (j - i - 1), SEEK_CUR);
		file_read(fdesc, b, recordSize);

		file_lseek(fdesc, recordSize * i, SEEK_SET);
		file_write(fdesc, b, recordSize);
		file_lseek(fdesc, recordSize * (j - i - 1), SEEK_CUR);
		file_write(fdesc, a, recordSize);
	}

	free(a);
	free(b);
	file_close(fdesc);
}

void sortLib(char *target, int recordCount, int recordSize){
	FILE *file = file_fopen(target, "r+b");
	unsigned char *a0 = malloc(1);
	unsigned char *b0 = malloc(1);
	unsigned char *a = malloc(recordSize);
	unsigned char *b = malloc(recordSize);

	for(int i = recordCount - 1; i > 0; i--){
		for(int j = 0; j < i; j++){
			file_fseek(file, recordSize * j, SEEK_SET);
			file_fread(a0, 1, 1, file);
			file_fseek(file, recordSize * (i - j) - 1, SEEK_CUR);
			file_fread(b0, 1, 1, file);

			if(a0[0] > b0[0]){
				file_fseek(file, -1, SEEK_CUR);
				file_fread(b, recordSize, 1, file);
				file_fseek(file, -1 * recordSize * (i - j + 1), SEEK_CUR);
				file_fread(a, recordSize, 1, file);
				file_fseek(file, -1 * recordSize, SEEK_CUR);
				file_fwrite(b, recordSize, 1, file);
				file_fseek(file, recordSize * (i - j - 1), SEEK_CUR);
				file_fwrite(a, recordSize, 1, file);
			}
		}
	}

	free(a0);
	free(b0);
	free(a);
	free(b);
	file_fclose(file);
}

void sortSys(char *target, int recordCount, int recordSize){
	int fdesc = file_open(target, O_RDWR);
	unsigned char *a0 = malloc(1);
	unsigned char *b0 = malloc(1);
	unsigned char *a = malloc(recordSize);
	unsigned char *b = malloc(recordSize);

	for(int i = recordCount - 1; i > 0; i--){
		for(int j = 0; j < i; j++){
			file_lseek(fdesc, recordSize * j, SEEK_SET);
			file_read(fdesc, a0, 1);
			file_lseek(fdesc, recordSize * (i - j) - 1, SEEK_CUR);
			file_read(fdesc, b0, 1);

			if(a0[0] > b0[0]){
				file_lseek(fdesc, -1, SEEK_CUR);
				file_read(fdesc, b, recordSize);
				file_lseek(fdesc, -1 * recordSize * (i - j + 1), SEEK_CUR);
				file_read(fdesc, a, recordSize);
				file_lseek(fdesc, -1 * recordSize, SEEK_CUR);
				file_write(fdesc, b, recordSize);
				file_lseek(fdesc, recordSize * (i - j - 1), SEEK_CUR);
				file_write(fdesc, a, recordSize);
			}
		}
	}

	free(a0);
	free(b0);
	free(a);
	free(b);
	file_close(fdesc);
}

int file_open(char *filePath, int flag){
	int fdesc = open(filePath, flag);
	if (fdesc == -1){
		perror("Error opening file");
		exit(1);
	}
	return fdesc;
}

int file_close(int fdesc){
	int status = close(fdesc);
	if(status == -1){
		perror("Error closing file");
		exit(1);
	}
	return status;
}

ssize_t file_read(int fdesc, void *target, size_t readSize){
	ssize_t recordsRead = read(fdesc, target, readSize);
	if(recordsRead == -1){
		fprintf(stderr, "Error while reading the file\n");
		exit(1);
	}
	else if(recordsRead == 0){
		fprintf(stderr, "Reached end of the file while reading\n");
	}
	else if(recordsRead < readSize){
		fprintf(stderr, "Read %d, expected %d\n", (int) recordsRead, (int) readSize);
	}
	return recordsRead;
}

ssize_t file_write(int fdesc, void *source, size_t writeSize){
	ssize_t recordsWritten = write(fdesc, source, writeSize);
	if(recordsWritten == -1){
		fprintf(stderr, "Error while reading the file\n");
		exit(1);
	}
	else if(recordsWritten < writeSize){
		fprintf(stderr, "Written %d, expected %d\n", (int) recordsWritten, (int) writeSize);
	}
	return recordsWritten;
}

int file_lseek(int fdesc, int offset, int whence){
	int status = lseek(fdesc, offset, whence);
	if(status == -1){
		perror("Error while searching for byte in file");
		exit(1);
	}
	return status;
}

FILE *file_fopen(char *filePath, char *flags){
	FILE *file = fopen(filePath, flags);
	if(file == NULL) {
		char *error = strerror(errno);
		fprintf(stderr, "Error opening file %s\n%s\n", filePath, error);
		exit(1);
	}
	return file;
}

int file_fclose(FILE *file){
	int status = fclose(file);
	if(status == EOF) {
		fprintf(stderr, "Error closing the file\n");
		exit(1);
	}
	return status;
}

size_t file_fread(void *target, size_t recordSize, size_t recordCount, FILE *source){
	size_t recordsRead = fread(target, recordSize, recordCount, source);
	if(recordsRead < recordCount){
		fprintf(stderr, "Error while reading file\n");
		exit(1);
	}
	return recordsRead;
}

size_t file_fwrite(void *source, size_t recordSize, size_t recordCount, FILE *target){
	size_t recordsWritten = fwrite(source, recordSize, recordCount, target);
	if(recordsWritten < recordCount){
		fprintf(stderr, "Error while writing to file\n");
		exit(1);
	}
	return recordsWritten;
}

int file_fseek(FILE* file, int offset, int whence){
	int status = fseek(file, offset, whence);
	if (status == -1){
		fprintf(stderr, "Error while searching for byte in file\n");
		exit(1);
	}
	return status;
} 


