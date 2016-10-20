#include <stdio.h>
#include <stdlib.h>

#define HUMAN -1
#define MUMMY -2

typedef struct map {
	int cell;
	int visit;

}map_info;

typedef struct coordinate {
	int x, y;
	int new_value;
	struct coordinate *next;

}coordinate;

int calculate_cell(int x, int y, int n, map_info **matrix) {
	int counter = 0;
	int max = 0;

	if (x == 0) counter++;
	else if (matrix[x - 1][y].cell >= 1) {
		counter++;
		if (matrix[x - 1][y].cell > max) max = matrix[x - 1][y].cell;			//napravi copy_map
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
		if (matrix[x][y].cell != HUMAN && matrix[x][y].cell != MUMMY) {

			matrix[x][y].cell = max + 1;
			while (1) {
				int left;
				int flag;

				if (y == 0)
					break;

				left = y - 1;
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

int q_empty(coordinate *top, coordinate *bottom) {
	int helper;

	if (top == bottom) helper = 1;
	else helper = 0;

	return helper;
}

coordinate* q_insert(coordinate*top, coordinate *bottom, coordinate new_space) {
	coordinate *helper;

	helper = calloc(1, sizeof(coordinate));
	*helper = new_space;


	top->next = helper;
	top = top->next;
	return top;
}

coordinate* q_delete(coordinate *bottom) {
	coordinate *helper;

	helper = bottom->next;

	free(bottom);

	return helper;
}

void q_print(coordinate *bottom) {
	coordinate one;

	one = *bottom;

	while (one.next != NULL) {
		printf("%d,%d ", one.x, one.y);
		one = *one.next;
	}
	printf("\n");
}

void make_path(map_info **matrix, int n, int x, int y) {
	int i, j;
	coordinate *top, *bottom;
	coordinate new_space;
	int waste;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			if (matrix[i][j].cell != 1)
				waste = calculate_cell(i, j, n, matrix);
		}

	printf("\n");

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%d ", matrix[i][j].cell);
		}
		printf("\n");
	}


	for (i = 0; i < n; i++)						//find_mummy, find_player
		for (j = 0; j < n; j++) {
			matrix[i][j].visit = 0;
		}

	top = bottom = calloc(1, sizeof(int));

	matrix[x][y].visit = 1;

	new_space.x = x;
	new_space.y = y;
	new_space.next = NULL;
	new_space.new_value = 500;					//hteo sam prvo INT_MAX ali je mnogo veliki 

	top = q_insert(top, bottom, new_space);
	q_print(bottom);

	while (!(q_empty(top, bottom))) {
		coordinate help;

		bottom = q_delete(bottom);
		help = *bottom;

		if (help.x != 0)
			if (matrix[help.x - 1][help.y].cell == 0 && matrix[help.x - 1][help.y].visit == 0) {
				new_space.x = help.x - 1;
				new_space.y = help.y;												//gore
				new_space.new_value = help.new_value - 1;
				new_space.next = NULL;
				matrix[help.x - 1][help.y].cell = help.new_value - 1;
				matrix[help.x - 1][help.y].visit = 1;
				top = q_insert(top, bottom, new_space);
			}

		if (help.x != n - 1)
			if (matrix[help.x + 1][help.y].cell == 0 && matrix[help.x + 1][help.y].visit == 0) {
				new_space.x = help.x + 1;
				new_space.y = help.y;
				new_space.new_value = help.new_value - 1;												//dole
				new_space.next = NULL;
				matrix[help.x + 1][help.y].cell = help.new_value - 1;
				matrix[help.x + 1][help.y].visit = 1;
				top = q_insert(top, bottom, new_space);
			}

		if (help.y != 0)
			if (matrix[help.x][help.y - 1].cell == 0 && matrix[help.x][help.y - 1].visit == 0) {
				new_space.x = help.x;
				new_space.y = help.y - 1;
				new_space.new_value = help.new_value - 1;
				new_space.next = NULL;															//levo
				matrix[help.x][help.y - 1].cell = help.new_value - 1;
				matrix[help.x][help.y - 1].visit = 1;
				top = q_insert(top, bottom, new_space);
			}

		if (help.y != n - 1)
			if (matrix[help.x][help.y + 1].cell == 0 && matrix[help.x][help.y + 1].visit == 0) {
				new_space.x = help.x;
				new_space.y = help.y + 1;
				new_space.new_value = help.new_value - 1;
				new_space.next = NULL;															//desno
				matrix[help.x][help.y + 1].cell = help.new_value - 1;
				matrix[help.x][help.y + 1].visit = 1;
				top = q_insert(top, bottom, new_space);
			}
	}

}
