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
	if(showhelp) {
		printf("Usage: grep-lite [OPTION]... PATTERN\nSearch for PATTERN in standard input. PATTERN is a\nstring. grep-lite will search standard input line by\nline, and (by default) print out those lines which\ncontain pattern as a substring.\n\n  -v, --invert-match     print non-matching lines\n  -n, --line-number      print line numbers with output\n  -q, --quiet            suppress all output\n\nExit status is 0 if any line is selected, 1 otherwise;\nif any error occurs, then the exit status is 2.\n");
	return EXIT_SUCCESS;
	}
	int pnmatch = 0;
	int ln = 0;
	int quiet = 0;
	for(ind = 1; ind < argc - 1; ind++){
		if(strcmp(argv[ind],"-v") == 0){
			pnmatch = 1;
		}
		if(strcmp(argv[ind],"-n") == 0){
			ln = 1;
		}
		if(strcmp(argv[ind],"-q") == 0){
			quiet = 1;
		}
		if(strcmp(argv[ind],"-v") != 0 && strcmp(argv[ind],"-n") != 0 && strcmp(argv[ind],"-q") != 0){
			fprintf(stderr,"ERROR:bogus command-line arguments are passed\n");
			return 2;
		}
	}
	if(argv[argc - 1][0] == '-'){
		fprintf(stderr,"ERROR:PATTERN can never begin with '-'\n");
		return 2;
	}
	FILE * file;
	file = stdin;
	char * str = (char *) malloc(2000 * sizeof(char));
	ind = 0;
	if(file != NULL){
		while(!feof(file)){
			fgets(str,2000,file);
			if(!feof(file)){
				ind++;
				if(pnmatch){
					if(strstr(str,argv[argc - 1]) == NULL){
						if(ln && !quiet){
							printf("%d:",ind);
						}
						if(!quiet) fprintf(stdout,"%s",str);
					}
				}
				else {
					if(strstr(str,argv[argc - 1]) != NULL){
						if(ln && !quiet){
							printf("%d:",ind);
						}
						if(!quiet) fprintf(stdout,"%s",str);
					}
				}
			}
		}
	}
	free(str);
	return EXIT_SUCCESS;
}
