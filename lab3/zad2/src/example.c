#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int *x = malloc(4000000 * sizeof(int));
	for(long i = 0;1; i++){
		for(long j = 0; 1; j++){
			x[i%15] = i % 15;
		}
	}
	free(x);
}