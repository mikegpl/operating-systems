#include <stdio.h>
#include <string.h>
#include "ioBenchmark.h"

int main(int argc, char *argv[]){
	AppData	*data = AppData_new();
	parseArgs(argc, argv, data);

	void _testShuffle(){
		printf("\nDataset size - %d, records size - %d\n", data->recordCount, data->recordSize);
		shuffle(data);
	}

	void _testSort(){
		printf("\nDataset size - %d, records size - %d\n", data->recordCount, data->recordSize);
		sort(data);
	}

	switch(data->action){
		case SHUFFLE:
			measureTime(_testShuffle, strcat(argv[1], " - shuffle"));
			break;
		case SORT:
			measureTime(_testSort, strcat(argv[1], " - sort"));
			break;
		default:
			printf("Invalid action\n");
			break;

	}

	AppData_delete(data);
	return 0;
}