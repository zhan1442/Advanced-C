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
	int N = 0;
	int i;
	int arrind = 0;
	int last = 0;

	for (i = 0; i < strlen(str); i++) {
		if (strchr(delims, str[i]) != NULL) {
			N++;
		}
	}		
	char * * strarr = (char * *) malloc(sizeof(char *) * (N + 1));
	* arrLen = N + 1;
	
	for (i = 0; i < (int) strlen(str); i++) {
		if (strchr(delims, str[i]) != NULL) {
			strarr[arrind] = (char *) malloc(sizeof(char)*(i - last + 1));
			memcpy(strarr[arrind], str + last, i - last);
			last = i + 1;
			arrind++;
		}
	}

	strarr[arrind] = (char *) malloc(sizeof(char)*(i - last + 1));
	memcpy(strarr[arrind], str + last, i - last);

	return(strarr);

}

char * implode(char * * strArr, int len, const char * glue)
{
	int i;
	char * str = malloc(sizeof(char) * 1);
	int x = 1;
	for (i = 0; i < len; i++) {
		strcat_ex(&str, &x, strArr[i]);
		if (i < len - 1)
		strcat_ex(&str, &x, glue);
	}
	return str;
}

int cstring_cmp(const void *a, const void *b);
void sortStringArray(char * * arrString, int len)
{
	qsort(arrString, len, sizeof(char *),cstring_cmp);

}
int cstring_cmp(const void *a, const void *b)
{
	const char **ia = (const char **)a;
	const char **ib = (const char **)b;
	return strcmp(*ia, *ib);
}

int cmpstr(void const *a, void const *b);
void sortStringCharacters(char * str)
{
	qsort(str, strlen(str), sizeof(char), cmpstr);	

}
int cmpstr(void const *a, void const *b) { 
	char const *aa = (char const *)a;
	char const *bb = (char const *)b;
	return strcmp(aa, bb);
}

void destroyStringArray(char * * strArr, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		free(strArr[i]);
	}
	free(strArr);
}











