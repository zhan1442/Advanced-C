#include <stdlib.h>
#include "answer03.h"
#include<string.h>
#include<stdio.h>

char * strcat_ex(char * * dest, int * n, const char * src)
{
	int src_len = strlen(src); 
	if( (* dest == NULL) || (src_len + strlen(*dest) + 1 > *n) ) {
		char * p;
		if (* dest == NULL) {
			p = (char *) malloc(sizeof(char) * (1 + 2 * strlen(src)));
			* n = 1 + 2 * strlen(src);
		}
		else {
			p = (char *) malloc(sizeof(char) * (1 + 2 * (strlen(*dest) + strlen(src))));
			* n = 1 + 2 * (strlen(*dest) + strlen(src));
			strcpy(p, * dest);
		}
		free(* dest);
		* dest = p;
		strcat(* dest, src);
		return * dest;  
	}
	else {
		* dest = strcat(* dest, src);
		return * dest;
	}
}

char * * explode(const char * str, const char * delims, int * arrLen)
{

	char * * strarr = (char * *) malocc(sizeof(char *) * ());


}
