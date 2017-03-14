#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <libcontact.h>

typedef struct{
    struct timeval ru_utime; /* user time used */
    struct timeval ru_stime; /* system time used */
    long   ru_maxrss;        /* maximum resident set size */
    long   ru_ixrss;         /* integral shared memory size */
    long   ru_idrss;         /* integral unshared data size */
    long   ru_isrss;         /* integral unshared stack size */
    long   ru_minflt;        /* page reclaims */
    long   ru_majflt;        /* page faults */
    long   ru_nswap;         /* swaps */
    long   ru_inblock;       /* block input operations */
    long   ru_oublock;       /* block output operations */
    long   ru_msgsnd;        /* messages sent */
    long   ru_msgrcv;        /* messages received */
    long   ru_nsignals;      /* signals received */
    long   ru_nvcsw;         /* voluntary context switches */
    long   ru_nivcsw;        /* involuntary context switches */
}rusage;


/*************************************************************/


double getSystemTime(){
    struct timeval tim;
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    tim = ru.ru_stime;
    double t = (double)tim.tv_sec * 1000.0 + tim.tv_usec / 1000.0;
    return t;
}

double getUserTime(){
    struct timeval tim;
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    tim = ru.ru_utime;
    double t = (double)tim.tv_sec * 1000.0 + tim.tv_usec / 1000.0;
    return t;
}

double getRealTime(){
    struct timeval tim;
    gettimeofday(&tim,NULL);
    double t = (double)tim.tv_sec * 1000.0 + tim.tv_usec / 1000.0;
    return t;
}


void measure_time(void(*activity)(void)){
    double userStart = getUserTime();
    double realStart = getRealTime();
    double systemStart = getSystemTime();

    activity();

    double userEnd = getUserTime();
    double realEnd = getRealTime();
    double systemEnd = getSystemTime();

    printf("User time: \t %f\n", (userEnd - userStart));
    printf("Real time: \t %f\n", (realEnd - realStart));
    printf("System time: \t %f\n", (systemEnd - systemStart));
}

const int contact_number = 1000;
const int read_buffer_size = 256;
const char *read_delimiters = ",\n";

static void parseContact(char *raw, Contact *parsed) {
    strcpy(parsed->name, strtok(raw, read_delimiters));
    strcpy(parsed->surname, strtok(NULL, read_delimiters));
    strcpy(parsed->birthDate, strtok(NULL, read_delimiters));
    strcpy(parsed->email, strtok(NULL, read_delimiters));
    strcpy(parsed->phoneNumber, strtok(NULL, read_delimiters));
    strcpy(parsed->address, strtok(NULL, read_delimiters));
}

static void loadContacts(Contact **contacts) {
    char *buffer = malloc(read_buffer_size);
    FILE *data = fopen("data.csv", "r");

    for (int i = 0; i < contact_number; ++i) {
        fgets(buffer, read_buffer_size, data);
        contacts[i] = malloc(sizeof(Contact));
        parseContact(buffer, contacts[i]);
    }

    fclose(data);
    free(buffer);
}

static void deleteContacts(Contact **contacts) {
    for (int i = 0; i < contact_number; ++i) {
        free(contacts[i]);
    }
    free(contacts);
}


int isPrime(int n){
    if(n < 2)
        return 0;
    if(n%2 == 0)
        return 1;
    for(int i = 3; i*i <= n; i += 2){
        if(n%i == 0)
            return 0;
    }
    return 1;
}

int main(void) {
    Contact **contacts = calloc(contact_number, sizeof(Contact *));
    
    void load(void){
        loadContacts(contacts);
    }
    measure_time(load);
    deleteContacts(contacts);
    return 0;
}