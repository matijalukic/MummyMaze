#include <stdio.h>
#include "curses.h"
#define HEIGHT 27
#define WIDTH 35
void print_picture() {

	WINDOW *picture_win;
	FILE* picture;
	int i, x, y;

	initscr();
	resize_term(27, 35);
	start_color();

	noecho();
	curs_set(0);

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	for (i = 0; i < 790; i++) {
		if (i % 2 == 1) {
			picture = fopen("picture.txt", "r");
		}
		else
			picture = fopen("picture2.txt", "r");
		char c = fgetc(picture);
		x = 0;
		y = 0;
		while (c != EOF) {
			if (c == '\n') {
				y = 0;
				x++;
				c = fgetc(picture);
			}
			if (c >= 48 && c <= 57) {
				wattron(stdscr, COLOR_PAIR(1));
				mvwprintw(stdscr, x, y, "%c", c);
				wattroff(stdscr, COLOR_PAIR(1));
				y++;

			}
			else {
				wattron(stdscr, COLOR_PAIR(2));
				mvwprintw(stdscr, x, y, "%c", c);
				wattroff(stdscr, COLOR_PAIR(2));
				y++;
			}
			c = fgetc(picture);
		}
		wattron(stdscr, COLOR_PAIR(1));
		if (i % 3 == 0) {
			mvwprintw(stdscr, 6, 13, "LOADING.");
		}
		else if (i % 3 == 1) {
			mvwprintw(stdscr, 6, 13, "LOADING..");
		}
		else
			mvwprintw(stdscr, 6, 13, "LOADING...");
		wattroff(stdscr, COLOR_PAIR(1));
		fclose(picture);
		wrefresh(stdscr);
		int i;
		for (i = 0; i < 4000000; i++);
		refresh();
	}
	clrtoeol();
	endwin();

}