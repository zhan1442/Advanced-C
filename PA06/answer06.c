#include "answer06.h"
#include <stdio.h>

void direction(char * * maze, int row, int col, int w, int h, int d);
void print_directions(char * * maze, int w, int h) 
{
	int row = 0;
	int col = 0;
	while (col < w && maze[row][col] != ' ') col++;
	//printf("row = %d, col = %d\n",row,col);
	direction(maze, row, col, w, h, 1);
	//printf("maze[0][0] = %c\n",maze[0][0]);
}
void direction(char * * maze, int row, int col, int w, int h, int d)//d left=2 right=3 up=0 down=1
{
	int steps = 0;
	int rrow = row;
	int rcol = col;

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
