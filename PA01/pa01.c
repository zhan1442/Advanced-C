 
#include <stdio.h>
#include <stdlib.h>

// This includes the /definitions/ of the functions that you write
// in this assignment. Including the definitions allows you to use
// the functions in this file.
#include "answer01.h"
#include "answer01.c"

void printArray(int * array, int len)
{
    printf("{");
    int ind;
    for(ind = 0; ind < len; ++ind) {
	printf("%d", array[ind]);
	if(ind != len - 1) {
	    printf(", ");
	}
    }
    printf("}");
    // If we don't include a '\n' character, then we need to 
    // include this line to ensure that our output is immediately
    // printed out output.
    fflush(stdout); 
}

void test_arraySum(int * array, int len, int expected)
{
    printArray(array, len);
    int sum = arraySum(array, len);
    printf(". sum = %d, expected = %d.", sum, expected);
    if(sum != expected)
	printf(" FAIL");
    printf("\n");
}

void test_00_arraySum()
{
    printf("Testing arraySum(...)\n");

    // Here we use "static initialization" to create an array
    int array1[] = { -4, -1, 0, 1, 5, 10, 20, 21 };
    int len1 = 8;
    int expected1 = 52;
    test_arraySum(array1, len1, expected1);

    // Our functions must always work... even on empty arrays
    int array2[] = {};
    int len2 = 0;
    int expected2 = 0;
    test_arraySum(array2, len2, expected2);

    // You can add more test-cases here
    // ...

    printf("\n"); // Tidy output is easier to use
}

void test_arrayCountNegative(int * array, int len, int expected)
{
    printArray(array, len);
    int count = arrayCountNegative(array,len); 
    printf(". count = %d, expected = %d.", count, expected);
    if(count != expected)
	printf(" FAIL");
    printf("\n");
}

void test_01_arrayCountNegative()
{
    printf("Testing arrayCountNegative(...)\n");

    int array1[] = { 1, 4, -5, 4 };
    int len1 = 4;
    test_arrayCountNegative(array1, len1, 1);

    int array2[] = {};
    int len2 = 0;
    test_arrayCountNegative(array2, len2, 0);

    int array3[] = { 0, 1, 2, 3, 4 };
    int len3 = 5;
    test_arrayCountNegative(array3, len3, 0);

    int array4[] = { -1, -2, -3, -4 };
    int len4 = 4;
    test_arrayCountNegative(array4, len4, 4);

    // You can add more test-cases here
    // ...

    printf("\n");
}

int main(int argc, char * * argv)
{
    int isInc = 1;
    int array1[4] = { 1, 4, -5, 4 };
    int array2[0] = {};
    int array3[5] = { 1, 1, 2, 3, 3 };
    int array4[4] = { 1, 2, 0, 4 };
    int haystack[] = { 1, 4, -5, 4 };
    int array[] = { 3, 4, -5 };

    printf("Welcome to ECE264, we are working on PA01.\n\n");

    // Uncomment to run two example (incomplete) testcases.
    test_00_arraySum();
    test_01_arrayCountNegative();
    
    isInc = arrayIsIncreasing(array1, 4);
    printf("[1, 4, -5, 4], isInc = %d\n",isInc);
    isInc = arrayIsIncreasing(array2, 0);
    printf("[], isInc = %d\n",isInc);
    isInc = arrayIsIncreasing(array3, 5);
    printf("[1, 1, 2, 3, 3], isInc = %d\n",isInc);
    isInc = arrayIsIncreasing(array4, 4);
    printf("[1, 2, 0, 4], isInc = %d\n",isInc);
    isInc = arrayIsIncreasing(NULL, 0);
    printf("NULL, isInc = %d\n\n",isInc); 

    int x1 = arrayIndexRFind(1, haystack, 4);
    printf("x1 = %d, expected = 0\n",x1);
    int x2 = arrayIndexRFind(4, haystack, 4);
    printf("x2 = %d, expected = 3\n",x2);
    int x3 = arrayIndexRFind(6, haystack, 4);
    printf("x3 = %d, expected = -1\n\n",x3);

    int ind1 = arrayFindSmallest(array1, 4);
    printf("smallest = %d, expected = 2\n",ind1);
    int ind2 = arrayFindSmallest(array3, 5);
    printf("smallest = %d, expected = 0\n",ind2);
    int ind3 = arrayFindSmallest(array, 3);
    printf("smallest = %d, expected = 2\n",ind3);
    int ind4 = arrayFindSmallest(array2, 0);
    printf("smallest = %d, expected = 0\n\n",ind4);

    return EXIT_SUCCESS;
}


