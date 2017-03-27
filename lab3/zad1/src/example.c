#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc < 2)
		return 1;
	char *envVariable = getenv(argv[1]);
	if(envVariable == NULL){
		printf("There is no variable %s\n", argv[1]);
	}
	else{
		printf("%s=%s\n", argv[1], envVariable);
	}
}