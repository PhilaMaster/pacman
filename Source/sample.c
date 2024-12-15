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

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */

	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       */
  LCD_Initialization();
	ADC_init();	
	ADC_start_conversion();
	
	//piccola attesa in modo che l'ADC calcoli un valore, per inizializzare il seed del random generator delle pills
	__ASM("wfi");
	LPC_SC -> PCONP |= (1 << 22);  // TURN ON TIMER 2
	init_timer(2,0,0,3,0x17D784);//un secondo, reset+interrupt
	enable_timer(2);
	int i;
	for(i=0;i<5;i++)	__ASM("wfi");
	disable_timer(2);
  
	//LPC_SC -> PCONP |= (1 << 23);  // TURN ON TIMER 3	
	
	inizializza();
	BUTTON_init();
	joystick_init();											/* Joystick Initialization            */	
	enable_RIT();													/* enable RIT to count 50ms				 */


	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
