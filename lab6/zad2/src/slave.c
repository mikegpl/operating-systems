#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <complex.h>
#include "slave.h"

int main(int argc, char *argv[]){
    if(argc == 2 && strcmp("--help", argv[1]) == 0){
        printf("%s\n", USAGE);
        exit(0);
    }
    else if(argc != 4){
        printf("%s\n", INVALID_ARGNUM);
        exit(1);
    }
    else{
        int pointsCount = argvToInt(2, argv[2]);
        int itersCount = argvToInt(3, argv[3]);
        srand(time(NULL)); 
        printf("Points to be generated: %d \t iters: %d \t\n",pointsCount, itersCount);
        
        FILE *pipe;
        char buffer[32];
        if(access(argv[1], F_OK) != -1 || mkfifo(argv[1], 0666) == 0){
            pipe = fopen(argv[1], "w");
            for(int i = 0; i < pointsCount; i++){
                Point point = getPoint(itersCount);
                if(i % 100 == 0)
                    printf("%3.f%%\r", 100 * (double) i / (double) pointsCount);
                // exactly 31 chars + newline, 4096 mod 32 == 0 -> safe writing
                sprintf(buffer, "% 10.8lf % 10.8lf %7.d\n", point.re, point.im, point.iters);
                fprintf(pipe, "%s", buffer);                
            }
            printf("Generated all of %d points\n", pointsCount);
            fclose(pipe);
        } else{
            fprintf(stderr, "%s at '%s'\n", ERROR_PIPE, argv[1]);
            exit(1);
        }
        return 0;
    }
}

int argvToInt(int position, char *number){
    char *stopAt;
    int value = (int) strtol(number, &stopAt, 10);
    if(number == stopAt || value < 0){
        printf("%s [%d]\n", INVALID_NUMBER, position);
        exit(1);
    }
    return value;
}

Point getPoint(int itersLimit){
    Point point;
    point.re = getRandomAtoB(REAL_MIN, REAL_MAX);
    point.im = getRandomAtoB(IMAG_MIN, IMAG_MAX);
    point.iters = getIters(point.re, point.im, itersLimit);
    return point;
}

double getRandomAtoB(double a, double b){
    double result = ((double) rand() / (double) RAND_MAX) * (b - a) + a;  
    return result;
}


int getIters(double real, double im, int itersLimit){
    double complex c = real + im * I;
    double complex zn = 0.0;
    int it;
    for (it = 0; it < itersLimit && cabs(zn) < 2.0; it++){
        zn = cpow(zn, 2.0) + c;
    }
    return it;
}
