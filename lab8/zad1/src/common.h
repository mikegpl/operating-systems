#ifndef ZAD1_COMMON_H
#define ZAD1_COMMON_H

#define RECORD_STR_LEN 1020

typedef struct Record {
    int id;
    char text[RECORD_STR_LEN];
} Record;

#endif //ZAD1_COMMON_H
