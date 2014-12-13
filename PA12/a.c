#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer12.h"

int main (int argc,char * * argv) 
{
	 uint128 x = alphaTou128(argv[1]);
	 char * str = u128ToString(x);
	 printf("%s\n",str);
	 int p = primalityTestParallel(x, 8);
	 printf("%d\n",p);
	 free(str);
	
	return 0;
}

