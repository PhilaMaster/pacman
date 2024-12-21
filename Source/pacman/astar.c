//#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "board/board.h"
#include "pacman/pacman.h"
#include "GLCD/GLCD.h"

#define LENGTH 100

extern uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];
int manhattan_distance(int x1, int y1, int x2, int y2);
int invert(int direction);
typedef struct Node {
    uint8_t x, y;
    struct Node* parent; //nodo precedente nel cammino
} Node;

bool is_valid(int x, int y) {
	return board[y][x] != WALL;
}

bool visited[BOARD_HEIGHT][BOARD_WIDTH];

//BFS - visita in ampiezza della mappa, aggiungo vicini in coda ma leggo all'inizio
//ottenendo così la via più corta, considerando che il peso associato allo spostarsi
//nella mappa è unitario. Si potrebbero fare delle ottimizzazioni euristiche aggiungendo
//a ogni spostamento un costo legato alla distanza manhattan, ad esempio. In questo modo
//si giungerebbe prima alla soluzione. (variante di A*)
int find_path(int start_x, int start_y, int goal_x, int goal_y, bool inverted) {
	int directions[4][2] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
	

	int i=0,j=0;
	for(i=0;i<BOARD_HEIGHT;i++)
		for(j=0;j<BOARD_WIDTH;j++)
			visited[i][j]=false;

	Node queue[BOARD_WIDTH * BOARD_HEIGHT];
	int front = 0, rear = 0;

	Node start = {start_x, start_y, NULL};
	queue[rear++] = start;
	visited[start_y][start_x] = true;

	// BFS
	while (front < rear) {
		Node current = queue[front++];
		
		//ricostruisco il percorso fino agli ultimi due elementi per decidere la direzione da prendere
		if (current.x == goal_x && current.y == goal_y) {
			int ret;
			Node* temp = &current;
			while (temp->parent->parent != NULL) {
				temp = temp->parent;
			}
			if (temp->x == temp->parent->x){
				if(temp->parent->y > temp->y)
					ret= UP;
				else
					ret= DOWN;
			}
			else{
				if(temp->parent->x > temp->x)
					ret= LEFT;
				else
					ret= RIGHT;
			}
			if(inverted){//per scappare dal fantasma
				//applico qui un'euristica per selezionare la direzione migliore nel caso in cui
				//l'opposto non sia consentito (ad esempio c'è un muro)
				int fx=start_x,fy=start_y;//posizione opposta
				switch(ret){
					case UP: fy++; break;
					case DOWN: fy--; break;
					case LEFT: fx++; break;
					case RIGHT: fx--; break;
				}
				if (is_valid(fx,fy)) return invert(ret);//restituisco direzione invertita
				else{//applico euristica selezionando la direzione con distanza manhattan maggiore
					int maxDistance = -1, bestDirection=-1;
					for (i = 0; i < 4; i++) {
						int nx = start_x + directions[i][0];
						int ny = start_y + directions[i][1];
						
						if(is_valid(nx,ny)){
							int dist = manhattan_distance(nx,ny,goal_x,goal_y);
							if(dist>maxDistance){
								maxDistance=dist;
								bestDirection=i;
							}
						}
					}
					ret = bestDirection;
				}
			}
			return ret;
			gameOver();//debug, non dovrebbe mai arrivare qui
		}

		//esploro i vicini
		int i;
		for (i = 0; i < 4; i++) {
			int nx = current.x + directions[i][0];
			int ny = current.y + directions[i][1];

			if (is_valid(nx, ny) && !visited[ny][nx]) {
				visited[ny][nx] = true;
				Node next = {nx, ny, &queue[front-1]};//il nodo parent front-1 perchè avevo fatto ++
				queue[rear++] = next;
			}
		}
	}
	
	return 1;//non dovrebbe mai arrivare qui
}

int manhattan_distance(int x1, int y1, int x2, int y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}

int invert(int direction){
	switch(direction){
		case UP: return DOWN;
		case DOWN: return UP; 
		case LEFT: return RIGHT; 
		case RIGHT: return LEFT;
		default: return UP;
	}
}