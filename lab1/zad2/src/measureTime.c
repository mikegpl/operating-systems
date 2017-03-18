#include <time.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include "measureTime.h"


ExecTime getExecTime(){
    rusage ru;
    ExecTime time;
    getrusage(RUSAGE_SELF, &ru);
    time.real = getRealTime();
    time.user = getUserTime(ru);
    time.sys = getSystemTime(ru);
    return time;
}

double getSystemTime(rusage ru){
    timeval tim;
    tim = ru.ru_stime;
    double t = (double)tim.tv_sec * 1000.0 + tim.tv_usec / 1000.0;
    return t;
}

double getUserTime(rusage ru){
    timeval tim;
    tim = ru.ru_utime;
    double t = (double)tim.tv_sec * 1000.0 + tim.tv_usec / 1000.0;
    return t;
}

double getRealTime(){
    timeval tim;
    gettimeofday(&tim,NULL);
    double t = (double)tim.tv_sec * 1000.0 + tim.tv_usec / 1000.0;
    return t;
}


ExecTime measureTime(Action action, char* message){
    ExecTime startTime = getExecTime();
    action();
    ExecTime endTime = getExecTime();

    ExecTime time;
    time.user = endTime.user - startTime.user;
    time.real = endTime.real - startTime.real;
    time.sys = endTime.sys - startTime.sys;
    if(message)
        printTime(time, message);
    return time;
}

void printTime(ExecTime time, char* message){
    printf("User: \t %f ms \t Real: %f ms \t System %f ms \t %s\n", time.user, time.real, time.sys, message);
}