//#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "board/board.h"
#include "pacman/pacman.h"
#include "GLCD/GLCD.h"

#define LENGTH 100

extern uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];


typedef struct Node {
    uint8_t x, y;          // Coordinate del nodo
    struct Node* parent; // Nodo precedente nel percorso
} Node;

bool is_valid(int x, int y) {
	return board[y][x] != WALL;
}

bool visited[BOARD_HEIGHT][BOARD_WIDTH];

//BFS 
int find_path(int start_x, int start_y, int goal_x, int goal_y) {
    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    

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
            Node* temp = &current;
            while (temp->parent->parent != NULL) {
                temp = temp->parent;
            }
						if (temp->x == temp->parent->x){
							if(temp->parent->y > temp->y)
								return UP;
							else
								return DOWN;
						}
						else{
							if(temp->parent->x > temp->x)
								return LEFT;
							else
								return RIGHT;
						}
						
						gameOver();//debug, non dovrebbe mai arrivare qui
        }

        //esploro i vicini
				int i;
        for (i = 0; i < 4; i++) {
            int nx = current.x + directions[i][0];
            int ny = current.y + directions[i][1];

            if (is_valid(nx, ny) && !visited[ny][nx]) {
                visited[ny][nx] = true;
                Node next = {nx, ny, &queue[front-1]};//il nodo parent è l'ultimo analizzato
                queue[rear++] = next;
            }
        }
    }
		
    return 1;//non dovrebbe mai arrivare qui
}