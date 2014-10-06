#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"answer05.h"

int main()
{
	int a;
	scanf("%d",&a);
	//printf("all\n");
	//partitionAll(a);
	//printf("\nodd\n");
	//partitionOdd(a);
	//printf("\nEven\n");
	//partitionEven(a);
	//printf("\nincreasing\n");
	partitionIncreasing(a);
	//printf("\ndecreasing\n");
	partitionDecreasing(a);
	return 0;
}
