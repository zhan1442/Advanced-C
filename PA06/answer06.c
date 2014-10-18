#include "answer06.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void check_ns(char * * maze, int row, int col, int w, int h, int d);
void check_we(char * * maze, int row, int col, int w, int h, int d);
void destruct(char * * maze, int w, int h);
void direction(char * * maze, int row, int col, int w, int h, int d);
void print_directions(char * * maze, int w, int h) 
{
	int row = 0;
	int col = 0;
	//int i;
	while (col < w && maze[row][col] != ' ') col++;
	//printf("row = %d, col = %d\n",row,col);
	//char * * rmaze = (char * *) malloc(sizeof(char *) * (h + 1));
	//for(i = 0; i < h; i++){
	//	rmaze[i] = (char *) malloc(sizeof(char) * (w + 1));
	//	rmaze[i][0] = '\0';
	//	strcpy(rmaze[i],maze[i]);
	//}

	direction(maze, row, col, w, h, 1);
	//for(i = 0; i < h; i++){
	//	printf("%s\n",rmaze[i]);
	//}
	//destruct(rmaze, w, h);
}
void direction(char * * maze, int row, int col, int w, int h, int d)//d left=2 right=3 up=0 down=1
{
	int steps = 0;
	int rrow = row;
	int rcol = col;
	//int v = 1;

	do{
		if(row + 1 < h && maze[row + 1][col] == ' '){
			if(d != 0){
				while(row + 1 < h && maze[row + 1][col] == ' '){
					steps ++;
					row ++;
					if((col + 1 < w && maze[row][col + 1] == ' ')&&(col - 1 >= 0 && maze[row][col - 1] == ' ')) {
						if(rrow != row || rcol != col){
							printf("S %d\n",steps);
							d = 1;
							check_we(maze, row,  col, w, h, d);
							steps = 0;
						}
					}
					else if(((col + 1 < w && maze[row][col + 1] == ' ')||(col - 1 >= 0 && maze[row][col - 1] == ' '))&&(row + 1 < h && maze[row + 1][col] == ' ')){
						if(rrow != row || rcol != col){
							printf("S %d\n",steps);
							d = 1;
							check_we(maze, row,  col, w, h, d);
							steps = 0;
						}
					}
					if(!(rrow != row || rcol != col)) break;
				}
				if(steps != 0){ 
					printf("S %d\n",steps);
					steps = 0;
					//v = 0;
					d = 1;
				}
			}
		}
		if(row - 1 >= 0 && maze[row - 1][col] == ' '){
			if(d != 1){
				while(row - 1 >= 0 && maze[row - 1][col] == ' '){
					steps ++;
					row --;
					if((col + 1 < w && maze[row][col + 1] == ' ')&&(col - 1 >= 0 && maze[row][col - 1] == ' ')) {
						if(rrow != row || rcol != col){
							printf("N %d\n",steps);
							d = 0;
							check_we(maze, row,  col, w, h, d); 
							steps = 0;
						}
					}
					else if(((col + 1 < w && maze[row][col + 1] == ' ')||(col - 1 >= 0 && maze[row][col - 1] == ' '))&&(row - 1 >= 0 && maze[row - 1][col] == ' ')) {
						if(rrow != row || rcol != col){
							printf("N %d\n",steps);
							d = 0;
							check_we(maze, row,  col, w, h, d);
							steps = 0;
						}
					}
					if(!(rrow != row || rcol != col)) break;
				}
				if(steps != 0){
					printf("N %d\n",steps);
					steps = 0;
					//v = 0;
				} 
			}
		}
		if(col + 1 < w && maze[row][col + 1] == ' '){
			if(d != 2){
				while(col + 1 < w && maze[row][col + 1] == ' '){ 
					steps ++;
					col ++; 
					if((row + 1 < h && maze[row + 1][col] == ' ')&&(row - 1 >= 0 && maze[row - 1][col] == ' ')) {
						if(rrow != row || rcol != col){
							printf("E %d\n",steps);
							d = 3;
							check_ns(maze, row,  col, w, h, d);
							steps = 0;
						}
					}
					else if(((row + 1 < h && maze[row + 1][col] == ' ')||(row - 1 >= 0 && maze[row - 1][col] == ' '))&&(col + 1 < w && maze[row][col + 1] == ' ')) {
						if(rrow != row || rcol != col){
							printf("E %d\n",steps);
							d = 3;
							check_ns(maze, row,  col, w, h, d);
							steps = 0;
						}
					}
					if(!(rrow != row || rcol != col)) break;
				}
				if(steps != 0) {
					printf("E %d\n",steps);
					steps = 0;
					d = 3;
					//v = 0;
				}
			}
		}
		if(col - 1 >= 0 && maze[row][col - 1] == ' '){
			if(d != 3){
				while(col - 1 >= 0 && maze[row][col - 1] == ' '){ 
					steps ++; 
					col --;
					if((row + 1 < h && maze[row + 1][col] == ' ')&&(row - 1 >= 0 && maze[row - 1][col] == ' ')) {
						if(rrow != row || rcol != col){
							printf("W %d\n",steps);
							d = 2;
							check_ns(maze, row,  col, w, h, d);
							steps = 0;
						}
					}
					else if(((row + 1 < h && maze[row + 1][col] == ' ')||(row - 1 >= 0 && maze[row - 1][col] == ' '))&&(col - 1 >= 0 && maze[row][col - 1] == ' ')) {
						if(rrow != row || rcol != col){
							printf("W %d\n",steps);
							d = 2;
							check_ns(maze, row,  col, w, h, d);
							steps = 0;
						}
					}
					if(!(rrow != row || rcol != col)) break;
				}
				if(steps != 0){
					printf("W %d\n",steps);
					steps = 0;
				}
			}
		}
		d = 4;
	}while(rrow != row || rcol != col);
}
void check_we(char * * maze, int row, int col, int w, int h, int d)
{
	int steps = 0;
	//int rrow = row;
	//int rcol = col;
	if(col + 1 < w && maze[row][col + 1] == ' '){
		steps ++;
		col ++;
		//if((row + 1 < h && maze[row + 1][col] == ' ')&&(row - 1 >= 0 && maze[row - 1][col] == ' ')) {
			//if(rrow != row || rcol != col){
		//		printf("W %d\n",steps);
		//		d = 2;
		//		check_ns(maze, row,  col, w, h, d); 
		//		steps = 0;
			//}   
		//}   
		if(((row + 1 < h && maze[row + 1][col] == ' ')||(row - 1 >= 0 && maze[row - 1][col] == ' '))&&(col - 1 >= 0 && maze[row][col - 1] == ' ')) {
			//if(rrow != row || rcol != col){
		//		printf("W %d\n",steps);
		//		d = 2;
				//check_ns(maze, row,  col, w, h, d); 
		//		steps = 0;
			//}   
		}
		//printf("E %d\n",steps);
		//check_ns(maze, row,  col, w, h, d);
	}
	if(steps != 0){
		printf("E %d\n",steps);
		direction(maze, row, col, w, h, 3);
		col -= steps;
		printf("W %d\n",steps);
		maze[row][col + 1] = 'X';
		//int i;
		//for(i = 0;i<h;i++){
		//	printf("%s\n",maze[i]);
		//}
		steps = 0;
	}
	if(col - 1 >= 0 && maze[row][col - 1] == ' '){
		steps ++;
		col --;
		//if((row + 1 < h && maze[row + 1][col] == ' ')&&(row - 1 >= 0 && maze[row - 1][col] == ' ')) {
			//if(rrow != row || rcol != col){
		//		printf("W %d\n",steps);
		//		d = 2;
		//		check_ns(maze, row,  col, w, h, d); 
		//		steps = 0;
			//}   
		//}   
		if(((row + 1 < h && maze[row + 1][col] == ' ')||(row - 1 >= 0 && maze[row - 1][col] == ' '))&&(col - 1 >= 0 && maze[row][col - 1] == ' ')) {
			//if(rrow != row || rcol != col){
		//		printf("W %d\n",steps);
		//		d = 2;
				//check_ns(maze, row,  col, w, h, d); 
		//		steps = 0;
			//}   
		}
		//printf("W %d\n",steps);
		//check_ns(maze, row,  col, w, h, 2);
	}
	if(steps != 0){
		printf("W %d\n",steps);
		direction(maze, row, col, w, h, 2);
		col += steps;
		printf("E %d\n",steps);
		maze[row][col - 1] = 'X';
		//int i;
		//for(i = 0;i<h;i++) printf("%s\n",maze[i]);
		//steps = 0;
	}

}

