#include <stdio.h>
#include <curses.h>
#include "map.h"
#define WIDTH 30
#define HEIGHT 7
#define KEY_ESC 27
#define KEY_ENTER 10
void print_alg_options(WINDOW* alg_win, int n_alg_choices, int highlight) {
	int x = 2;
	int y = 5;
	int i;
	char* alg_choices[] = {
		"Kruskal",
		"Recursive Divison"
	};
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	wattron(alg_win, COLOR_PAIR(1));
	box(alg_win, 0, 0);
	for (i = 0; i < n_alg_choices; ++i) {
		if (highlight == i + 1) {
			wattron(alg_win, COLOR_PAIR(2));
			mvwprintw(alg_win, x, y, "%s", alg_choices[i]);
			wattroff(alg_win, COLOR_PAIR(2));
		}
		else {
			wattron(alg_win, COLOR_PAIR(1));
			mvwprintw(alg_win, x, y, "%s", alg_choices[i]);
			wattroff(alg_win, COLOR_PAIR(1));
		}
		++x;
	}

	wrefresh(alg_win);
}
void set_alg(MAP* map_info) {
	char* alg_choices[] = {
		"Kruskal",
		"Recursive Division"
	};
	int n_alg_choices = sizeof(alg_choices) / sizeof(char*);
	WINDOW* alg_win;
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

	alg_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(alg_win, TRUE);
	print_alg_options(alg_win, n_alg_choices, highlight);
	while (1) {
		c = wgetch(alg_win);
		switch (c) {
		case KEY_UP:
			if (highlight == 1)
				highlight = n_alg_choices;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if (highlight == n_alg_choices)
				highlight = 1;
			else
				++highlight;
			break;
		case KEY_ENTER:
			choice = highlight;
			break;
		default: break;
		}
		print_alg_options(alg_win, n_alg_choices, highlight);
		if (choice != 0)
			break;
	}
	clrtoeol();
	refresh();
	endwin();
	switch (choice) {
	case 1: map_info->alg = 1; break;
	case 2: map_info->alg = 2; break;
	}

}
void print_draw_options(WINDOW* draw_win, int n_draw_choices, int highlight) {
	int x = 2;
	int y = 5;
	int i;
	char* draw_choices[] = {
		"Show me the algorithm",
		"Just let me play"
	};
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	wattron(draw_win, COLOR_PAIR(1));
	box(draw_win, 0, 0);
	for (i = 0; i < n_draw_choices; ++i) {
		if (highlight == i + 1) {
			wattron(draw_win, COLOR_PAIR(2));
			mvwprintw(draw_win, x, y, "%s", draw_choices[i]);
			wattroff(draw_win, COLOR_PAIR(2));
		}
		else {
			wattron(draw_win, COLOR_PAIR(1));
			mvwprintw(draw_win, x, y, "%s", draw_choices[i]);
			wattroff(draw_win, COLOR_PAIR(1));
		}
		++x;
	}

	wrefresh(draw_win);
}
void set_draw(MAP* map_info) {
	char* draw_choices[] = {
		"Show me the algorithm",
		"Just let me play"
	};
	int n_draw_choices = sizeof(draw_choices) / sizeof(char*);
	WINDOW* draw_win;
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

	draw_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(draw_win, TRUE);
	print_draw_options(draw_win, n_draw_choices, highlight);
	while (1) {
		c = wgetch(draw_win);
		switch (c) {
		case KEY_UP:
			if (highlight == 1)
				highlight = n_draw_choices;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if (highlight == n_draw_choices)
				highlight = 1;
			else
				++highlight;
			break;
		case KEY_ENTER:
			choice = highlight;
			break;
		default: break;
		}
		print_draw_options(draw_win, n_draw_choices, highlight);
		if (choice != 0)
			break;
	}
	clrtoeol();
	refresh();
	endwin();
	switch (choice) {
	case 1: map_info->draw = 1; break;
	case 2: map_info->draw = 0; break;
	}

}
