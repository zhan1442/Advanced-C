#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * * argv) 
{
	int ind = 1;
	int showhelp = 0;
	for(; ind < argc; ind++){
		if(strcmp(argv[ind],"--help") == 0){
			showhelp = 1;
		}
	}
	if(showhelp){
		printf("Usage: cat-lite [--help] [FILE]...\nWith no FILE, or when FILE is -, read standard input.\n\n");
		printf("Examples:\ncat-lite README   Print the file README to standard output.\ncat-lite f - g    Print f's contents, then standard input,\n                  then g's contents.\ncat-lite          Copy standard input to standard output.\n");
		return EXIT_SUCCESS;
	}	
	
	FILE * file = NULL;
	char str;
	if(argc == 1){
		file = stdin;
		if (file != NULL){
			while(!feof(file)){
				fscanf(file,"%c",&str);
				if(!feof(file)){
					printf("%c",str);
				}
			}
		}
	}
	for(ind = 1; ind < argc; ind++){
		if(strcmp(argv[ind],"-") == 0){
			file = stdin;
			if (file != NULL){
				while(!feof(file)){
					fscanf(file,"%c",&str);
					if(!feof(file)){
						printf("%c",str);
					}
				}
			}
		}
		else {
			file = fopen(argv[ind],"r");
			if (file != NULL){
				while(!feof(file)){
					fscanf(file,"%c",&str);
					if(!feof(file)){
						printf("%c",str);
					}
				}
				fclose(file);
			}
			else{
				fprintf(stderr,"cat cannot open %s\n",argv[ind]);
				return EXIT_FAILURE;
			}
		}
	}

	return EXIT_SUCCESS;
}
