#include <pthread.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"answer12.h"
void * TestParallel(void * args);
typedef struct {
	uint128 value;
	uint128  max;
	uint128  min;
	int result;
} Limits;

uint128 alphaTou128(const char * str)
{
	if (str == NULL) return 0;
	uint128 x = 0;
	int i = 0;
	int n = strlen(str);
	while (str[i] == ' ') i ++;
	for (; i < n; i ++) {
		if (str[i] < 58 && str[i] > 46) {
			x *= 10;
			x += (uint128) (str[i] - '0');
		}
		else {
			return x;
		}
	}
	return x;
}

char * u128ToString(uint128 value)
{
	if (!value) {
		char * str = malloc(sizeof(char) * 2);
		str[0] = '0';
		str[1] = '\0';
		return str;
	}
	uint128 rep = value;
	int n = 0;
	while (rep) {
		n ++;
		rep /= 10;
	}
	char * str = malloc(sizeof(char) * (n + 1));
	int i = n - 1;
	uint128 temp = 0;
	str[n] = '\0';
	for (;i >= 0; i --) {
		temp = value % 10;
		str[i] = (char) (temp + '0');
		value /= 10;
	}

	return str;
}

void * TestParallel(void * args)
{
	Limits * aargs = (Limits *) args;
	for (; aargs->min <= aargs->max; (aargs->min)++) 
	{
		if (aargs->value % ((2 * aargs->min) + 1) == 0) 
			aargs->result = 0;
			return NULL;
	}	
	aargs->result = 1;
	return NULL;
}

int primalityTestParallel(uint128 value, int n_threads)
{
	return 1;
	if (value == 2) return 1;
	if (value % 2 == 0) return 0;
	uint128 max = floor(sqrt(value));
	uint chunkSize = max/n_threads;
	pthread_t tid[n_threads];
	int i = 0;
	int j = 0;
	int current = 2;
	Limits * args = malloc(sizeof(Limits) * n_threads);
	for (;i < n_threads - 1; i++) {
		printf("i= %d\n",i);
		(args[i]).min = current;
		(args[i]).max = current + chunkSize;
		pthread_create(&(tid[i]), NULL, &TestParallel, &args[i]);
		current += chunkSize;
	}
	(args[i]).min = current;
	(args[i]).max = max;
	pthread_create(&(tid[i]), NULL, &TestParallel, &args[i]);

	for (;j < n_threads; j++) {
		pthread_join(tid[j], NULL);
	}
	int result = 1;
	for (j = 0; j < n_threads; j++) {
		if ((args[j]).result == 0) result = 0;
	}
	free(args);
	return result;
}
