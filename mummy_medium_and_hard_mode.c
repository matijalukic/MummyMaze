#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mummy_moves.h"
#include "prepare_map.h"
#include "map.h"

#define MAX_MATRIX 24

void mummy_move_harder_mode(MAP* map_info, MUMMY_COORDINATES*mummy_info, int x_next, int y_next, copy_map copy_matrix[MAX_MATRIX][MAX_MATRIX]) {
	int next_move;

	next_move = map_info->map_matrix[x_next][y_next];

	if (next_move == PATH || next_move == HUMAN) {		//kad dodamo jos jednu mumiju, ovde menjam da se napravi jedna
		map_info->map_matrix[x_next][y_next] = MUMMY;
		copy_matrix[x_next][y_next].cell = MUMMY;
		map_info->map_matrix[mummy_info->x_temp][mummy_info->y_temp] = PATH;
		copy_matrix[mummy_info->x_temp][mummy_info->y_temp].cell = PATH;
		
		mummy_info->x_temp = x_next;
		mummy_info->y_temp = y_next;

		if (next_move == HUMAN)
			map_info->hit = MUMMY;
	}
}

void mummy_harder(WINDOW* map_win, MAP* map_info, PLAYER* player_info, int level) {
	MUMMY_COORDINATES* mummy_info;
	copy_map copy_matrix[MAX_MATRIX][MAX_MATRIX];
	int x_next, y_next;
	int max, mis_turn;
	int counter = 0;
	int i,j;
	int mummy_number = map_info->mummy_number;
	int num = 0;

	mummy_info = calloc(1, sizeof(PLAYER*));

	mummy_info->x_temp = 0;
	mummy_info->y_temp = 0;
	
																		//za odredivanje u kom koraku ce mumija da pogrresi
	switch (level) {
		case 2: mis_turn = 3; break;
		case 3: mis_turn = 5; break;
	}
	
	for (num; num < mummy_number; num++) {
		counter = 0;

		find_mummy(map_info, mummy_info);
		find_player(map_info, player_info);
		prepare_map(map_info, player_info, copy_matrix);

		for (counter; counter < MUMMY_TURN; counter++) {

			max = 0;
			int flagy = 1;
			int see;

			if (map_info->game_level == 2) see = 3;					//postavljam u odnosu na tezinu koliko daleko mumija moze da "vidi"
			else see = 4;

			for (i = mummy_info->x_temp - see; i <= mummy_info->x_temp + see; i++) {
				for (j = mummy_info->y_temp - see; j <= mummy_info->y_temp + see; j++) {
					if (copy_matrix[i][j].cell == HUMAN) {
						flagy = 0;
						break;
					}
				}
				if (!flagy) break;
			}

			if (player_info->number_of_moves % mis_turn == 0 && flagy) {				//ovako smo napravili tezinu mumija
				int lower_max = -1;
				int flag = 1;

				if (mummy_info->x_temp != 0) {
					if (copy_matrix[mummy_info->x_temp - 1][mummy_info->y_temp].cell == HUMAN) {				//ako je u njegovoj blizini igrac, ide pravo na njega
						x_next = mummy_info->x_temp - 1;
						y_next = mummy_info->y_temp;
						flag = 0;
					}
					if (max < copy_matrix[mummy_info->x_temp - 1][mummy_info->y_temp].cell && flag) {				//gore
						max = copy_matrix[mummy_info->x_temp - 1][mummy_info->y_temp].cell;
					}
					if (lower_max<copy_matrix[mummy_info->x_temp - 1][mummy_info->y_temp].cell && flag && max

					> copy_matrix[mummy_info->x_temp - 1][mummy_info->y_temp].cell) {
						lower_max = copy_matrix[mummy_info->x_temp - 1][mummy_info->y_temp].cell;
						x_next = mummy_info->x_temp - 1;
						y_next = mummy_info->y_temp;
					}

				}
				if (mummy_info->x_temp != map_info->map_size - 1) {
					if (copy_matrix[mummy_info->x_temp + 1][mummy_info->y_temp].cell == HUMAN) {
						x_next = mummy_info->x_temp + 1;
						y_next = mummy_info->y_temp;
						flag = 0;
					}
					if (max < copy_matrix[mummy_info->x_temp + 1][mummy_info->y_temp].cell && flag) {					//dole
						max = copy_matrix[mummy_info->x_temp + 1][mummy_info->y_temp].cell;
					}
					if (lower_max<copy_matrix[mummy_info->x_temp + 1][mummy_info->y_temp].cell && flag && max > copy_matrix[mummy_info->x_temp + 1][mummy_info->y_temp].cell) {
						lower_max = copy_matrix[mummy_info->x_temp + 1][mummy_info->y_temp].cell;
						x_next = mummy_info->x_temp + 1;
						y_next = mummy_info->y_temp;
					}
				}
				if (mummy_info->y_temp != 0) {
					if (copy_matrix[mummy_info->x_temp][mummy_info->y_temp - 1].cell == HUMAN) {
						x_next = mummy_info->x_temp;
						y_next = mummy_info->y_temp - 1;
						flag = 0;
					}
					if (max < copy_matrix[mummy_info->x_temp][mummy_info->y_temp - 1].cell && flag) {					//levo
						max = copy_matrix[mummy_info->x_temp][mummy_info->y_temp - 1].cell;
					}
					if (lower_max<copy_matrix[mummy_info->x_temp][mummy_info->y_temp - 1].cell && flag && max

					> copy_matrix[mummy_info->x_temp][mummy_info->y_temp - 1].cell) {
						lower_max = copy_matrix[mummy_info->x_temp][mummy_info->y_temp - 1].cell;
						x_next = mummy_info->x_temp;
						y_next = mummy_info->y_temp - 1;
					}
				}
				if (mummy_info->y_temp != map_info->map_size - 1) {
					if (copy_matrix[mummy_info->x_temp][mummy_info->y_temp + 1].cell == HUMAN) {
						x_next = mummy_info->x_temp;
						y_next = mummy_info->y_temp + 1;
						flag = 0;
					}
					if (max < copy_matrix[mummy_info->x_temp][mummy_info->y_temp + 1].cell && flag) {					//desno
						max = copy_matrix[mummy_info->x_temp][mummy_info->y_temp + 1].cell;
					}
					if (lower_max<copy_matrix[mummy_info->x_temp][mummy_info->y_temp + 1].cell && flag && max > copy_matrix[mummy_info->x_temp][mummy_info->y_temp + 1].cell) {
						lower_max = copy_matrix[mummy_info->x_temp][mummy_info->y_temp + 1].cell;
						x_next = mummy_info->x_temp;
						y_next = mummy_info->y_temp + 1;
					}

				}

				mummy_move_harder_mode(map_info, mummy_info, x_next, y_next, copy_matrix);
				if (map_info->hit == MUMMY) break;
				else {
					print_map(map_win, map_info);
					for (i = 0; i < 80000000; i++) {
						//da ostane ispis
					}
				}
			}
			else if (mummy_info->x_temp != player_info->x_temp || mummy_info->y_temp != player_info->y_temp) {
				int flag = 1;

				if (mummy_info->x_temp != 0) {
					if (copy_matrix[mummy_info->x_temp - 1][mummy_info->y_temp].cell == HUMAN) {
						x_next = mummy_info->x_temp - 1;
						y_next = mummy_info->y_temp;
						flag = 0;
					}
					if (max < copy_matrix[mummy_info->x_temp - 1][mummy_info->y_temp].cell && flag) {				//gore
						max = copy_matrix[mummy_info->x_temp - 1][mummy_info->y_temp].cell;
						x_next = mummy_info->x_temp - 1;
						y_next = mummy_info->y_temp;
					}
				}
				if (mummy_info->x_temp != map_info->map_size - 1) {
					if (copy_matrix[mummy_info->x_temp + 1][mummy_info->y_temp].cell == HUMAN) {
						x_next = mummy_info->x_temp + 1;
						y_next = mummy_info->y_temp;
						flag = 0;
					}
					if (max < copy_matrix[mummy_info->x_temp + 1][mummy_info->y_temp].cell && flag) {				//dole
						max = copy_matrix[mummy_info->x_temp + 1][mummy_info->y_temp].cell;
						x_next = mummy_info->x_temp + 1;
						y_next = mummy_info->y_temp;
					}
				}
				if (mummy_info->y_temp != 0) {
					if (copy_matrix[mummy_info->x_temp][mummy_info->y_temp - 1].cell == HUMAN) {
						x_next = mummy_info->x_temp;
						y_next = mummy_info->y_temp - 1;
						flag = 0;
					}
					if (max < copy_matrix[mummy_info->x_temp][mummy_info->y_temp - 1].cell && flag) {				//levo
						max = copy_matrix[mummy_info->x_temp][mummy_info->y_temp - 1].cell;
						x_next = mummy_info->x_temp;
						y_next = mummy_info->y_temp - 1;
					}
				}
				if (mummy_info->y_temp != map_info->map_size - 1) {
					if (copy_matrix[mummy_info->x_temp][mummy_info->y_temp + 1].cell == HUMAN) {
						x_next = mummy_info->x_temp;
						y_next = mummy_info->y_temp + 1;
						flag = 0;
					}
					if (max < copy_matrix[mummy_info->x_temp][mummy_info->y_temp + 1].cell && flag) {				//desno
						max = copy_matrix[mummy_info->x_temp][mummy_info->y_temp + 1].cell;
						x_next = mummy_info->x_temp;
						y_next = mummy_info->y_temp + 1;
					}
				}

				mummy_move_harder_mode(map_info, mummy_info, x_next, y_next, copy_matrix);
				if (map_info->hit == MUMMY) break;
				else {
					print_map(map_win, map_info);
					for (i = 0; i < 80000000; i++) {
						//da ostane ispis
					}
				}

			}
			else {
				map_info->hit = MUMMY;
				break;
			}
		}
	}
	
}