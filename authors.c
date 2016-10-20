#include <stdio.h>
#include <curses.h>
#define KEY_ESC 27

void authors() {
	char c;
	initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	while (1) {
		noecho();
		curs_set(0);
		attron(COLOR_PAIR(1));
		mvwprintw(stdscr, 5, 5, "Jakov Jezdic");
		mvwprintw(stdscr, 6, 5, "Matija Lukic");
		mvwprintw(stdscr, 7, 5, "Stefan Matic");
		mvwprintw(stdscr, 8, 5, "Mentor : Milan Ilic");
		mvwprintw(stdscr, 10, 5, "Back-Esc");

		attroff(COLOR_PAIR(1));
		c = getch();
		if (c == KEY_ESC)
			break;
		refresh();
	}
	endwin();
}