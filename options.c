#pragma comment(lib, "winmm.lib")
#include <windows.h>
#include <stdio.h>
#include <curses.h>
#define WIDTH 30
#define HEIGHT 11
#define KEY_ESC 27
#define KEY_ENTER 10
#define EASY 1
#define MEDIUM 2
#define HARD 3
typedef struct OPTIONS {
	int map_size;
	int game_diff;
	int mummy_number;
	int player_emoticon;
	int wall_color;
}OPTIONS;


char* options_choices[] = {
	"Map Size",
	"Difficulty",
	"Number of Mummies",
	"Player Emoticon",
	"Wall color",
	"Show current settings",
	"Music",
	"Back"
};
int n_options = sizeof(options_choices) / sizeof(char *);
void print_options(WINDOW*, int);

int options_menu(OPTIONS* options_info) {
	WINDOW* opt_win;
	int highlight = 1;
	int choice = 0;
	int c, startx, starty;

	initscr();
	resize_term(30, 50);
	clear();
	noecho();
	cbreak();
	start_color();
	curs_set(0);
	startx = (50 - WIDTH) / 2;
	starty = (30 - HEIGHT) / 2;

	opt_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(opt_win, TRUE);
	print_options(opt_win, highlight);
	while (1) {
		c = wgetch(opt_win);
		switch (c) {
		case KEY_UP:
			if (highlight == 1)
				highlight = n_options;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if (highlight == n_options)
				highlight = 1;
			else
				++highlight;
			break;
		case KEY_ENTER:
			choice = highlight;
			break;
		default: break;
		}
		print_options(opt_win, highlight);
		if (choice != 0)
			break;
	}
	clrtoeol();
	refresh();
	endwin();
	return choice;
}
void print_options(WINDOW* opt_win, int highlight) {
	int x = 1;
	int y = 5;
	int i;
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	wattron(opt_win, COLOR_PAIR(1));
	box(opt_win, 0, 0);
	for (i = 0; i < n_options; ++i) {
		if (highlight == i + 1) {
			wattron(opt_win, COLOR_PAIR(2));
			mvwprintw(opt_win, x, y, "%s", options_choices[i]);
			wattroff(opt_win, COLOR_PAIR(2));
		}
		else {
			wattron(opt_win, COLOR_PAIR(1));
			mvwprintw(opt_win, x, y, "%s", options_choices[i]);
			wattroff(opt_win, COLOR_PAIR(1));
		}
		++x;
	}
	wrefresh(opt_win);
}
void print_set_map_size_options(WINDOW* map_size_win, int n_map_size_choices,int highlight) {
	int x = 2;
	int y = 5;
	int i;
	char* map_size_choices[] = {
		"Small (16 x 16)",
		"Medium (20 x 20)",
		"Big (24 x 24)",
	};
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	wattron(map_size_win, COLOR_PAIR(1));
	box(map_size_win, 0, 0);
	for (i = 0; i < n_map_size_choices; ++i) {
		if (highlight == i + 1) {
			wattron(map_size_win, COLOR_PAIR(2));
			mvwprintw(map_size_win, x, y, "%s", map_size_choices[i]);
			wattroff(map_size_win, COLOR_PAIR(2));
		}
		else {
			wattron(map_size_win, COLOR_PAIR(1));
			mvwprintw(map_size_win, x, y, "%s", map_size_choices[i]);
			wattroff(map_size_win, COLOR_PAIR(1));
		}
		++x;
	}
	wrefresh(map_size_win);
}
void set_map_size(OPTIONS* options_info) {
	char* map_size_choices[] = {
		"Small (16 x 16)",
		"Medium (20 x 20)",
		"Big (24 x 24)",
	};
	int n_map_size_choices = sizeof(map_size_choices) / sizeof(char*);
	WINDOW* map_size_win;
	int highlight = 1;
	int choice = 0;
	int c, startx, starty;

	initscr();
	resize_term(30, 50);
	clear();
	noecho();
	cbreak();
	start_color();
	curs_set(0);
	startx = (50 - WIDTH) / 2;
	starty = (30 - HEIGHT) / 2;

	map_size_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(map_size_win, TRUE);
	print_set_map_size_options(map_size_win, n_map_size_choices, highlight);
	while (1) {
		c = wgetch(map_size_win);
		switch (c) {
		case KEY_UP:
			if (highlight == 1)
				highlight = n_map_size_choices;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if (highlight == n_map_size_choices)
				highlight = 1;
			else
				++highlight;
			break;
		case KEY_ENTER:
			choice = highlight;
			break;
		default: break;
		}
		print_set_map_size_options(map_size_win, n_map_size_choices, highlight);
		if (choice != 0)
			break;
	}
	clrtoeol();
	refresh();
	endwin();
	if (choice == 1)
		options_info->map_size = 16;
	else if (choice == 2)
		options_info->map_size = 20;
	else
		options_info->map_size = 24;
}
void print_diff_options(WINDOW* diff_win, int n_diff_choices, int highlight) {
	int x = 2;
	int y = 5;
	int i;
	char* diff_choices[] = {
		"Easy",
		"Medium",
		"Hard",
	};
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	wattron(diff_win, COLOR_PAIR(1));
	box(diff_win, 0, 0);
	for (i = 0; i < n_diff_choices; ++i) {
		if (highlight == i + 1) {
			wattron(diff_win, COLOR_PAIR(2));
			mvwprintw(diff_win, x, y, "%s", diff_choices[i]);
			wattroff(diff_win, COLOR_PAIR(2));
		}
		else {
			wattron(diff_win, COLOR_PAIR(1));
			mvwprintw(diff_win, x, y, "%s", diff_choices[i]);
			wattroff(diff_win, COLOR_PAIR(1));
		}
		++x;
	}
	wrefresh(diff_win);
}
void set_diff(OPTIONS* options_info) {
	char* diff_choices[] = {
		"Easy",
		"Medium",
		"Hard",
	};
	int n_diff_choices = sizeof(diff_choices) / sizeof(char*);
	WINDOW* diff_win;
	int highlight = 1;
	int choice = 0;
	int c, startx, starty;

	initscr();
	resize_term(30, 50);
	clear();
	noecho();
	cbreak();
	start_color();
	curs_set(0);
	startx = (50 - WIDTH) / 2;
	starty = (30 - HEIGHT) / 2;

	diff_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(diff_win, TRUE);
	print_diff_options(diff_win, n_diff_choices, highlight);
	while (1) {
		c = wgetch(diff_win);
		switch (c) {
		case KEY_UP:
			if (highlight == 1)
				highlight = n_diff_choices;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if (highlight == n_diff_choices)
				highlight = 1;
			else
				++highlight;
			break;
		case KEY_ENTER:
			choice = highlight;
			break;
		default: break;
		}
		print_diff_options(diff_win, n_diff_choices, highlight);
		if (choice != 0)
			break;
	}
	clrtoeol();
	refresh();
	endwin();
	if (choice == 1)
		options_info->game_diff = EASY;
	else if (choice == 2)
		options_info->game_diff = MEDIUM;
	else
		options_info->game_diff = HARD;

}

