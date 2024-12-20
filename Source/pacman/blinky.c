#include "board/board.h"
#include "pacman/pacman.h"
#include "LPC17xx.h"
#include "timer/timer.h"
#include "GLCD/GLCD.h"
#include <stdbool.h>
//#define INF 1e9

int velocita = 0, bx=B_INITIAL_X,by=B_INITIAL_Y;

//extern int astar(uint8_t grid[BOARD_HEIGHT][BOARD_WIDTH], int start_x, int start_y, int goal_x, int goal_y); //serve mantenere questo extern?
extern uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];
extern int x,y;

void disegnaBlinky(bool delete);

void inizializzaBlinky(){
	init_timer(3,0,0,3,0x2FAF080);//velocità 1
	init_timer(3,0,1,1,0x17D7840);//velocità 2
	init_timer(3,0,2,1,0xBEBC20);//velocità 3
	init_timer(3,0,3,1,0x23C3460);//velocità 3
	//disegno iniziale
	LCD_DrawSphere(getX(bx)+WIDTH/2, getY(by)+HEIGHT/2, PACMAN_RADIUS, Red);
}

void spostaFantasmino(){
	int direction = astar(board, bx, by, x, y);
	disegnaBlinky(true);
	if(direction==UP && board[by-1][bx]!=WALL)by--;
	if(direction==DOWN && board[by+1][bx]!=WALL)by++;
	if(direction==LEFT && board[by][bx-1]!=WALL)bx--;
	if(direction==RIGHT && board[by][bx+1]!=WALL)bx++;
	disegnaBlinky(false);
}

void disegnaBlinky(bool delete){
	if(delete){
		LCD_DrawSphere(getX(bx)+WIDTH/2, getY(by)+HEIGHT/2, PACMAN_RADIUS, Black);
		if(board[bx][by]!=EMPTY) refreshBoard(bx,bx+1,by,by+1); 
	}
	else
		LCD_DrawSphere(getX(bx)+WIDTH/2, getY(by)+HEIGHT/2, PACMAN_RADIUS, Red);
	
}

bool checkHit(){
	return x==bx && y==by;
}