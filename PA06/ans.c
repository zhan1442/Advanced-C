#include "answer06.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void destruct(char * * maze, int w, int h);
void direction(char * * maze, int row, int col, int w, int h, int d);
void print_directions(char * * maze, int w, int h) 
{
	int row = 0;
	int col = 0;
	int i;
	while (col < w && maze[row][col] != ' ') col++;
	//printf("row = %d, col = %d\n",row,col);
	char * * rmaze = (char * *) malloc(sizeof(char *) * h);
	for(i = 0; i < h; i++){
		rmaze[i] = malloc(sizeof(char) * w);
		strcpy(rmaze[i],maze[i]);
		printf("%s\n",rmaze[i]);
	}

	direction(rmaze, row, col, w, h, 1);
	//printf("maze[0][0] = %c\n",maze[0][0]);
	destruct(rmaze, w, h);
}
void direction(char * * maze, int row, int col, int w, int h, int d)//d left=2 right=3 up=0 down=1
{
	int steps = 0;
	int rrow = row;
	int rcol = col;
	//int up = 0;
	//int down = 0;
	//int l = 0;
	//int r = 0;

	do{
		if(d != 0){
			while(row + 1 < h && maze[row + 1][col] == ' '){
				
				steps ++;
				row ++;
			}
			if(steps != 0){ 
				printf("S %d\n",steps);
				steps = 0;
				d = 1;
			}
		}
		if(d != 1){
			while(row - 1 >= 0 && maze[row - 1][col] == ' '){
				steps ++;
				row --;
			}
			if(steps != 0){
				printf("N %d\n",steps);
				steps = 0;
			} 
		}	
		if(d != 2){
			while(col + 1 < w && maze[row][col + 1] == ' '){ 
				steps ++;
				col ++; 
			}
			if(steps != 0) {
				printf("E %d\n",steps);
				steps = 0;
				d = 3;
			}
		}
		if(d != 3){
			while(col - 1 >= 0 && maze[row][col - 1] == ' '){ 
				steps ++; 
				col --;
			}
			if(steps != 0){
				printf("W %d\n",steps);
				steps = 0;
			}
		}
		d = 4;
	}while(rrow != row || rcol != col);
}

void destruct(char * * maze, int w, int h)
{
	int i;
	for (i = 0; i < h; i++){
		free(maze[i]);
	}
	free(maze);
}
