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
#include "music/music.h"
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
volatile bool pause = true, timerEnabled=false;
extern int actualDirection, wantedDirection, x, y, remainingTime;
extern uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];
extern bool feared, eaten, velocita;
int respawnCounter=0, fearCounter=0;
int currentPlaying = INTRO;

NOTE pacman_cookie[] = 
{
    {c4, time_semicroma}, 
    {g3, time_semicroma}, 
    {c4, time_semicroma}, 
    {g3, time_semicroma}, 
    {c4, time_semicroma}, 
    {pausee, time_semicroma} // breve pausa per creare il ritmo
};

NOTE pacman_super_pill[] = 
{
    // Pattern ripetuto rapidamente
    {c4, time_biscroma}, 
    {g3, time_biscroma}, 
    {c4, time_biscroma}, 
    {g3, time_biscroma}, 
    {c4, time_biscroma}, 
    {g3, time_biscroma},
    
    // Secondo ciclo (per dare continuità)
    {d4, time_biscroma}, 
    {a3, time_biscroma}, 
    {d4, time_biscroma}, 
    {a3, time_biscroma}, 
    {d4, time_biscroma}, 
    {a3, time_biscroma},
    
    // Pausa breve per ritmo
    {pausee, time_semibiscroma},

    // Ripetizione del pattern
    {c4, time_biscroma}, 
    {g3, time_biscroma}, 
    {c4, time_biscroma}, 
    {g3, time_biscroma}, 
    {c4, time_biscroma}, 
    {g3, time_biscroma},
};

NOTE pacman_base_theme[] = 
{
    // Pattern principale
    {c4, time_semicroma}, 
    {g3, time_semicroma}, 
    {c4, time_semicroma}, 
    {g3, time_semicroma},

    // Pausa per un ritmo meno frenetico
    {pausee, time_biscroma},

    // Seconda parte con piccola variazione
    {d4, time_semicroma}, 
    {a3, time_semicroma}, 
    {d4, time_semicroma}, 
    {a3, time_semicroma},

    // Pausa di chiusura prima di ripetere
    {pausee, time_biscroma},

    // Ripetizione del pattern
    {c4, time_semicroma}, 
    {g3, time_semicroma}, 
    {c4, time_semicroma}, 
    {g3, time_semicroma},
};

NOTE pacman_intro[] = 
{
    // Sequenza iniziale riconoscibile
    {g3, time_semicroma}, 
    {g4, time_semicroma}, 
    {a4, time_semicroma}, 
    {g4, time_semicroma},

    // Breve pausa per creare tensione
    {pausee, time_biscroma},

    // Crescendo rapido
    {f4, time_semicroma}, 
    {g4, time_semicroma}, 
    {a4, time_semicroma}, 
    {g4, time_semicroma}, 
    {b4, time_semicroma},

    // Pausa per chiusura
    {pausee, time_semicroma},

    // Conclusione
    {c5, time_croma}, 
    {pausee, time_croma}, 
};

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

/*************************INT0***************************/
if(down_0 !=0){
	down_0++;
	if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
		switch(down_0){
			case 2:
				pause = !pause;
				if(pause){
					GUI_Text(100, 140, (uint8_t *) "PAUSE", Red, White);
					fermaTempo();
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
					avviaTempo();//faccio ripartire il tempo
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
//***************MUSICA****************

	static int currentNote = 0;
	static int ticks = 0;
	
	static bool intro = true;
	if(!isNotePlaying() && !pause)
	{
		++ticks;
		if(ticks == 1)
		{
			ticks = 0;
			
			switch(currentPlaying){
				case INTRO:
				if(intro){
					playNote(pacman_intro[currentNote++]);
					if (currentNote++/(sizeof(pacman_base_theme)/sizeof(pacman_base_theme[0]))) intro=false;
				}
				break;
				case BASE:playNote(pacman_base_theme[currentNote++%(sizeof(pacman_base_theme)/sizeof(pacman_base_theme[0]))]);break;
				case FAST:playNote(pacman_super_pill[currentNote++%(sizeof(pacman_super_pill)/sizeof(pacman_super_pill[0]))]);break;
				case BISCUIT:playNote(pacman_cookie[currentNote++%(sizeof(pacman_cookie)/sizeof(pacman_cookie[0]))]);break;
				default: playNote(pacman_base_theme[currentNote++%(sizeof(pacman_base_theme)/sizeof(pacman_base_theme[0]))]);
			}
			
		}
	}

	//gestione tempo di gioco (timer dei 60 secondi)
	static int timerCounter=0;
	if (timerEnabled) timerCounter++;
	if (timerCounter==20){//refresh ogni secondo, quindi 1000msec/50msec=20
		remainingTime--;
		disegnaTempo();
		if (remainingTime==0)
			gameOver();
		timerCounter=0;
	}
		


	//gestione fear fantasmino, dopo 10 secondi deve tornare normale
	//10000msec/50msec = 200
	if (feared && fearCounter == 200){
		feared=false;
		currentPlaying=BASE;
		velocita=1;
	}
	fearCounter++;
	//gestione respawn fantasmino, dopo 3 secondi deve tornare
	//3000msec/50msec = 60

	if (eaten && respawnCounter==60){
		enable_timer(3);
		eaten=false;
	}
	respawnCounter++;
	
	
	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