void print_player_options(WINDOW* player_win, int n_player_choices, int highlight) {
	int x = 2;
	int y = 5;
	int i;
	char* player_choices[] = {
		"J",
		"M",
		"S",
	};
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	wattron(player_win, COLOR_PAIR(1));
	box(player_win, 0, 0);
	for (i = 0; i < n_player_choices; ++i) {
		if (highlight == i + 1) {
			wattron(player_win, COLOR_PAIR(2));
			mvwprintw(player_win, x, y, "%s", player_choices[i]);
			wattroff(player_win, COLOR_PAIR(2));
		}
		else {
			wattron(player_win, COLOR_PAIR(1));
			mvwprintw(player_win, x, y, "%s", player_choices[i]);
			wattroff(player_win, COLOR_PAIR(1));
		}
		++x;
	}
	wrefresh(player_win);
	
}
void set_player(OPTIONS* options_info) {
	char* player_choices[] = {
		"J",
		"M",
		"S",
	};
	int n_player_choices = sizeof(player_choices) / sizeof(char*);
	WINDOW* player_win;
	int highlight = 1;
	int choice = 0;
	int c, startx, starty;

	initscr();
	resize_term(30, 50);
	clear();
	noecho();
	cbreak();
	start_color();
	curs_set(0);
	startx = (50 - WIDTH) / 2;
	starty = (30 - HEIGHT) / 2;

	player_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(player_win, TRUE);
	print_player_options(player_win, n_player_choices, highlight);
	while (1) {
		c = wgetch(player_win);
		switch (c) {
		case KEY_UP:
			if (highlight == 1)
				highlight = n_player_choices;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if (highlight == n_player_choices)
				highlight = 1;
			else
				++highlight;
			break;
		case KEY_ENTER:
			choice = highlight;
			break;
		default: break;
		}
		print_player_options(player_win, n_player_choices, highlight);
		if (choice != 0)
			break;
	}
	clrtoeol();
	refresh();
	endwin();
	switch (choice) {
	case 1: options_info->player_emoticon = 'J'; break;
	case 2: options_info->player_emoticon = 'M'; break;
	case 3: options_info->player_emoticon = 'S'; break;
	}

}
void print_wall_options(WINDOW* wall_win, int n_wall_choices, int highlight) {
	int x = 2;
	int y = 5;
	int i;
	char* wall_choices[] = {
		"White",
		"Blue",
		"Green",
		"Black",
	};
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	wattron(wall_win, COLOR_PAIR(1));
	box(wall_win, 0, 0);
	for (i = 0; i < n_wall_choices; ++i) {
		if (highlight == i + 1) {
			wattron(wall_win, COLOR_PAIR(2));
			mvwprintw(wall_win, x, y, "%s", wall_choices[i]);
			wattroff(wall_win, COLOR_PAIR(2));
		}
		else {
			wattron(wall_win, COLOR_PAIR(1));
			mvwprintw(wall_win, x, y, "%s", wall_choices[i]);
			wattroff(wall_win, COLOR_PAIR(1));
		}
		++x;
	}
	wrefresh(wall_win);
}

