#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * * argv)
{
	int ind;
	for(ind = 1;ind < argc; ind++){ 
		if(ind < argc - 1){
		printf("%s ",argv[ind]);
		}
		else{
			printf("%s",argv[ind]);
		}
	}
	printf("\n");
	return EXIT_SUCCESS;

}
