#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"answer06.h"

int main(){
	char * maze[4] = {"x xxxxxxxx",
					  "x    x    ",
					  "xxxx x xx ",
					  "xxxx   xx "}; 

	int i;
	for(i = 0; i < 4;i++){
		printf("%s\n",maze[i]);
	}
	print_directions(maze, 10, 4);

	return 0;
}
