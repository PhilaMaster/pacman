//#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "board/board.h"
#include "pacman/pacman.h"
#include "GLCD/GLCD.h"

#define MAX_NODES 100 // Numero massimo di nodi gestibili

typedef struct Node {
    int x, y;         // Coordinate
    int g, h, f;      // Costi
    struct Node* parent; // Puntatore al nodo precedente
} Node;

// Funzione per calcolare l'euristica (distanza Manhattan)
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Controlla se una cella è valida (nei limiti e non è un muro)
bool is_valid(int x, int y, uint8_t grid[BOARD_HEIGHT][BOARD_WIDTH]) {
    return (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT && grid[x][y] == 0);
}//TODO per gestire il tp posso mettere in OR x>0 e <WIDTH con la y pari al tp

/*
// Funzione per creare un nodo
Node* create_node(int x, int y, int g, int h, Node* parent) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->x = x;
    node->y = y;
    node->g = g;
    node->h = h;
    node->f = g + h;
    node->parent = parent;
    return node;
}
*/

// Pool di nodi statico e indice per il prossimo nodo disponibile
static Node nodes_pool[MAX_NODES];
static int nodes_index = 0;

// Funzione per creare un nuovo nodo
Node* create_node(int x, int y, int g, int h, Node* parent) {
    if (nodes_index >= MAX_NODES) {
        // Pool pieno, impossibile creare un nuovo nodo
        return NULL;
    }

    // Ottieni un puntatore al nodo successivo disponibile
    Node* node = &nodes_pool[nodes_index++];
    node->x = x;
    node->y = y;
    node->g = g;
    node->h = h;
    node->f = g + h;
    node->parent = parent;

    return node;
}

// Aggiungi un nodo alla lista (Open List)
void add_to_open(Node* open_list[], int* open_size, Node* node) {
    open_list[*open_size] = node;
    (*open_size)++;
}

// Trova il nodo con f più basso nella Open List
int find_lowest_f(Node* open_list[], int open_size) {
    int min_index = 0;
		int i;
    for (i = 1; i < open_size; i++) {
        if (open_list[i]->f < open_list[min_index]->f) {
            min_index = i;
        }
    }
    return min_index;
}

// Rimuovi un nodo dalla Open List
void remove_from_open(Node* open_list[], int* open_size, int index) {
		int i;
    for (i = index; i < *open_size - 1; i++) {
        open_list[i] = open_list[i + 1];
    }
    (*open_size)--;
}

// Ricostruisci il percorso
void reconstruct_path(Node* current) {
    //printf("Percorso trovato:\n");
    while (current != NULL) {
        //printf("(%d, %d) <- ", current->x, current->y);
        current = current->parent;
    }
    //printf("Start\n");
}

//test iniziale: a ogni refresh del fantasmino ricalcolo il percorso minimo (da ottimizzare: restituisce una lista di mosse da fare)
int reconstruct_first_move(Node* current) {

	//in teoria avrà sempre due elementi, il primo (della lista) sarà il nodo iniziale, mentre l'ultimo il finale
    while (current->parent != NULL) 
        current = current->parent;
    
		int xSucc=current->x, ySucc= current->y;
		int xPrec=current->parent->x, yPrec= current->parent->y;
		if(xSucc==xPrec){
			if (ySucc-yPrec>0)
				return DOWN;
			else
				return UP;
		}else
			if(xSucc-xPrec>0)
				return RIGHT;
			else
				return LEFT;
}

// Algoritmo A*
int astar(uint8_t grid[BOARD_HEIGHT][BOARD_WIDTH], int start_x, int start_y, int goal_x, int goal_y) {
    // Open List e Closed List
	//TODO rimpiazzare con codice stub solo per vedere se il resto va
  /*  
	Node* open_list[BOARD_HEIGHT * BOARD_WIDTH];
    int open_size = 0;
    bool closed_list[BOARD_HEIGHT][BOARD_WIDTH];

    // Aggiungi nodo iniziale alla Open List
    Node* start_node = create_node(start_x, start_y, 0, heuristic(start_x, start_y, goal_x, goal_y), NULL);
    add_to_open(open_list, &open_size, start_node);

    // Direzioni possibili (su, giù, sinistra, destra)
    int directions[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    while (open_size > 0) {
        // Trova il nodo con il costo f più basso
        int current_index = find_lowest_f(open_list, open_size);
        Node* current = open_list[current_index];
        
        // Se siamo al nodo obiettivo, ricostruisci il percorso
        if (current->x == goal_x && current->y == goal_y) {
            //reconstruct_path(current);
            return reconstruct_first_move(current);
        }

        // Rimuovi il nodo dalla Open List e aggiungilo alla Closed List
        remove_from_open(open_list, &open_size, current_index);
        closed_list[current->x][current->y] = true;

        // Esplora i vicini
				int i,j;
        for (i = 0; i < 4; i++) {
            int nx = current->x + directions[i][0];
            int ny = current->y + directions[i][1];

            // Se il vicino non è valido o è già nella Closed List, salta
            if (!is_valid(nx, ny, grid) || closed_list[nx][ny]) continue;

            int g = current->g + 1; // Costo g accumulato
            int h = heuristic(nx, ny, goal_x, goal_y);

            // Controlla se il nodo esiste già nella Open List
            bool in_open = false;
            for (j = 0; j < open_size; j++) {
                if (open_list[j]->x == nx && open_list[j]->y == ny) {
                    in_open = true;
                    if (g < open_list[j]->g) {
                        open_list[j]->g = g;
                        open_list[j]->f = g + h;
                        open_list[j]->parent = current;
                    }
                    break;
                }
            }

            // Se il nodo non è nella Open List, aggiungilo
            if (!in_open) {
                Node* neighbor = create_node(nx, ny, g, h, current);
                add_to_open(open_list, &open_size, neighbor);
            }
        }
    }
		*/
	return UP;//non dovrebbe mai arrivare qua, significherebbe non aver trovato un percorso
}