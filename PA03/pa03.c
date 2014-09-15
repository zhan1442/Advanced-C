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
	free(p);
	free(a);
	free(c);

	const char * d = "abcaf\ndefsdfv\rehisdfbsdf fklsdf hweicfow cmn\nbfvdfv";
	const char * dim = "\n\f\r ";
	char * * po;
	int len;
	po = explode(d, dim, &len);
	int i;
	for (i = 0; i < len; i++){
		printf("po[%d] = %s\n",i,po[i]);
	}
    sortStringArray(po, len);
	char * pa;
	char * glue = ", ";
	pa = implode(po,len,glue);
	printf("pa = %s\n",pa);

	sortStringCharacters(pa);
	printf("pa = %s\n",pa);

	free(pa);

	destroyStringArray(po, len);
		
	return 0; 

		
}
