#include <stdlib.h>
#include "answer03.h"
#include<string.h>

char * strcat_ex(char * * dest, int * n, const char * src)
{
	int src_len = strlen(src); 
    int dst_len = strlen(*dest);
	if( (*dest == NULL) || (src_len + dst_len + 1 > *n) ) {
		char * p = (char *) malloc(sizeof(char) * (1 + 2 * (strlen(*dest) + strlen(src))));
		* n = 1 + 2 * (strlen(*dest) + strlen(src));
		strcpy(p, *dest);
		//free(*dest);
		*dest = p;
		strcat(*dest, src);
		return *dest;  
	}
	else {
		*dest = strcat(*dest, src);
		return *dest;
	}
}
