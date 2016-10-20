#include <stdio.h>
#include <curses.h>
#define KEY_ESC 27
void about_game() {
	char c;
	initscr();
	resize_term(30, 76);
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	while (1) {
		noecho();
		curs_set(0);
		attron(COLOR_PAIR(1));
		mvwprintw(stdscr, 5, 2, "The original version of Mummy Maze has been published by PopCap Games.");
		mvwprintw(stdscr, 6, 2, "Game rules are pretty simple, avoid the mummy and escape from the maze!");
		mvwprintw(stdscr, 7, 2, "This game has been made, as the homework assignment");
		mvwprintw(stdscr, 8, 2, "for subject Programming Practicum 2, by The Sloths");
		mvwprintw(stdscr, 11, 5, "June 2016.");
		mvwprintw(stdscr, 13, 5, "Esc-Back");
		attroff(COLOR_PAIR(1));
		c = getch();
		if (c == KEY_ESC)
			break;
		refresh();
	}
	endwin();
}