void check_ns(char * * maze, int row, int col, int w, int h, int d)
{
	int steps = 0;
	//int rrow = row;
	  //  int rcol = col;
	if(row + 1 < h && maze[row + 1][col] == ' '){
		steps ++;
		row ++;
		//if((col + 1 < w && maze[row][col + 1] == ' ')&&(col - 1 >= 0 && maze[row][col - 1] == ' ')) {
		//	if(rrow != row || rcol != col){
		//		printf("S %d\n",steps);
		//		d = 1;
		//		check_we(maze, row,  col, w, h, d);
		//		steps = 0;
		//	}
		//}
		/*if(((col + 1 < w && maze[row][col + 1] == ' ')||(col - 1 >= 0 && maze[row][col - 1] == ' '))&&(row + 1 < h && maze[row + 1][col] == ' ')){
			if(rrow != row || rcol != col){
				printf("S %d\n",steps);
				d = 1;
				check_we(maze, row,  col, w, h, d);
				steps = 0;
			}
		}*/
		//check_we(maze, row,  col, w, h, 1);
	}
	if(steps != 0){
		printf("S %d\n",steps);
		direction(maze, row, col, w, h, 1);
		row -= steps;
		printf("N %d\n",steps);
		maze[row + 1][col] = 'X';
		//int i;
		//for(i = 0;i<h;i++){
		//	printf("%s\n",maze[i]);
		//}
		steps = 0;
	}
	if(row - 1 >= 0 && maze[row - 1][col] == ' '){
		steps ++;
		row --;
		//if((col + 1 < w && maze[row][col + 1] == ' ')&&(col - 1 >= 0 && maze[row][col - 1] == ' ')) {
		//	if(rrow != row || rcol != col){
		//		printf("S %d\n",steps);
		//		d = 1;
		//		check_we(maze, row,  col, w, h, d);
		//		steps = 0;
		//	}
		/*}
		else if(((col + 1 < w && maze[row][col + 1] == ' ')||(col - 1 >= 0 && maze[row][col - 1] == ' '))&&(row + 1 < h && maze[row + 1][col] == ' ')){
			if(rrow != row || rcol != col){
				printf("S %d\n",steps);
				d = 1;
				check_we(maze, row,  col, w, h, d);
				steps = 0;
			}
		}*/
		//check_we(maze, row,  col, w, h, 0);
		//printf("1\n");
	}
	if(steps != 0){
		printf("N %d\n",steps);
		direction(maze, row, col, w, h, 0);
		row += steps;
		printf("S %d\n",steps);
		maze[row - 1][col] = 'X';
		//int i;
		//for(i = 0;i<h;i++) printf("%s\n",maze[i]);
		//steps = 0;
	}  
}

void destruct(char * * maze, int w, int h)
{
	int i;
	for (i = 0; i < h; i++){
		free(maze[i]);
	}
	free(maze);
}