void set_wall(OPTIONS* options_info) {
	char* wall_choices[] = {
		"White",
		"Blue",
		"Green",
		"Black",
	};
	int n_wall_choices = sizeof(wall_choices) / sizeof(char*);
	WINDOW* wall_win;
	int highlight = 1;
	int choice = 0;
	int c, startx, starty;

	initscr();
	resize_term(30, 50);
	clear();
	noecho();
	cbreak();
	start_color();
	curs_set(0);
	startx = (50 - WIDTH) / 2;
	starty = (30 - HEIGHT) / 2;

	wall_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(wall_win, TRUE);
	print_wall_options(wall_win, n_wall_choices, highlight);
	while (1) {
		c = wgetch(wall_win);
		switch (c) {
		case KEY_UP:
			if (highlight == 1)
				highlight = n_wall_choices;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if (highlight == n_wall_choices)
				highlight = 1;
			else
				++highlight;
			break;
		case KEY_ENTER:
			choice = highlight;
			break;
		default: break;
		}
		print_wall_options(wall_win, n_wall_choices, highlight);
		if (choice != 0)
			break;
	}
	clrtoeol();
	refresh();
	endwin();
	if (choice == 1) {
		options_info->wall_color = 1;
	}
	else if (choice == 2) {
		options_info->wall_color = 2;
	}
	else if (choice==3) {
		options_info->wall_color = 3;
	}
	else {
		options_info->wall_color = 4;
	}
}


void print_mummy_number(WINDOW* mummy_number_win, int n_mummy_number_choices, int highlight) {
	int x = 2;
	int y = 5;
	int i;
	char* mummy_number_choices[] = {
		"One",
		"Two",
	};
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	wattron(mummy_number_win, COLOR_PAIR(1));
	box(mummy_number_win, 0, 0);
	for (i = 0; i < n_mummy_number_choices; ++i) {
		if (highlight == i + 1) {
			wattron(mummy_number_win, COLOR_PAIR(2));
			mvwprintw(mummy_number_win, x, y, "%s", mummy_number_choices[i]);
			wattroff(mummy_number_win, COLOR_PAIR(2));
		}
		else {
			wattron(mummy_number_win, COLOR_PAIR(1));
			mvwprintw(mummy_number_win, x, y, "%s", mummy_number_choices[i]);
			wattroff(mummy_number_win, COLOR_PAIR(1));
		}
		++x;
	}
	wrefresh(mummy_number_win);
}
void set_mummy_number(OPTIONS* options_info) {
	char* mummy_number_choices[] = {
		"One",
		"Two",
	};
	int n_mummy_number_choices = sizeof(mummy_number_choices) / sizeof(char*);
	WINDOW* mummy_number_win;
	int highlight = 1;
	int choice = 0;
	int c, startx, starty;

	initscr();
	resize_term(30, 50);
	clear();
	noecho();
	cbreak();
	start_color();
	curs_set(0);
	startx = (50 - WIDTH) / 2;
	starty = (30 - HEIGHT) / 2;

	mummy_number_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(mummy_number_win, TRUE);
	print_mummy_number(mummy_number_win, n_mummy_number_choices, highlight);
	while (1) {
		c = wgetch(mummy_number_win);
		switch (c) {
		case KEY_UP:
			if (highlight == 1)
				highlight = n_mummy_number_choices;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if (highlight == n_mummy_number_choices)
				highlight = 1;
			else
				++highlight;
			break;
		case KEY_ENTER:
			choice = highlight;
			break;
		default: break;
		}
		print_mummy_number(mummy_number_win, n_mummy_number_choices, highlight);
		if (choice != 0)
			break;
	}
	clrtoeol();
	refresh();
	endwin();
	if (choice == 1) {
		options_info->mummy_number = 1;
	}
	else if (choice == 2) {
		options_info->mummy_number = 2;
	}
}

