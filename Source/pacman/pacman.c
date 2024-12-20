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
void victory();
void gameOver();

//info a schermo
int remainingTime = 60, score = 0, livesScore=0, remainingLives = 1, pills=0;

//personaggio
int x=INITIAL_X,y=INITIAL_Y, actualDirection = DOWN, wantedDirection = DOWN, tpCoordinate;
bool mosso=false,teleported=false;

//extern int bx,by,velocita;

void inizializzaSchermo(){
	LCD_Clear(Black);
	GUI_Text(0, 0, (uint8_t *) "Score: 00000          Time: 60", Red, White);
	GUI_Text(0, 304, (uint8_t *) "Remaining lives: 1            ", Red, White);
	
	//disegno tutta la mappa
	refreshBoard(0,BOARD_WIDTH, 0, BOARD_HEIGHT);
	//primo disegno di pacman:
	LCD_DrawSphere(getX(x)+WIDTH/2, getY(y)+HEIGHT/2, PACMAN_RADIUS, Yellow);
	//LCD_DrawSphere(getX(bx)+WIDTH/2, getY(by)+HEIGHT/2, PACMAN_RADIUS, Red);
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

void disegnaVite(){
	char text[3];
	sprintf(text, "%d", remainingLives);
	GUI_Text(135, 304, (uint8_t *)text, Red, White);
}

void disegnaPacman(){
	LCD_DrawSphere(getX(x)+WIDTH/2, getY(y)+HEIGHT/2, PACMAN_RADIUS, Yellow);
	int drawX,drawY;
	switch(actualDirection){
		case UP:
			drawX=x; drawY=y+1;
			break;
		case LEFT:
			if(!teleported) drawX=x+1; 
			else {
				drawX=1;
				teleported=false;
			}
			drawY=y;
			break;
		case DOWN:
			drawX=x; drawY=y-1;
			break;
		case RIGHT:
			if(!teleported) drawX=x-1;
			else {
				drawX=BOARD_WIDTH-2;
				teleported=false;
			}
			drawY=y;
			break;
	}
	LCD_DrawSphere(getX(drawX)+WIDTH/2, getY(drawY)+HEIGHT/2, PACMAN_RADIUS, Black);
}

void fastRefresh(){
	if (mosso)
		disegnaPacman();
}

int randomPills[] = {-1,-1,-1,-1,-1,-1};

bool contiene(int v[], int n){
	int i;
	for (i=0;i<6;i++)
		if (v[i] == n)
			return true;
	return false;
}
void bubblesort(int v[], int n) {
	int i,k;
	int temp;
	for(i = 0; i<n-1; i++) {
	 for(k = 0; k<n-1-i; k++) {
					 if(v[k] > v[k+1]) {
						temp = v[k];
						v[k] = v[k+1];
						v[k+1] = temp;
					 }
	 }
	}
}
extern AD_current;
void inizializza(){
	int i,j;
	srand(AD_current);
	//conto numero pills
	for(i=0;i<BOARD_HEIGHT;i++)
		for(j=0;j<BOARD_WIDTH;j++)
			if (board[i][j]==PILL)
				pills++;
	//genero casualmente le super pills	
	j=0;
	for(i=0;i<6;i++){
		int r;
		do{
			r = rand()%pills;
		}while(contiene(randomPills,r));
		randomPills[j++]=r;
	}
	bubblesort(randomPills,6); //poichè distribuisco le pills in ordine
	j=0;//itera sul vettore randPills (6)
	i=0;//itera sull'intera board (WIDTH*HEIGHT)
	int k=0;//indice che itera sulle pills (270)
	while(j<6 && i<BOARD_WIDTH*BOARD_HEIGHT){
		if(board[i/BOARD_WIDTH][i%BOARD_WIDTH] == PILL && randomPills[j]==k++){
			board[i/BOARD_WIDTH][i%BOARD_WIDTH] = SUPER_PILL;
			j++;
		}
		i++;
	}
	
	//disegno iniziale
	inizializzaSchermo();
	
	GUI_Text(100, 140, (uint8_t *) "PAUSE", Red, White);//scritta di pausa
	init_timer(0,0,0,3,0x17D7840);//timer per lo scorrere del tempo, decrementa ogni secondo il tempo rimanente
	//enable_timer(0); //all'inizio sono in uno stato di pausa, quindi il tempo non scorre subito
	init_timer(1,0,0,3,0x393870);//timer per il refresh dello schermo (velocità pacman) 393870 -> 0,15 sec refresh
	//enable_timer(1); //non si deve muovere se in pausa
}

void spostaPersonaggio(){
	int prevScore = score;
	switch(actualDirection){
		case UP:
			if(board[y-1][x] != WALL){
				y--;
				mosso=true;
				if (board[y][x]==PILL){
					score+=10;livesScore+=10;
					board[y][x]=EMPTY;
				} 
				else if (board[y][x]==SUPER_PILL){
					score+=50;livesScore+=50;
					board[y][x]=EMPTY;
				}
			}else mosso=false;
			break;
		case LEFT:
			if(board[y][x-1] != WALL){
				x--;
				mosso=true;
				if (board[y][x]==PILL){
					score+=10;livesScore+=10;
					board[y][x]=EMPTY;
				} 
				else if (board[y][x]==SUPER_PILL){
					score+=50;livesScore+=50;
					board[y][x]=EMPTY;
				}
				else if (board[y][x]==TELEPORT){
					teleported=true;
					x=BOARD_WIDTH-1;
				}
			}else mosso=false;
			break;
		case DOWN:
			if(board[y+1][x] != WALL){
				y++;
				mosso=true;
				if (board[y][x]==PILL){
					score+=10;livesScore+=10;
					board[y][x]=EMPTY;
				} 
				else if (board[y][x]==SUPER_PILL){
					score+=50;livesScore+=50;
					board[y][x]=EMPTY;
				}
			}else mosso=false;
			break;
		case RIGHT:
			if(board[y][x+1] != WALL){
				x++;
				mosso=true;
				if (board[y][x]==PILL){
					score+=10;livesScore+=10;
					board[y][x]=EMPTY;
				} 
				else if (board[y][x]==SUPER_PILL){
					score+=50;livesScore+=50;
					board[y][x]=EMPTY;
				}
				else if (board[y][x]==TELEPORT){
					teleported=true;
					x=1;
				}
			}else mosso=false;
			break;
	}
	if (score!=prevScore) {
		disegnaScore();
		
		if(score>=(pills-6)*10+6*50) victory();
		
		if (livesScore>=1000){
			livesScore%=1000;
			remainingLives++;
			disable_timer(0);//ci mette un po' a disegnare, blocco il tempo intanto
			disegnaVite();
			enable_timer(0);
		}
	}
	fastRefresh();
}

void victory(){
	disable_RIT();//fermo input
	disable_timer(0);//fermo tempo
	disable_timer(1);//fermo personaggio
	disable_timer(3);//fermo fantasmino
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	GUI_Text(80, 140, (uint8_t *) "VICTORY!", Red, White);
	GUI_Text(50, 170, (uint8_t *) "Reset to play again", Red, White);
}

void gameOver(){
	disable_RIT();//fermo input
	disable_timer(0);//fermo tempo
	disable_timer(1);//fermo personaggio
	disable_timer(3);//fermo fantasmino
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	GUI_Text(80, 140, (uint8_t *) "Game over!", Red, White);
	GUI_Text(50, 170, (uint8_t *) "Reset to play again", Red, White);
}

extern int velocita,bx,by;

void hitted(){
	disable_timer(0);//fermo tempo
	disable_timer(1);//fermo personaggio
	disable_timer(3);//fermo fantasmino
	if(remainingLives==1) gameOver();
	else{
		//aggiorno vite
		remainingLives--;
		disegnaVite();
		//cancello vecchio pacman
		LCD_DrawSphere(getX(x)+WIDTH/2, getY(y)+HEIGHT/2, PACMAN_RADIUS, Black);
		//imposto nuove coordinate
		x=INITIAL_X;
		y=INITIAL_Y;
		bx=B_INITIAL_X;
		by=B_INITIAL_Y;
		//resetto velocità fantasmino
		velocita=0;
	}
	
	enable_timer(0);
	enable_timer(1);
	enable_timer(3);
}