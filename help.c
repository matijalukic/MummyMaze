#include <stdio.h>
#include <curses.h>
#define KEY_ESC 27
void help() {
	char c;
	initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	while (1) {
		noecho();
		curs_set(0);
		attron(COLOR_PAIR(1));
		mvwprintw(stdscr, 5, 2, "Use arrows to move around the maze...");
		mvwprintw(stdscr, 6, 2, "Try to realise how mummy moves !");
		mvwprintw(stdscr, 7, 2, "If you ever get stuck, just press H ;)");
		mvwprintw(stdscr, 8, 2, "Good luck and have fun !");
		mvwprintw(stdscr, 10, 5, "Esc-Back");
		attroff(COLOR_PAIR(1));
		c = getch();
		if (c == KEY_ESC)
			break;
		refresh();
	}
	endwin();
}