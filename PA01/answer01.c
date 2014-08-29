#include <stdlib.h>
#include "answer01.h"

int arraySum(int * array, int len)
{
	int sum = 0; //the sum of the array elements
	int i; //the index number 

	for (i = 0; i < len; i++)
	{
		sum = sum + array[i];
	}

	return sum;
}

int arrayCountNegative(int * array, int len)
{
	int i; //index number
	int count = 0; //count of negtive numbers

	for (i = 0; i < len; i++)
	{
		if (array[i] < 0)
		{
			count++;
		}
	}

	return count;
}

int arrayIsIncreasing(int * array, int len)
{   
	int isInc = 1; //if the array elements are increasing
	int i = 1; //index number

	while (i < len && isInc == 1)
	{
		if (array[i - 1] > array[i])
		{
			isInc = 0;
		}

		i++;
	}    

	return isInc;
}

int arrayIndexRFind(int needle, const int * haystack, int len)
{
	int i; //index number
	int index = -1; //the idex of needle

	for (i = 0; i < len; i++)
	{
		if (haystack[i] == needle)
		{
			index = i;  
		}
	}

	return index;
}

int arrayFindSmallest(int * array, int len)
{
	int i; //index number
	int index = 0; //index of the smallest element

	for (i = 0; i < len; i++)
	{
		if (array[i] < array[index])
		{
			index = i;
		}
	}

	return index;
}
