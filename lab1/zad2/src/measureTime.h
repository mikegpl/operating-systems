// measureTime.h
#ifndef MTIME_H
#define MTIME_H

typedef struct timespec timespec;
typedef struct timeval timeval;
typedef struct rusage rusage;
typedef struct ExecTime{
    double user;
    double real;
    double sys;
}ExecTime;
typedef void (*Action)(void);

ExecTime getExecTime();
double getSystemTime(rusage);
double getUserTime(rusage);
double getRealTime();
ExecTime measureTime(Action, char*);
void printTime(ExecTime, char*);

#endif