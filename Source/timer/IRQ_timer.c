/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
//#include "../TouchPanel/TouchPanel.h"
//#include "../pac"
#include <stdio.h> /*for sprintf*/
#include "board/board.h"
#include "pacman/pacman.h"
#include <stdbool.h>
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern remainingTime;
extern score;
uint16_t SinTable[45] =
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

#include "music/music.h"
void TIMER0_IRQHandler (void)
{
	if(LPC_TIM0->IR & 1) // MR0
	{ 
		static int sineticks=0;
		/* DAC management */	
		static int currentValue; 
		currentValue = SinTable[sineticks]*VOLUME/100;
		currentValue -= 410;
		currentValue /= 1;
		currentValue += 410;
		LPC_DAC->DACR = currentValue <<6;
		sineticks++;
		if(sineticks==45) sineticks=0;
		/*old time management 
		remainingTime--;
		disegnaTempo();
		if (remainingTime==0)
			gameOver();*/
		LPC_TIM0->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM0->IR & 2){ // MR1
		// your code	
		LPC_TIM0->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM0->IR & 4){ // MR2
		// your code	
		LPC_TIM0->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM0->IR & 8){ // MR3
		// your code	
		LPC_TIM0->IR = 8;			// clear interrupt flag 
	}
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern bool pause;
extern int actualDirection,wantedDirection,x,y;
extern uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];

void TIMER1_IRQHandler (void)
{
	if(LPC_TIM1->IR & 1) // MR0
	{ 

		if(actualDirection!=wantedDirection)
			switch(wantedDirection){//controllo se l'ultimo input del giocatore � valido, in caso cambio direzione
				case UP:
					if(board[y-1][x]!=WALL) actualDirection=wantedDirection; break;
				case DOWN:
					if(board[y+1][x]!=WALL) actualDirection=wantedDirection; break;
				case LEFT:
					if(board[y][x-1]!=WALL) actualDirection=wantedDirection; break;
				case RIGHT:
					if(board[y][x+1]!=WALL) actualDirection=wantedDirection; break;
			}
		//if (!pause) 
		spostaPersonaggio();
			
		//controllo fantasmino
		if(checkHit())
			hitted();
		
		//ogni tanto il rit sfora e non si resetta, rimedio con questo reset
		reset_RIT();
		LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
		LPC_TIM1->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM1->IR & 2){ // MR1
		// your code	
		LPC_TIM1->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM1->IR & 4){ // MR2
		// your code	
		LPC_TIM1->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM1->IR & 8){ // MR3
		// your code	
		LPC_TIM1->IR = 8;			// clear interrupt flag 
	} 

	return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void)
{
	if(LPC_TIM2->IR & 1) // MR0
	{ 
		ADC_start_conversion();
		disable_timer(0);
		LPC_TIM2->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM2->IR & 2){ // MR1
		// your code	
		LPC_TIM2->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM2->IR & 4){ // MR2
		// your code	
		LPC_TIM2->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM2->IR & 8){ // MR3
		// your code	
		LPC_TIM2->IR = 8;			// clear interrupt flag 
	} 

	return;
}


/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern velocita, feared;

void TIMER3_IRQHandler (void)
{
	if(LPC_TIM3->IR & 1) // MR0
	{ 
		static int count=0;
		spostaFantasmino();
		if(count++>40 && velocita < 2 && !feared){
			velocita++;
			count = 0;
		}
		LPC_TIM3->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM3->IR & 2){ // MR1
		if (velocita==2)
			spostaFantasmino();
		LPC_TIM3->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM3->IR & 4){ // MR2
		if (velocita>0)
			spostaFantasmino();
		LPC_TIM3->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM3->IR & 8){ // MR3
		if (velocita==2)
			spostaFantasmino();
		LPC_TIM3->IR = 8;			// clear interrupt flag 
	} 
	//controllo fantasmino
	if(checkHit())
		hitted();
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
