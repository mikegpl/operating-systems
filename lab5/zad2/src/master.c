#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int stringToInt(char *number);
double stringToDouble(char *number);
int **int2DArray_new(int rows, int cols);
void int2DArray_delete(int **array, int rows);
void printArray(int **array, int rows, int cols);
void processLine(char *line, int **array, int res);
int getXcoord(double position, int resolution);
int getYcoord(double position, int resolution);


static const char *INVALID_ARGNUM = "Incorrect number of arguments.\nUse ./master --help for usage";
static const char *INVALID_NUMBER = "Entered illegal number as one of arguments";
static const char *INVALID_PATH = "Entered incorrect path to pipe file";
static const char *ERROR_PIPE = "Error while opening named pipe";
static const char *ERROR_BAD_ALLOC = "Error while allocating memory for array";
static const char *ERROR_STR_DOUBLE = "Error while converting string to double";
static const char *ERROR_STR_INT = "Error while converting string to int";
static const char *USAGE = "./master pathToPipe resolution\n(string, int >= 0";

static const char *WSPACE_DELIMITERS = " \n\t";
static const double REAL_MIN = -2.0;
static const double REAL_MAX = 1.0;
static const double IMAG_MIN = -1.0;
static const double IMAG_MAX = 1.0;

static int correctlyRead = 0;

int main(int argc, char *argv[]){
	if(argc == 2 && strcmp("--help", argv[1]) == 0){
		printf("%s\n", USAGE);
		exit(0);
	}
	else if(argc < 3 || argc > 3){
		printf("%s\n", INVALID_ARGNUM);
		exit(1);
	}
	int res = stringToInt(argv[2]);
	// if(access(argv[1], F_OK) == -1){
		// fprintf(stderr, "%s\n", INVALID_PATH);
		// exit(1);
	// }		
	int **T = int2DArray_new(res, res);
	FILE *pipe;
	pipe = fopen("pipe.p", "r");
	if(pipe == NULL){
		fprintf(stderr, "%s\n", ERROR_PIPE);
		exit(1);
	}
	char line[64];
	int i =0;
	while(fgets(line, 64, pipe)){
		processLine(line, T, res);
	}
	fclose(pipe);

	for(int i = 0; i < res; i++){
		for(int j = 0; j < res; j++)
			printf("%d %d %d\n", i, j, T[i][j]);
	}

	unlink(argv[1]);
	int2DArray_delete(T, res);
	fprintf(stderr, "Correctly read %d\n", correctlyRead);
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
	char testo[256];
	strcpy(testo, line);
	if(testo == NULL)
		return;
	char *cols[3];
	cols[0] = strtok(testo, WSPACE_DELIMITERS);
	if(cols[0] == NULL){
		return;
		printf("First col was null\n");
		printf("%s\n", line);
	}
	cols[1] = strtok(NULL, WSPACE_DELIMITERS);
	if(cols[1] == NULL){
		return;
		printf("Second col was null\n");
		printf("%s\n", line);
	}
	cols[2] = strtok(NULL, WSPACE_DELIMITERS);
	if(cols[2] == NULL){
		return;
		printf("Last col ws null\n");
		printf("%s\n", line);
	}

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