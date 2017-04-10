#ifndef SLAVE_H
#define SLAVE_H

typedef struct Point Point;
struct Point
{
    double re;
    double im;
    int iters;
};

int argvToInt(int position, char *number);
Point getPoint(int itersLimit);
double getRandomAtoB(int a, int b);
int getIters(double real, double im, int itersLimit);

static const char *INVALID_ARGNUM = "Incorrect number of arguments.\nUse ./slave --help for usage";
static const char *INVALID_NUMBER = "Entered invalid number as one of arguments at position";
static const char *ERROR_PIPE = "Error while opening named pipe";
static const char *USAGE = "./slave pathToPipe numberOfPoints itersCount\n(string, int >= 0, int >=0)";

static const size_t writeBufferSize = 256;
static const double REAL_MIN = -2.0;
static const double REAL_MAX = 1.0;
static const double IMAG_MIN = -1.0;
static const double IMAG_MAX = 1.0;

#endif