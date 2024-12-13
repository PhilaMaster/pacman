#include "GLCD/GLCD.h" 
#include "stdio.h"
#include "board/board.h"
#include "pacman.h"
#include <stdbool.h>
void disegna();
void inizializza();

extern uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];

void disegnaTempo();
void disegnaScore();
void disegnaTesto();

//info a schermo
int remainingTime = 60, score = 0, remainingLives = 1;

//personaggio
int x=5,y=3, lastDirection = DOWN;
bool mosso=false;

void inizializzaSchermo(){
	LCD_Clear(Black);
	GUI_Text(0, 0, (uint8_t *) "Score: 00000          Time: 60", Red, White);
	GUI_Text(0, 304, (uint8_t *) "Remaining lives: 1            ", Red, White);
	
	primoDisegnoBoard();
	//primo disegno di pacman:
	LCD_DrawSphere(getX(x)+WIDTH/2, getY(y)+HEIGHT/2, PACMAN_RADIUS, Yellow);
}

void disegnaTesto(){
	disegnaTempo();
	disegnaScore();
	GUI_Text(0, 304, (uint8_t *) "Remaining lives: 1            ", Red, White);
}

void disegnaTempo(){
	char text[3];
	sprintf(text, "%02d", remainingTime);
	GUI_Text(224, 0, (uint8_t *)text, Red, White);
}

void disegnaScore(){
	char text[6];
	sprintf(text, "%05d", score);
	GUI_Text(56, 0, (uint8_t *)text, Red, White);
}

void disegnaPacman(){
	LCD_DrawSphere(getX(x)+WIDTH/2, getY(y)+HEIGHT/2, PACMAN_RADIUS, Yellow);
	switch(lastDirection){
		case UP:
			LCD_DrawSphere(getX(x)+WIDTH/2, getY(y+1)+HEIGHT/2, PACMAN_RADIUS, Black);
			break;
		case LEFT:
			LCD_DrawSphere(getX(x+1)+WIDTH/2, getY(y)+HEIGHT/2, PACMAN_RADIUS, Black);
			break;
		case DOWN:
			LCD_DrawSphere(getX(x)+WIDTH/2, getY(y-1)+HEIGHT/2, PACMAN_RADIUS, Black);
			break;
		case RIGHT:
			LCD_DrawSphere(getX(x-1)+WIDTH/2, getY(y)+HEIGHT/2, PACMAN_RADIUS, Black);
			break;
	}
}

void fastRefresh(){
	if (mosso)
		disegnaPacman();
}

void inizializza(){
	int i,j;
	int count=0;
	//conto pillole, devono essere 270, solo debug per count pillole
    for (i = 1; i < BOARD_HEIGHT - 1; i++) {
        for (j = 1; j < BOARD_WIDTH - 1; j++) {
            if (board[i][j] == PILL){ 
								count++;
            }
        }
    }
	score = count;
	
	
	//disegno iniziale
	inizializzaSchermo();
	disegnaScore();//solo debug per count pillole
}

void spostaPersonaggio(){
	int prevScore = score;
	switch(lastDirection){
		case UP:
			if(board[y-1][x] != WALL){
				y--;
				mosso=true;
				if (board[y][x]==PILL){
					score+=10;
					board[y][x]=EMPTY;
				} 
				else if (board[y][x]==SUPER_PILL){
					score+=50;
					board[y][x]=EMPTY;
				}
			}else mosso=false;
			break;
		case LEFT:
			if(board[y][x-1] != WALL){
				x--;
				mosso=true;
				if (board[y][x]==PILL){
					score+=10;
					board[y][x]=EMPTY;
				} 
				else if (board[y][x]==SUPER_PILL){
					score+=50;
					board[y][x]=EMPTY;
				}
				else if (board[y][x]==TELEPORT)
					x=BOARD_WIDTH-1;
			}else mosso=false;
			break;
		case DOWN:
			if(board[y+1][x] != WALL){
				y++;
				mosso=true;
				if (board[y][x]==PILL){
					score+=10;
					board[y][x]=EMPTY;
				} 
				else if (board[y][x]==SUPER_PILL){
					score+=50;
					board[y][x]=EMPTY;
				}
			}else mosso=false;
			break;
		case RIGHT:
			if(board[y][x+1] != WALL){
				x++;
				mosso=true;
				if (board[y][x]==PILL){
					score+=10;
					board[y][x]=EMPTY;
				} 
				else if (board[y][x]==SUPER_PILL){
					score+=50;
					board[y][x]=EMPTY;
				}
				else if (board[y][x]==TELEPORT)
					x=1;
			
			}else mosso=false;
			break;
	}
	if (score!=prevScore) disegnaScore();
	fastRefresh();
}