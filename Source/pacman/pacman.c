#include "GLCD/GLCD.h" 
#include "stdio.h"

#define X_OFFSET 12
#define Y_OFFSET 30
#define WIDTH 8
#define HEIGHT 8
#define BOARD_WIDTH 27
#define BOARD_HEIGHT 32
int getXvalue(int j);
int getYvalue(int i);
void disegna();
void inizializza();

uint8_t board  [BOARD_HEIGHT][BOARD_WIDTH];

int getXvalue(int j){return X_OFFSET+j*WIDTH;}
int getYvalue(int i){return Y_OFFSET+i*HEIGHT;}
void disegnaTempo();
void disegnaScore();
void disegnaTesto();





int remainingTime = 60, score = 0, remainingLives = 1;

void disegnaTutto(){
	LCD_Clear(Black);
	GUI_Text(0, 0, (uint8_t *) "Score: 00000          Time: 60", Red, White);
	GUI_Text(0, 304, (uint8_t *) "Remaining lives: 1            ", Red, White);
	
	int i,j;
	
	for (i=0;i<BOARD_HEIGHT;i++)
		for(j=0;j<BOARD_WIDTH;j++){	
			int x_start = getXvalue(j);
			int y_start = getYvalue(i);
			if(board[i][j]=='1')
				LCD_DrawLine(x_start, y_start, x_start+WIDTH, y_start+HEIGHT, White);
		}
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

void inizializza(){
	int i,j;
	
	for (i=0;i<BOARD_HEIGHT;i++)
		for(j=0;j<BOARD_WIDTH;j++)
			board[i][j]='0';
	
	for (i=0;i<BOARD_HEIGHT;i++){
		board[i][0] = '1';
		board[i][BOARD_WIDTH-1] = '1';
	}
	
	for (j=0;j<BOARD_WIDTH;j++){
		board[0][j] = '1';
		board[BOARD_HEIGHT-1][j] = '1';
	}
	
	
	disegnaTutto();
}