#pragma once
#include "map.h"

typedef struct easy_mummy {
	int x_temp;
	int y_temp;

} MUMMY_COORDINATES; 

void find_mummy(MAP* map_info, MUMMY_COORDINATES* easy_mummy_info);
void mummy_move(MAP* map_info, MUMMY_COORDINATES*mummy_info, int x_next, int y_next, int *num);
