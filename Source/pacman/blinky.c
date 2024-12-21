#include "board/board.h"
#include "pacman/pacman.h"
#include "LPC17xx.h"
#include "timer/timer.h"
#include "GLCD/GLCD.h"
#include <stdbool.h>
//#define INF 1e9

int velocita = 0, bx=B_INITIAL_X,by=B_INITIAL_Y;
bool feared = false, eaten=false;

//extern int astar(uint8_t grid[BOARD_HEIGHT][BOARD_WIDTH], int start_x, int start_y, int goal_x, int goal_y); //serve mantenere questo extern?
extern uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];
extern int x,y;

void disegnaBlinky(bool delete);

void inizializzaBlinky(){
	init_timer(3,0,0,3,0x989680);//velocità 1, 0,4 sec
	init_timer(3,0,1,1,0x4C4B40);//velocità 2, 0,2 sec
	init_timer(3,0,2,1,0x7270E0);//velocità 3, 0,1 sec
	init_timer(3,0,3,1,0x2625A0);//velocità3, 0,3 sec
	//disegno iniziale
	LCD_DrawSphere(getX(bx)+WIDTH/2, getY(by)+HEIGHT/2, PACMAN_RADIUS, Red);
}

void spostaFantasmino(){
	int direction = find_path(bx, by, x, y, feared);
	disegnaBlinky(true);
	if(direction==UP && board[by-1][bx]!=WALL){
		by--;
	}
	if(direction==DOWN && board[by+1][bx]!=WALL){
		by++;
	}
	if(direction==LEFT && board[by][bx-1]!=WALL){
		bx--;
	}
	if(direction==RIGHT && board[by][bx+1]!=WALL){
		bx++;
	}
	disegnaBlinky(false);
}

void disegnaBlinky(bool delete){
	if(delete){
		LCD_DrawSphere(getX(bx)+WIDTH/2, getY(by)+HEIGHT/2, PACMAN_RADIUS, Black);
		if(board[bx][by]!=EMPTY) refreshBoard(bx,bx+1,by,by+1); 
	}
	else{
		uint16_t color = Red;
		if(feared) color = Blue;
		LCD_DrawSphere(getX(bx)+WIDTH/2, getY(by)+HEIGHT/2, PACMAN_RADIUS, color);
	}
	
}

bool checkHit(){
	return x==bx && y==by;
}