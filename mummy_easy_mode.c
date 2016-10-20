#include <stdio.h>
#include <curses.h>
#include <math.h>
#include "map.h"

typedef struct easy_mummy {
	int x_temp;
	int y_temp;

} EASY_MUMMY;

void find_mummy(MAP* map_info, EASY_MUMMY* easy_mummy_info) {
	int i, j, go_out = 0;
	int first_time = 1, k;
	while (!go_out) {
		for (i = easy_mummy_info->x_temp; i < map_info->map_size; i++) {
			if (first_time) {
				first_time = 0;
				k = easy_mummy_info->y_temp + 1;
			}
			else
				k = 0;
			for (j = k; j < map_info->map_size; j++) {
				
				if (map_info->map_matrix[i][j] == MUMMY) {
					easy_mummy_info->x_temp = i;
					easy_mummy_info->y_temp = j;
					go_out = 1;
					break;
				}
			}
			if (go_out)
				break;
		}
		if (!go_out) {
			easy_mummy_info->x_temp = 0;
			easy_mummy_info->y_temp = 0;
		}
	}

}

void mummy_move(MAP* map_info,EASY_MUMMY*mummy_info, int x_next, int y_next, int *num) {
	int next_move;

	next_move = map_info->map_matrix[x_next][y_next];

	if (next_move == PATH || next_move == HUMAN || next_move == MUMMY) {		//kad dodamo jos jednu mumiju, ovde menjam da se napravi jedna
		map_info->map_matrix[x_next][y_next] = MUMMY;
		map_info->map_matrix[mummy_info->x_temp][mummy_info->y_temp] = PATH;

		mummy_info->x_temp = x_next;
		mummy_info->y_temp = y_next;

		if (next_move == MUMMY) num--;

		if (next_move == HUMAN) 
			map_info->hit = MUMMY;
	}

}

void mummy_movement(WINDOW* map_win, MAP* map_info) {
	PLAYER* player_info;
	EASY_MUMMY* mummy_info;
	int x_diference, y_diference, i;
	int x_next, y_next;
	int helper;
	int counter = 0;
	int mummy_number = map_info->mummy_number;
	int num = 0;

	mummy_info = calloc(1, sizeof(EASY_MUMMY));

	mummy_info->x_temp = 0;
	mummy_info->y_temp = 0;

	for (num; num < mummy_number; num++) {
		counter = 0;

	find_mummy(map_info, mummy_info);

	player_info = calloc(1, sizeof(PLAYER));
	find_player(map_info, player_info);

	for (counter; counter < MUMMY_TURN; counter++) {			//okrece se onoliko pute koliko mumija u jednom potezu moze da ide

		if (mummy_info->x_temp != player_info->x_temp || mummy_info->y_temp != player_info->y_temp) {			// nepotreban uslov!
			x_diference = abs(mummy_info->x_temp - player_info->x_temp);
			y_diference = abs(mummy_info->y_temp - player_info->y_temp);

			if (x_diference == 0)x_diference = MAX;
			if (y_diference == 0)y_diference = MAX;

			if (x_diference <= y_diference) {

				helper = player_info->x_temp - mummy_info->x_temp;

				if (helper > 0) {
					x_next = mummy_info->x_temp + 1;
					y_next = mummy_info->y_temp;
				}
				else {
					x_next = mummy_info->x_temp - 1;
					y_next = mummy_info->y_temp;
				}
			}
			else {

				helper = player_info->y_temp - mummy_info->y_temp;

				if (helper > 0) {
					x_next = mummy_info->x_temp;
					y_next = mummy_info->y_temp + 1;
				}
				else {
					x_next = mummy_info->x_temp;
					y_next = mummy_info->y_temp - 1;
				}
			}

			mummy_move(map_info, mummy_info, x_next, y_next, &mummy_number);
			if (map_info->hit == MUMMY) break;
			else if (counter == 0) {
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

