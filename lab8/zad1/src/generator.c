#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"

const char *SOURCE = "./resources/lyrics.txt";

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Invalid number of arguments. Usage: ./generator targetFileName recordCount");
        exit(EXIT_FAILURE);
    }
    srand((unsigned int) time(NULL));
    char *fileName = argv[1];
    int recordCount = atoi(argv[2]);
    FILE *target = fopen(fileName, "w");
    FILE *source = fopen(SOURCE, "r");
    fseek(source, 0, SEEK_END);
    int sourceLength = (int) ftell(source) - 1020;
    Record record;
    for (int i = 0; i < recordCount; i++) {
        int position = rand() % sourceLength;
        fseek(source, position, SEEK_SET);
        record.id = i;
        fread(&record.text, 1020, 1, source);
        fwrite(&record, sizeof(Record), 1, target);
    }
    fclose(source);
    fclose(target);
    return 0;
}