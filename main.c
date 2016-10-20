#pragma comment(lib, "winmm.lib")
#include <windows.h>
#include <stdio.h>
#include <curses.h>
#include "map.h"
#include "menu.h"
#include "about.h"
#include "options.h"
#include "algorithms.h"
#include "high_score.h"


void continue_game(MAP*, PLAYER*);
void save_current_game(MAP*, PLAYER*);

int main() {
	int choice = 1;
	char x;
	PLAYER* player_info = calloc(1, sizeof(PLAYER));
	MAP* map_info = calloc(1, sizeof(MAP));
	OPTIONS* options_info = calloc(1, sizeof(OPTIONS));
	FILE *f;

	map_info->minutes_played = 0;
	map_info->seconds_played = 0;
	options_info->game_level = map_info->game_level = 1;
	options_info->map_size = map_info->map_size = 16;
	options_info->player_emoticon = map_info->player_emoticon = 'P';
	map_info->wall_color = 1;
	map_info->mummy_number = 1;

	print_picture();

	while (choice != 9) {
		choice = menu();
		switch (choice) {
		case 1: set_alg(map_info); set_draw(map_info); gameplay(map_info, player_info); map_info->hit = 0; break;
		case 2: continue_game(map_info, player_info); gameplay(map_info, player_info); map_info->load = 0; map_info->map_size = options_info->map_size; break;
		case 3: load_game(map_info, player_info); gameplay(map_info, player_info); map_info->load = 0; map_info->map_size = options_info->map_size; break;
		case 4: write_high_scores(); break;
		case 5: 
			options_info->map_size = map_info->map_size;
			options_info->game_level = map_info->game_level;
			options_info->player_emoticon = map_info->player_emoticon;
			options_info->wall_color = map_info->wall_color;
			options_info->mummy_number = map_info->mummy_number;

			options_main(options_info); 

			map_info->map_size = options_info->map_size;
			map_info->game_level = options_info->game_level;
			map_info->player_emoticon = options_info->player_emoticon;
			map_info->wall_color = options_info->wall_color;
			map_info->mummy_number = options_info->mummy_number;
			break;
		case 6: help(); break;
		case 7: about_game(); break;
		case 8: authors(); break;
		case 9: initscr(); wprintw(stdscr, ""); clrtoeol(); refresh(); endwin(); exit(1);
		}
	}
	f = fopen("continue.txt", "w");
	putc(f, 'p');



	return 0;
}