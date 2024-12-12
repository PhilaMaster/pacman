#include "GLCD/GLCD.h" 
#include "stdio.h"
#include "board/board.h"

void disegna();
void inizializza();

extern uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];

void disegnaTempo();
void disegnaScore();
void disegnaTesto();

int remainingTime = 60, score = 0, remainingLives = 1;

void inizializzaSchermo(){
	LCD_Clear(Black);
	GUI_Text(0, 0, (uint8_t *) "Score: 00000          Time: 60", Red, White);
	GUI_Text(0, 304, (uint8_t *) "Remaining lives: 1            ", Red, White);
	
	primoDisegnoBoard();
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