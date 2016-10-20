#include <stdio.h>
#include <curses.h>

#define WIDTH 75
#define HEIGHT 34
#define KEY_ENTER 10


char *choices[] = {
	"New Game",
	"Continue",
	"Load Game",
	"High Scores",
	"Options",
	"Help",
	"About",
	"Authors",
	"Exit"
};
int n = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *menu_win, int highlight);

int menu() {
	WINDOW *menu_win;

	int highlight = 1;
	int choice = 0;
	int c, startx, starty, i = 0;
	initscr();
	resize_term(34, 75);
	clear();
	noecho();
	cbreak();
	start_color();
	curs_set(0);
	
	//startx = (80 - WIDTH) / 2;
	//starty = (40 - HEIGHT) / 2;

	menu_win = newwin(HEIGHT, WIDTH, 0, 0);
	keypad(menu_win, TRUE);
	refresh();
	
	print_menu(menu_win, highlight);
	while (1){
		c = wgetch(menu_win);
		switch (c){
			case KEY_UP:
				if (highlight == 1)
					highlight = n;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if (highlight == n)
					highlight = 1;
				else
					++highlight;
				break;
			case KEY_ENTER:
				choice = highlight;
				break;
			default: break;
		}
		print_menu(menu_win, highlight);
		if (choice != 0)	
			break;
	}
	clrtoeol();
	endwin(menu_win);
	return choice;
}


void print_menu(WINDOW *menu_win, int highlight){
	int x, y, i = 0;
	x = 16;
	y = 4;
	
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	wattron(menu_win,COLOR_PAIR(1));
	FILE* text = fopen("text.txt", "r"), *picture = fopen("pyramid.txt", "r");
	char linija[250];
	i = 0;
	while (feof(text) == 0) {
		fgets(linija, 250, text);
		mvwprintw(menu_win, (i++) , 6, "%s", linija);
	}
	fclose(text);
	//box(menu_win, 0, 0);
	for (i = 0; i < n; ++i){
		if (highlight == i + 1) {
			wattron(menu_win, COLOR_PAIR(2));
			mvwprintw(menu_win, x, y, "%s", choices[i]);
			wattroff(menu_win, COLOR_PAIR(2));
		}
		else {
			wattron(menu_win, COLOR_PAIR(1));
			mvwprintw(menu_win, x, y, "%s", choices[i]);
			wattroff(menu_win, COLOR_PAIR(1));
		}
		++x;
	} 
	while (feof(picture) == 0) {
		fgets(linija, 250, picture);
		mvwprintw(menu_win, (i++) , 21, "%s", linija);
	}
	fclose(picture);
	
	wrefresh(menu_win);
}