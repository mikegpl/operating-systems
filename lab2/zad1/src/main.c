#include <stdio.h>
#include "ioBenchmark.h"

int main(int argc, char *argv[]){
	AppData *data = AppData_new();
	parseArgs(argc, argv, data);
	switch(data->action){
		case GENERATE:
			generate(data);
			break;
		case SHUFFLE:
			shuffle(data);
			break;
		case SORT:
			sort(data);
			break;
		default:
			printf("Error: invalid operation\n");
			exit(1);
	}

	AppData_delete(data);
}
