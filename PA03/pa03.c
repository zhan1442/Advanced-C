#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"answer03.h"

int main(int argc, char * * argv)
{
	char * p = (char *) malloc(sizeof(char)*4);
	strcpy(p, "zzz");
	char * a = (char *) malloc(sizeof(char)*7);
	strcpy(a, "hello ");
	char * c = (char *) malloc(sizeof(char)*3);
	strcpy(c, "");
	char * b = "world!";

	int x = 7;
	int y = 4;
	int z = 3;
	printf("%s\n",strcat_ex(&a,&x, b));
	printf("%s\n",strcat_ex(&p,&y,b));
	printf("%s\n",strcat_ex(&a,&x, b));
	printf("%s\n",strcat_ex(&a,&x, b));
	printf("%s\n",strcat_ex(&a,&x, b));
	printf("%s\n",strcat_ex(&a,&x, b));
	printf("%s\n",strcat_ex(&a,&x, b));
	printf("%s\n",strcat_ex(&a,&x, b));
	printf("%s\n",strcat_ex(&a,&x, b));
	printf("%s\n",strcat_ex(&a,&x, b));
	printf("%s\n",strcat_ex(&c,&z, b));
	return 0;
		
}
