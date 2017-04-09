#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "master.h"

static int correctlyRead = 0;
static const char *gOpts[4] = {"set view map", 
								  "set xrange", 
								  "set yrange", 
								  "plot 'data' with image"};

int main(int argc, char *argv[]){
	// Parse args
	if(argc == 2 && strcmp("--help", argv[1]) == 0){
		printf("%s\n", USAGE);
		exit(0);
	}
	else if(argc != 3){
		printf("%s\n", INVALID_ARGNUM);
		exit(1);
	}
	int res = stringToInt(argv[2]);
	if(access(argv[1], F_OK) == -1){
		fprintf(stderr, "%s\n", INVALID_PATH);
		exit(1);
	}		


	int **T = int2DArray_new(res, res);
	
	FILE *pipe;
	pipe = fopen(argv[1], "r");
	if(pipe == NULL){
		fprintf(stderr, "%s\n", ERROR_PIPE);
		exit(1);
	}

	char line[64];
	while(fgets(line, 64, pipe)){
		processLine(line, T, res);
	}
	fclose(pipe);

	fprintf(stderr, "Correctly read %d\n", correctlyRead);

	FILE *data;
	data = fopen(TARGET_FILE, "a");
	if(data == NULL){
		fprintf(stderr, "%s\n", ERROR_TARGET);
		exit(1);
	}

	for(int i = 0; i < res; i++){
		for(int j = 0; j < res; j++){
			fprintf(data, "%d %d %d\n", i, j, T[i][j]);
		}
	}
	fclose(data);

	FILE *gplotPipe = popen("gnuplot", "w");
	if(gplotPipe == NULL){
		fprintf(stderr, "%s\n", ERROR_POPEN);
		exit(1);
	}
	fprintf(gplotPipe, "%s\n%s[0:%d]\n%s[0:%d]\n%s\n", gOpts[0], gOpts[1], res, gOpts[2], res, gOpts[3]);
	
	fflush(gplotPipe);
	printf("Press [ENTER] to exit\n");
	getchar();

	pclose(gplotPipe);	
	remove(TARGET_FILE);
	unlink(argv[1]);
	int2DArray_delete(T, res);
	return 0;

	
}

int stringToInt(char *number){
	assert(number != NULL);
	char *stopAt;
	int value = (int) strtol(number, &stopAt, 10);
	if(number == stopAt || value < 0){
		printf("%s\n", ERROR_STR_INT);
		exit(1);
	}
	return value;
}

double stringToDouble(char *number){
	assert(number != NULL);
	double result;
	int read = sscanf(number, "%lf", &result);
	if(read == 0){
		fprintf(stderr, "%s\n", ERROR_STR_DOUBLE);
		exit(1);
	}
	return result;
}

int **int2DArray_new(int rows, int cols){
	assert(rows > 0 && cols > 0);
	int **array = (int **) calloc(rows, sizeof(*array));
	if(array == NULL){
		fprintf(stderr, "%s\n", ERROR_BAD_ALLOC);
		exit(1);
	}
	for(int i = 0; i < rows; i++){
		array[i] = (int *) calloc(cols, sizeof(*array[i]));
		if(array[i] == NULL){
			fprintf(stderr, "%s\n", ERROR_BAD_ALLOC);
			exit(1);
		}
	}
	return array;
}

void int2DArray_delete(int **array, int rows){
	assert(array != NULL);
	for(int i = 0; i < rows; i++)
		free(array[i]);
	free(array);
}

void printArray(int **array, int rows, int cols){
	assert(array != NULL);
	for(int i = 0; i < rows; i++){
		printf("%d[i]: ", i);
		for(int j = 0; j < cols; j++)
			printf("%d ", array[i][j]);
		printf("\n");
	}
}

void processLine(char *line, int **array, int res){
	assert(line != NULL);
	assert(array != NULL);
	char *cols[3];
	cols[0] = strtok(line, WSPACE_DELIMITERS);
	cols[1] = strtok(NULL, WSPACE_DELIMITERS);
	cols[2] = strtok(NULL, WSPACE_DELIMITERS);

	double real = stringToDouble(cols[0]);
	double imag = stringToDouble(cols[1]);
	int iters = stringToInt(cols[2]);

	int x = getXcoord(real, res);
	int y = getYcoord(imag, res);
	array[x][y] = iters;
	correctlyRead++;
}

int getXcoord(double position, int resolution){
	double rangeX = REAL_MAX - REAL_MIN;
	position -= REAL_MIN;
	int xCoord = (int) ((position / rangeX) * resolution);
	return xCoord;
}

int getYcoord(double position, int resolution){
	double rangeY = IMAG_MAX - IMAG_MIN;
	position -= IMAG_MIN;
	int yCoord = (int) ((position / rangeY) * resolution);
	return yCoord; 
}