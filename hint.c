#include <stdio.h>
#include <stdlib.h>
#include "prepare_map.h"
#include "map.h"

Dot find_hint(MAP *map_info, Dot place, int x_exit, int y_exit, int *flagy) {
	copy_map copy_matrix[MAX_MATRIX][MAX_MATRIX];
	int i, j;
	int max = 0;
	int x_next;
	int y_next;
	int flag = 1;
	Dot helper;

	for (i = 0; i < map_info->map_size; i++)
		for (j = 0; j < map_info->map_size; j++) {
			copy_matrix[i][j].cell = map_info->map_matrix[i][j];
		}

	make_path(copy_matrix, map_info->map_size, x_exit, y_exit);
/*
	for (i = 0; i < map_info->map_size; i++, printf("\n")) {
		for (j = 0; j < map_info->map_size; j++) {
			printf("%d ", copy_matrix[i][j].cell);
		}
	}
	*/
	if (place.x != 0) {
		if (copy_matrix[place.x - 1][place.y].cell == DOOR) {
			x_next = -1;
			y_next = -1;
			flag = 0;
		}
		if (max < copy_matrix[place.x - 1][place.y].cell && flag) {				//gore
			max = copy_matrix[place.x - 1][place.y].cell;
			x_next = place.x - 1;
			y_next = place.y;
		}
	}
	if (place.x != map_info->map_size - 1) {
		if (copy_matrix[place.x + 1][place.y].cell == DOOR) {
			x_next = -1;
			y_next = -1;
			flag = 0;
		}
		if (max < copy_matrix[place.x + 1][place.y].cell && flag) {				//dole
			max = copy_matrix[place.x + 1][place.y].cell;
			x_next = place.x + 1;
			y_next = place.y;
		}
	}
	if (place.y != 0) {
		if (copy_matrix[place.x][place.y - 1].cell == DOOR) {
			x_next = -1;
			y_next = -1;
			flag = 0;
		}
		if (max < copy_matrix[place.x][place.y - 1].cell && flag) {				//levo
			max = copy_matrix[place.x][place.y - 1].cell;
			x_next = place.x;
			y_next = place.y - 1;
		}
	}
	if (place.y != map_info->map_size - 1) {
		if (copy_matrix[place.x][place.y + 1].cell == DOOR) {
			x_next = -1;
			y_next = -1;
			flag = 0;
		}
		if (max < copy_matrix[place.x][place.y + 1].cell && flag) {				//desno
			max = copy_matrix[place.x][place.y + 1].cell;
			x_next = place.x;
			y_next = place.y + 1;
		}
	}

	*flagy = flag;
	helper.x = x_next;
	helper.y = y_next;
	return helper;
}

Dot * hint(MAP *map_info, Dot place, int x_exit, int y_exit) {
	Dot *hints, point1, point2, point3;
	int flag = 1;

	hints = (Dot *)calloc(3, sizeof(Dot));

	point1 = find_hint(map_info, place, x_exit, y_exit, &flag);
	if (flag) { 
		point2 = find_hint(map_info, point1, x_exit, y_exit, &flag);
	}
	else { 
		point2.x = -1;
		point2.y = -1;
	}
	if (flag) {
		point3 = find_hint(map_info, point2, x_exit, y_exit, &flag);
	}
	else {
		point3.x = -1;
		point3.y = -1;
	}


	if (!hints) return NULL;

	hints[0] = point1;
	hints[1] = point2;
	hints[2] = point3;

	return hints;
	
}

