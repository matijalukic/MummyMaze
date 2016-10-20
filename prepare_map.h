#pragma once
#include "map.h"

#define MAX_MATRIX 24
#define MAX_QUEUE 150

typedef struct map {
	int cell;
	int visit;
}copy_map;

typedef struct coordinate {
	int x, y;
	int new_value;
	struct coordinate *next;
}coordinate;

int calculate_cell(int x, int y, int n, copy_map matrix[MAX_MATRIX][MAX_MATRIX]);
int q_empty(coordinate *top, coordinate *bottom);
coordinate* q_insert(coordinate*top, coordinate *bottom, coordinate new_space);
coordinate* q_delete(coordinate *bottom);
void make_path(copy_map matrix[MAX_MATRIX][MAX_MATRIX], int n, int player_x, int player_y);
void prepare_map(MAP* map_info, PLAYER* player_info, copy_map copy_matrix[MAX_MATRIX][MAX_MATRIX]);				//ne sme da ostanu brojevi