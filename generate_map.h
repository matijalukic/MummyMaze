#include <stdio.h>
#include <stdlib.h>
#include "map.h"

#define RAND (rand() % (map_point->map_size - 2)) + 1

typedef struct dot {
	int x, y;
} Dot;

int dot_empty(Dot t);
int set_empty(Dot *target, int n);
int sets_num(Dot **sets, int n);
int in_set(Dot target, Dot dots[], int n);
Dot *find_set(Dot target, Dot ** sets, int n);
Dot *dot_to_set(Dot * target, Dot source, int n);
Dot * set_remove(Dot * target);
Dot * union_sets(Dot * target, Dot * source, int n);
Dot ** new_set(Dot new_dot, Dot **sets, int n);
int iter(int map_size);
int inside(Dot target, int map_size);
Dot neighbor(Dot target, MAP *map, char direction);
int neighbors(Dot first, Dot second);
int has_neighbors(Dot target, MAP *map);
Dot **generate_map(MAP **map);
Dot **extend_map(MAP **map, Dot **sets);

