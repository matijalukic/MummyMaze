#include <stdio.h>
#include <stdlib.h>
#include "map.h"

#define MAX_MATRIX 24
#define MAX_QUEUE 500

typedef struct map {
	int cell;
	int visit;
}copy_map;

typedef struct coordinate {
	int x, y;
	int new_value;
}coordinate;

int calculate_cell(int x, int y, int n, copy_map matrix[MAX_MATRIX][MAX_MATRIX]) {					//ide po vrstama i trazi slepe ulice
	int counter = 0;
	int max = 0;

	if (x == 0) counter++;
	else if (matrix[x - 1][y].cell >= 1) {
		counter++;
		if (matrix[x - 1][y].cell > max) max = matrix[x - 1][y].cell;		
	}

	if (x == n - 1) counter++;
	else if (matrix[x + 1][y].cell >= 1) {
		counter++;
		if (matrix[x + 1][y].cell > max) max = matrix[x + 1][y].cell;
	}

	if (y == 0) counter++;
	else if (matrix[x][y - 1].cell >= 1) {
		counter++;
		if (matrix[x][y - 1].cell > max) max = matrix[x][y - 1].cell;
	}

	if (y == n - 1) counter++;
	else if (matrix[x][y + 1].cell >= 1) {
		counter++;
		if (matrix[x][y + 1].cell > max) max = matrix[x][y + 1].cell;
	}

	if (counter == 3) {
		if (matrix[x][y].cell != HUMAN && matrix[x][y].cell != MUMMY && matrix[x][y].cell != DOOR) {

			matrix[x][y].cell = max + 1;
			while (1) {
				int left;
				int flag;

				if (y == 0)
					break;

				left = y - 1;											//odvija se rekurzivno posto mora unazad da se proveri da li se usput napravila neka slepa ulica
				if (matrix[x][left].cell == 1)
					break;

				flag = calculate_cell(x, left, n, matrix);				//rekurzivno

				if (flag) break;
			}

			while (1) {
				int up;
				int flag;

				if (x == 0) break;

				up = x - 1;
				if (matrix[up][y].cell == 1) break;

				flag = calculate_cell(up, y, n, matrix);				//rekurzivno

				if (flag) break;
			}
		}
	}
	return 1;
}

int q_empty(int top, int bottom) {

	if (top == bottom) return 1;
	else return 0;
}

void q_insert(int* top, coordinate queue[MAX_QUEUE], coordinate new_space) {

	if (*top == MAX_QUEUE) {
		*top = 0;
		queue[*top] = new_space;
	}
	else {
		queue[*top] = new_space;
		*top = *top + 1;
	}
}

coordinate q_delete(int *bottom, coordinate queue[MAX_QUEUE]) {
	coordinate helper;

	if (*bottom == MAX_QUEUE) {
		*bottom = 0;
		helper = queue[*bottom];
		*bottom += 1;
	}
	else {
		helper = queue[*bottom];
		*bottom += 1;
	}

	return helper;
}

void make_path(copy_map matrix[MAX_MATRIX][MAX_MATRIX], int n, int player_x, int player_y) {
	int i, j;
	coordinate queue[MAX_QUEUE];
	coordinate new_space;
	int *top, *bottom;
	int waste;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			if (matrix[i][j].cell != 1)
				waste = calculate_cell(i, j, n, matrix);
		}

	for (i = 0; i < n; i++)						//stavlja da jos nijednu celiju nismo presli
		for (j = 0; j < n; j++) {
			matrix[i][j].visit = 0;
		}
	top = malloc(sizeof(int));
	bottom = malloc(sizeof(int));

	*top = *bottom = 0;

	matrix[player_x][player_y].visit = 1;			//algoritam BFS krece od igraca

	new_space.x = player_x;
	new_space.y = player_y;
	new_space.new_value = 5000;					//hteo sam prvo INT_MAX ali je mnogo veliki 

	q_insert(top, queue, new_space);

	while (!(q_empty(*top, *bottom))) {
		coordinate help;

		help = q_delete(bottom, queue);

		if (help.x != 0)
			if ((matrix[help.x - 1][help.y].cell == 0) && (matrix[help.x - 1][help.y].visit == 0)) {
				new_space.x = help.x - 1;
				new_space.y = help.y;												//gornja celija
				new_space.new_value = help.new_value - 1;												//sve okolne celije su za jedan manji od pocetne
				matrix[help.x - 1][help.y].cell = help.new_value - 1;
				matrix[help.x - 1][help.y].visit = 1;
				q_insert(top, queue, new_space);
			}

		if (help.x != n - 1)
			if ((matrix[help.x + 1][help.y].cell == 0) && (matrix[help.x + 1][help.y].visit == 0)) {
				new_space.x = help.x + 1;
				new_space.y = help.y;
				new_space.new_value = help.new_value - 1;												//donja celija
				matrix[help.x + 1][help.y].cell = help.new_value - 1;
				matrix[help.x + 1][help.y].visit = 1;
				q_insert(top, queue, new_space);
			}

		if (help.y != 0)
			if ((matrix[help.x][help.y - 1].cell == 0) && (matrix[help.x][help.y - 1].visit == 0)) {
				new_space.x = help.x;
				new_space.y = help.y - 1;
				new_space.new_value = help.new_value - 1;														//levo celija
				matrix[help.x][help.y - 1].cell = help.new_value - 1;
				matrix[help.x][help.y - 1].visit = 1;
				q_insert(top, queue, new_space);
			}

		if (help.y != n - 1)
			if ((matrix[help.x][help.y + 1].cell == 0) && (matrix[help.x][help.y + 1].visit == 0)) {
				new_space.x = help.x;
				new_space.y = help.y + 1;
				new_space.new_value = help.new_value - 1;														//desno celija
				matrix[help.x][help.y + 1].cell = help.new_value - 1;
				matrix[help.x][help.y + 1].visit = 1;
				q_insert(top, queue, new_space);
			}
	}

	free(top);
	free(bottom);
}

void prepare_map(MAP* map_info, PLAYER* player_info, copy_map copy_matrix[MAX_MATRIX][MAX_MATRIX]) {				//smanjiti ovaj max za matrice
	int i, j;

	for (i = 0; i < map_info->map_size; i++)
		for (j = 0; j < map_info->map_size; j++) {
			copy_matrix[i][j].cell = map_info->map_matrix[i][j];
		}

	find_player(map_info, player_info);

	make_path(copy_matrix, map_info->map_size, player_info->x_temp, player_info->y_temp);					// ovde su mi potrebne koordinate igraca

}