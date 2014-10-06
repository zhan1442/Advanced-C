#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"answer05.h"

void partition(int * arr, int ind, int left);
void printPartition(int * arr, int length);
void partitionO(int * arr, int ind, int left);
void partitionE(int * arr, int ind, int left);
void partitionI(int * arr, int ind, int left);
void partitionD(int * arr, int ind, int left);
void printPartitionD(int * arr, int length);

void partitionAll(int value)
{
	int * arr;
	arr = malloc(sizeof(int) * value);
	partition(arr, 0, value);
	free (arr);
}

void partitionOdd(int value)
{
	int * arr;
	arr = malloc(sizeof(int) * value);
	partitionO(arr, 0, value);
	free (arr);
}

void partitionEven(int value)
{
	int * arr;
	arr = malloc(sizeof(int) * value);
	partitionE(arr, 0, value);
	free (arr);
}

void partitionIncreasing(int value)
{
	int * arr;
	arr = malloc(sizeof(int) * value);
	partitionI(arr, 0, value);
	free (arr);
}

void partitionDecreasing(int value)
{
	int * arr;
	arr = malloc(sizeof(int) * value);
	partitionD(arr, 0, value);
	free (arr);
}

void partitionOddAndEven(int value)
{
	return;
}

void partitionPrime(int value)
{
	return;
}

//cited from Prof. Lu's PDF book
void partition(int * arr, int ind, int left)
{
	int val = 1;
	if (left == 0)
	{
		printPartition(arr, ind);
		return;
	}
	for (val = 1; val <= left; val++)
	{
		arr[ind] = val;
		partition(arr, ind + 1, left - val);
	}
}
//cited from Prof. Lu's PDF book
void printPartition(int * arr, int length)
{
	int ind;
	printf("= ");
	for (ind = 0; ind < length - 1; ind ++)
	{
		printf("%d + ", arr[ind]);
	}
	printf("%d\n", arr[length - 1]);
}

void printPartitionD(int * arr, int length)
{
	int ind;
	printf("= ");
	for (ind = length - 1; ind > 0; ind --)
	{
		printf("%d + ", arr[ind]);
	}
	printf("%d\n", arr[0]);
}

//cited from Prof. Lu's PDF book
void partitionO(int * arr, int ind, int left)
{
	int val = 1;
	if (left == 0)
	{
		printPartition(arr, ind);
		return;
	}
	for (val = 1; val <= left; val += 2)
	{
		arr[ind] = val;
		partitionO(arr, ind + 1, left - val);
	}
}

void partitionE(int * arr, int ind, int left)
{
	int val = 1;
	if (left == 0)
	{
		printPartition(arr, ind);
		return;
	}
	for (val = 2; val <= left; val += 2)
	{
		arr[ind] = val;
		partitionE(arr, ind + 1, left - val);
	}
}

void partitionI(int * arr, int ind, int left)
{
	int val;
	if (left == 0)
	{
		printPartition(arr, ind);
		return;
	}
	int min = 1;
	if (ind != 0)
	{
		min = arr[ind - 1] + 1;
	}
	for (val = min; val <= left; val++)
	{
		arr[ind] = val;
		partitionI(arr, ind + 1, left - val);
	}
}


void partitionD(int * arr, int ind, int left)
{
	int val;
	if (left == 0)
	{
		printPartitionD(arr, ind);
		return;
	}
	int min = 1;
	if (ind != 0)
	{
		min = arr[ind - 1] + 1;
	}
	for (val = min; val <= left; val++)
	{
		arr[ind] = val;
		partitionD(arr, ind + 1, left - val);
	}
}









