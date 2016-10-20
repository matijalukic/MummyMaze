#include <stdio.h>
#include "map.h"

void load_game(MAP* map_info,PLAYER* player_info) {
	FILE* f;
	int i, j, x;
	f = fopen("save.txt", "r");
	x = getc(f);
	switch (x) {
	case 'a':map_info->map_size = 16; break;  //dodati ostale velicine
	case 'b':map_info->map_size = 20; break;
	case 'c':map_info->map_size = 24; break;
	}
	for (i = 0; i < map_info->map_size;i++) {
		for (j = 0; j < map_info->map_size;j++) {
			switch (getc(f)) {
			case 'm': x = MUMMY; break;
			case 'd': x = DOOR; break;
			case 'w': x = WALL; break;
			case 'p': x = HUMAN; break;
			case 'r': x = PATH; break;
			}
			map_info->map_matrix[i][j] = x;
		}
	}
	player_info->number_of_moves = getc(f);
	map_info->minutes_played = getc(f);
	map_info->seconds_played = getc(f);
	map_info->player_emoticon = getc(f);
	map_info->wall_color = getc(f);
	map_info->game_level = getc(f);
	map_info->mummy_number = getc(f);
	map_info->load = 1;
}
void save_game(MAP* map_info, PLAYER* player_info) {
	FILE *f = fopen("save.txt", "w");
	int i, j;
	char x;
	switch (map_info->map_size) {
	case 16:x = 'a'; break;  //dodati ostale velicine
	case 20:x = 'b'; break;
	case 24:x = 'c'; break;
	}
	putc(x, f); 
	for (i = 0; i < map_info->map_size; i++) {
		for (j = 0; j < map_info->map_size; j++) {
			switch (map_info->map_matrix[i][j]) {
			case MUMMY: x = 'm'; break;
			case DOOR: x = 'd'; break;
			case WALL: x = 'w'; break;
			case HUMAN:x = 'p'; break;
			case PATH: x = 'r'; break;
			}
			putc(x, f);
		}
	}
	putc(player_info->number_of_moves, f);
	putc(map_info->minutes_played, f);
	putc(map_info->seconds_played, f);
	putc(map_info->player_emoticon, f);
	putc(map_info->wall_color, f);
	putc(map_info->game_level, f);
	putc(map_info->mummy_number, f);
	map_info->minutes_played = 0;
	map_info->seconds_played = 0;
	fclose(f);
}
void save_current_game(MAP* map_info, PLAYER* player_info) {
	FILE *f = fopen("continue.txt", "w");
	int i, j;
	char x;
	switch (map_info->map_size) {
	case 16:x = 'a'; break; 
	case 20:x = 'b'; break;
	case 24:x = 'c'; break;
	}
	putc(x, f);
	for (i = 0; i < map_info->map_size; i++) {
		for (j = 0; j < map_info->map_size; j++) {
			switch (map_info->map_matrix[i][j]) {
			case MUMMY: x = 'm'; break;
			case DOOR: x = 'd'; break;
			case WALL: x = 'w'; break;
			case HUMAN:x = 'p'; break;
			case PATH: x = 'r'; break;
			}
			putc(x, f);
		}
	}
	putc(player_info->number_of_moves, f);
	putc(map_info->minutes_played, f);
	putc(map_info->seconds_played, f);
	putc(map_info->player_emoticon, f);
	putc(map_info->wall_color, f);
	putc(map_info->game_level, f);
	putc(map_info->mummy_number, f);
	map_info->minutes_played = 0;
	map_info->seconds_played = 0;
	fclose(f);

}
void continue_game(MAP* map_info,PLAYER* player_info) {
	FILE* f;
	int i, j, x;
	f = fopen("continue.txt", "r");
	x = getc(f);
	switch (x) {
	case 'a':map_info->map_size = 16; break;  
	case 'b':map_info->map_size = 20; break;
	case 'c':map_info->map_size = 24; break;
	}
	for (i = 0; i < map_info->map_size; i++) {
		for (j = 0; j < map_info->map_size; j++) {
			switch (getc(f)) {
			case 'm': x = MUMMY; break;
			case 'd': x = DOOR; break;
			case 'w': x = WALL; break;
			case 'p': x = HUMAN; break;
			case 'r': x = PATH; break;
			}
			map_info->map_matrix[i][j] = x;
		}
	}
	player_info->number_of_moves = getc(f);
	map_info->minutes_played = getc(f);
	map_info->seconds_played = getc(f);
	map_info->player_emoticon = getc(f);
	map_info->wall_color = getc(f);
	map_info->game_level = getc(f);
	map_info->mummy_number = getc(f);
	map_info->load = 1;
}