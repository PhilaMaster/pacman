/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Pasquale Papalia
** Modified date:           13/12/2024
** Version:                 v2.0
** Descriptions:            pacman extrapoint 1
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
//#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include <stdio.h>
#include "music/music.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif
extern currentPlaying;
void adjustPriorities();
int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */

	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       */
  LCD_Initialization();
	ADC_init();	
	ADC_start_conversion();
	
	//piccola attesa in modo che l'ADC calcoli un valore, per inizializzare il seed del random generator delle pills
	__ASM("wfi");
	LPC_SC -> PCONP |= (1 << 22);  // TURN ON TIMER 2,gestione ADC e random pills
	init_timer(2,0,0,3,0x17D784);//un secondo, reset+interrupt
	enable_timer(2);
	int i;
	for(i=0;i<5;i++)	__ASM("wfi");
	disable_timer(2);
  
	LPC_SC -> PCONP |= (1 << 23);  // TURN ON TIMER 3, gestione fantasmino, quattro velocità, una per ogni match register
	
	inizializza();
	inizializzaBlinky();
	BUTTON_init();
	joystick_init();											/* Joystick Initialization            */	
	currentPlaying = BASE;//musica base
	enable_RIT();													/* enable RIT to count 50ms				 */

	//adjustPriorities();
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
	//Speaker Initializzation
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
	//bus can
	CAN_Init();
	NVIC_EnableIRQ(EINT1_IRQn);//disable key1 until end of the game (used to send statistics)
	
  while (1)	
  {
		__ASM("wfi");
  }
}

void adjustPriorities(){//nota:spostamento pacman se non ha priorità massima potrebbe lasciare il vecchio pacman disegnato
	NVIC_SetPriority(RIT_IRQn,1);//timer, musica e polling joystick
	NVIC_SetPriority(TIMER0_IRQn,2);//musica
	NVIC_SetPriority(TIMER1_IRQn,0);//spostamento pacman
	NVIC_SetPriority(TIMER2_IRQn,2);//musica
	NVIC_SetPriority(TIMER3_IRQn,0);//spostamento fantasma
	NVIC_SetPriority(ADC_IRQn,4);
}
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