void print_music(WINDOW* music_win, int n_music_choices, int highlight) {
	int x = 2;
	int y = 5;
	int i;
	char* music_choices[] = {
		"Brave",
		"Spooky",
		"None",
	};
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	wattron(music_win, COLOR_PAIR(1));
	box(music_win, 0, 0);
	for (i = 0; i < n_music_choices; ++i) {
		if (highlight == i + 1) {
			wattron(music_win, COLOR_PAIR(2));
			mvwprintw(music_win, x, y, "%s", music_choices[i]);
			wattroff(music_win, COLOR_PAIR(2));
		}
		else {
			wattron(music_win, COLOR_PAIR(1));
			mvwprintw(music_win, x, y, "%s", music_choices[i]);
			wattroff(music_win, COLOR_PAIR(1));
		}
		++x;
	}
	wrefresh(music_win);
}
void set_music() {
	char* music_choices[] = {
		"Brave",
		"Spooky",
		"None",
	};
	int n_music_choices = sizeof(music_choices) / sizeof(char*);
	WINDOW* music_win;
	int highlight = 1;
	int choice = 0;
	int c, startx, starty;

	initscr();
	resize_term(30, 50);
	clear();
	noecho();
	cbreak();
	start_color();
	curs_set(0);
	startx = (50 - WIDTH) / 2;
	starty = (30 - HEIGHT) / 2;

	music_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(music_win, TRUE);
	print_music(music_win, n_music_choices, highlight);
	while (1) {
		c = wgetch(music_win);
		switch (c) {
		case KEY_UP:
			if (highlight == 1)
				highlight = n_music_choices;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if (highlight == n_music_choices)
				highlight = 1;
			else
				++highlight;
			break;
		case KEY_ENTER:
			choice = highlight;
			break;
		case 'p':
			choice = 11;
			break;
		default: break;
		}
		print_music(music_win, n_music_choices, highlight);
		if (choice != 0 || choice==11)
			break;
	}
	clrtoeol();
	refresh();
	endwin();
	if (choice == 1) {
		PlaySound(TEXT("brave.wav"), NULL, SND_ASYNC);
	}
	else if (choice == 2) {
		PlaySound(TEXT("spooky.wav"), NULL, SND_ASYNC);
	}
	else if (choice == 3) {
		PlaySound(NULL, NULL, SND_ASYNC);
	}
	
}

void show_settings(OPTIONS* options_info) {
	char c;
	int x, y, z;
	initscr();
	resize_term(30, 50);
	noecho();
	clear();
	start_color();
	curs_set(0);
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	char *map_sizes[] = {
		"SMALL",
		"MEDIUM",
		"BIG"
	};
	char* game_diffs[] = {
		"Easy",
		"Medium",
		"Hard"
	};
	char* wall_colors[] = {
		"White",
		"Blue",
		"Green",
		"Black"
	};

	if (options_info->map_size == 16) {
		x = 0;
	}
	else if (options_info->map_size == 20) {
		x = 1;
	}
	else {
		x = 2;
	}

	if (options_info->game_diff == EASY) {
		y = 0;
	}
	else if (options_info->game_diff == MEDIUM) {
		y = 1;
	}
	else {
		y = 2;
	}
	if (options_info->wall_color == 1) {
		z = 0;
	}
	else if (options_info->wall_color == 2) {
		z = 1;
	}
	else if (options_info->wall_color==3) {
		z = 2;
	}
	else {
		z = 3;
	}
	while (1) {
		wattron(stdscr, COLOR_PAIR(1));
		mvwprintw(stdscr, 5, 5, "Map Size: %s", map_sizes[x]);
		mvwprintw(stdscr, 6, 5, "Game Difficulty: %s", game_diffs[y]);
		mvwprintw(stdscr, 7, 5, "Player Emoticon: %c", options_info->player_emoticon);
		mvwprintw(stdscr, 8, 5, "Wall color: %s", wall_colors[z]);
		mvwprintw(stdscr, 9, 5, "Back-Esc");
		wattroff(stdscr, COLOR_PAIR(1));
		c = getch();
		if (c == KEY_ESC)
			break;
	}
	endwin();
}
void options_main(OPTIONS* options_info) {
	int choice = 1;
	while (choice!=n_options) {
		choice=options_menu(options_info);
		switch (choice) {
		case 1:set_map_size(options_info); break;
		case 2:set_diff(options_info); break;
		case 3:set_mummy_number(options_info); break;
		case 4:set_player(options_info); break;
		case 5:set_wall(options_info); break;
		case 6:show_settings(options_info); break;
		case 7:set_music(); break;
		}
	}
}
