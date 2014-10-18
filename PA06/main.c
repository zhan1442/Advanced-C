#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"answer06.h"

int main(){
	char * maze[5] = {"xxxxx xxxxxxxxx",
					  "x       x    xx",
					  "x xxx xxxx xxxx",
					  "    x         x",
					  "x xxx xxxx xxxx"}; 

	int i;
	for(i = 0; i < 5;i++){
		printf("%s\n",maze[i]);
	}
	print_directions(maze, 15, 5);

	return 0;
}
