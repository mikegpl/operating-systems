#ifndef MASTER_H
#define MASTER_H

int stringToInt(char *number);
double stringToDouble(char *number);
int **int2DArray_new(int rows, int cols);
void int2DArray_delete(int **array, int rows);
void printArray(int **array, int rows, int cols);
void processLine(char *line, int **array, int res);
int getXcoord(double position, int resolution);
int getYcoord(double position, int resolution);
void arrayToFile(int **array, int m, int n, const char *fileName);
void readFromPipe(int **array, char *fileName, int res);
void drawGnuplot(int res);

static const char *INVALID_ARGNUM = "Incorrect number of arguments.\nUse ./master --help for usage";
static const char *INVALID_PATH = "Entered incorrect path to pipe file";
static const char *ERROR_PIPE = "Error while opening named pipe";
static const char *ERROR_TARGET = "Error while opening 'data' file";
static const char *ERROR_BAD_ALLOC = "Error while allocating memory for array";
static const char *ERROR_STR_DOUBLE = "Error while converting string to double";
static const char *ERROR_STR_INT = "Error while converting string to int";
static const char *ERROR_POPEN = "Error while opening unnamed pipe to gnuplot";
static const char *USAGE = "./master pathToPipe resolution\n(string, int >= 0";

static const char *TARGET_FILE = "data";
static const char *WSPACE_DELIMITERS = " \n\t";
static const double REAL_MIN = -2.0;
static const double REAL_MAX = 1.0;
static const double IMAG_MIN = -1.0;
static const double IMAG_MAX = 1.0;

#endif