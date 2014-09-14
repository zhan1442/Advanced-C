#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"answer03.h"

int main(int argc, char * * argv)
{
	char * p = NULL;
	char * a = malloc(sizeof(char)*6);
	a = "hello ";
	char * b = "world!";
	int x = 6;
	int y = 0;
	int z = 13;
	printf("%s\n",strcat_ex(&a,&x, b));
	printf("%s\n",strcat_ex(&p,&y,b));
	printf("%s\n",strcat_ex(&a,&z, b));

	return 0;
		
}
