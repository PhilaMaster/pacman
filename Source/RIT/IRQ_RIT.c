/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
//#include "../led/led.h"
//#include "../music/music.h"
#include "pacman/pacman.h"
#include "board/board.h"
#include <stdbool.h>
#include "GLCD/GLCD.h" 
/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
volatile int down_0 = 0;
volatile int down_1 = 0;
volatile int down_2 = 0;
volatile bool pause = true;
extern int actualDirection, wantedDirection, x, y;
extern uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];

void RIT_IRQHandler (void)
{			

	
	
/*************************JOYSTICK***************************/
static int up=0;	
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		/* Joytick UP pressed */
		up++;
		switch(up){
			case 1:
				if (board[y-1][x] != WALL)
					actualDirection = UP;
				wantedDirection = UP;
				break;
		}
	}
	else{
			up=0;
	}

	static int down=0;	
	
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){
		/* Joytick DOWN pressed */
		down++;
		switch(down){
			case 1:
				if (board[y+1][x] != WALL)
					actualDirection = DOWN;
				wantedDirection = DOWN;
				break;
		}
	}
	else{
			down=0;
	}
	static int left=0;	
	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
		/* Joytick LEFT pressed */
		left++;
		switch(left){
			case 1:
				if (board[y][x-1] != WALL)
					actualDirection = LEFT;
				wantedDirection = LEFT;
				break;
		}
	}
	else{
			left=0;
	}
	static int right=0;	
	
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
		/* Joytick RIGHT pressed */
		right++;
		switch(right){
			case 1:
				if (board[y][x+1] != WALL)
					actualDirection = RIGHT;
				wantedDirection = RIGHT;
				break;
		}
	}
	else{
			right=0;
	}
	/*
	static int select=0;	
	
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){
		// Joytick SELECT pressed 
		select++;
		switch(select){
			case 1:
				//my code
				break;
			default:
				break;
		}
	}
	else{
			select=0;
	}*/
/*************************INT0***************************/
if(down_0 !=0){
	down_0++;
	if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
		switch(down_0){
			case 2:
				pause = !pause;
				if(pause){
					GUI_Text(100, 140, (uint8_t *) "PAUSE", Red, White);
					disable_timer(0);//fermo il tempo
					disable_timer(1);//fermo pacman
					disable_timer(3);//fermo il fantasma
				}
				else{
					//coordinate della pausa
					int x1=9,x2=14,y1=10,y2=12;
					disable_RIT();//perse 2 ore di debug per capire perchè non funzionava ed era il RIT, ci metteva tanto a disegnare =)
					LCD_FillRegion(getX(x1)+3,getY(y1)+3,getX(x2)-3,getY(y2),Black);//disegno di nero lo sfondo sotto la scritta pausa
					enable_RIT();
					refreshBoard(x1,x2,y1,y2);//clear del messaggio di pausa
					enable_timer(0);//faccio ripartire il tempo
					enable_timer(1);//faccio ripartire pacman
					enable_timer(3);//faccio ripartire il fantasma
				}
			
				break;
			default:
				break;
		}
	}
	else {	/* button released */
		down_0=0;			
		NVIC_EnableIRQ(EINT0_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
	}
} // end INT0

/*************************KEY1***************************/
/*
if(down_1 !=0){
	down_1++;
	if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){
		switch(down_1){
			case 2:
				// your code	
				break;
			default:
				break;
		}
	}
	else {	// button released 
		down_1=0;			
		NVIC_EnableIRQ(EINT1_IRQn);							 // disable Button interrupts			
		LPC_PINCON->PINSEL4    |= (1 << 22);     // External interrupt 0 pin selection 
	}
} // end KEY1

**************************KEY2***************************
if(down_2 !=0){
	down_2++;
	if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){
		switch(down_2){
			case 2:
				// your code	
				break;
			default:
				break;
		}
	}
	else {	// button released 
		down_2=0;		
		NVIC_EnableIRQ(EINT2_IRQn);							 // disable Button interrupts			/
		LPC_PINCON->PINSEL4    |= (1 << 24);     // External interrupt 0 pin selection /
	}
} // end KEY2
	*/
//***************MUSICA****************
/*
	static int currentNote = 0;
	static int ticks = 0;
	if(!isNotePlaying())
	{
		++ticks;
		if(ticks == 1)
		{
			ticks = 0;
			playNote(song[currentNote++]);
		}
	}
	
	if(currentNote == (sizeof(song) / sizeof(song[0])))
	{
		disable_RIT();
	}
*/

	//********GESTIONE PERSONAGGIO********
	/*
	static int count = 0;
	switch(count){
		case 6://6*50 msec = 300msec
			if(actualDirection!=wantedDirection)
				switch(wantedDirection){//controllo se l'ultimo input del giocatore è valido, in caso cambio direzione
					case UP:
						if(board[y-1][x]!=WALL) actualDirection=wantedDirection; break;
					case DOWN:
						if(board[y+1][x]!=WALL) actualDirection=wantedDirection; break;
					case LEFT:
						if(board[y][x-1]!=WALL) actualDirection=wantedDirection; break;
					case RIGHT:
						if(board[y][x+1]!=WALL) actualDirection=wantedDirection; break;
				}
			if (!pause) spostaPersonaggio();
			count=0;
			break;
		
	}
	count++;
	*/
	
	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
