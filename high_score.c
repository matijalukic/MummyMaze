#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curses.h"

#define MAX_NAME 30
#define WIDTH 30
#define HEIGHT 11
#define KEY_ENTER 10
#define KEY_ESC 27

typedef struct high_score {
	char player_name[30];
	int score;
}high_score;

void make_high_score_table(FILE* high_scores) {
	long int end_point;
	high_score player;
	int i;
	char check = 0;

	for (i = 0; i < 10; i++) {
		fseek(high_scores, i*sizeof(player), SEEK_SET);
		strcpy(player.player_name, "Anonymous");
		player.score = 0;
		fwrite(&player, sizeof(player), 1, high_scores);
		rewind(high_scores);
	}

	fseek(high_scores, 0L, SEEK_END);
	end_point = ftell(high_scores);
	rewind(high_scores);

	for (i = 0; i < end_point; i++) {
		char help;
		fread(&help, sizeof(help), 1, high_scores);
		check ^= help;
	}
	fwrite(&check, sizeof(check), 1, high_scores);

}

int check_for_cheaters(FILE* high_scores) {
	char check = 0;
	long int end_point;
	long int i;

	fseek(high_scores, 0L, SEEK_END);
	end_point = ftell(high_scores);
	rewind(high_scores);

	for (i = 0; i < end_point; i++) {
		char help;
		fread(&help, sizeof(help), 1, high_scores);
		check ^= help;
	}

	return check;
}

int update_high_scores(int score) {
	int check_cheats;
	int i, j;
	FILE* high_scores;
	high_score player;
	char flag = 0;
	long int end_point;
	char check = 0;
	char ulaz[] = { "HIGH SCORE" };

	if ((fopen_s(&high_scores, ulaz, "r+b"))) {
		fopen_s(&high_scores, ulaz, "w+b");
		make_high_score_table(high_scores);
		check_cheats = 0;
	}


	else check_cheats = check_for_cheaters(high_scores);

	if (check_cheats) {
		fclose(high_scores);
		fopen_s(&high_scores, ulaz, "w+b");
		make_high_score_table(high_scores);
		check_cheats = 0;
	}

	fclose(high_scores);
	fopen_s(&high_scores, ulaz, "r+b");

	fseek(high_scores, 0L, SEEK_SET);

	for (i = 0; i < 10; i++) {			//proveravamo da li uposte ima potrebe da menjamo list high score-eva
		fread(&player, sizeof(player), 1, high_scores);
		if (score > player.score) {
			flag = 1;
			break;
		}
	}

	rewind(high_scores);
	if (flag) {

		for (j = 9; j > i; j--) {

			fseek(high_scores, (j - 1) * sizeof(player), SEEK_SET);
			fread(&player, sizeof(player), 1, high_scores);
			fseek(high_scores, (j) * sizeof(player), SEEK_SET);
			fwrite(&player, sizeof(player), 1, high_scores);
			rewind(high_scores);
		}

		/*initscr();
		start_color();

		noecho();
		curs_set(0);*/

		init_pair(1, COLOR_RED, COLOR_BLACK);
		init_pair(2, COLOR_WHITE, COLOR_BLACK);
		wattron(stdscr, COLOR_PAIR(2));
		mvwprintw(stdscr, 7, 2, "Enter your name : \n");
		wrefresh(stdscr);
		wattroff(stdscr, COLOR_PAIR(2));

		char c;
		j = 0;
		while ((c = getch()) != KEY_ENTER) {
			if (c != 8) {
				player.player_name[j++] = c;
				mvwprintw(stdscr, 7, 19 + j, "%c_", c);
			}
			else if (c == 8 && j >= 1) {
				mvwprintw(stdscr, 7, 19 + j + 1, " ");
				mvwprintw(stdscr, 7, 19 + j--, "_");
				player.player_name[j] = ' ';
			}
		}
		player.player_name[j] = '\0';
		wrefresh(stdscr);


		clrtoeol();
		endwin();
		//ovde mora lepse ovo da se odradi
		//scanf(" %s", player.player_name);
		player.score = score;

		fseek(high_scores, (i) * sizeof(player), SEEK_SET);
		fwrite(&player, sizeof(player), 1, high_scores);
		fseek(high_scores, 0L, SEEK_END);
		end_point = ftell(high_scores);
		rewind(high_scores);

		for (long i = 0; i < end_point; i++) {
			char help;

			fread(&help, sizeof(help), 1, high_scores);
			check ^= help;

		}

		fclose(high_scores);
		fopen_s(&high_scores, ulaz, "a+b");
		fwrite(&check, sizeof(check), 1, high_scores);
	}

	fclose(high_scores);
	return flag;

}

void write_high_scores() {
	int i = 0;
	FILE *high_scores;
	high_score player;
	char c;
	int check_cheats;


	initscr();
	resize_term(27, 35);
	start_color();

	noecho();
	curs_set(0);

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	fopen_s(&high_scores, "HIGH SCORE", "r+b");

	check_cheats = check_for_cheaters(high_scores);

	if (check_cheats) {
		fclose(high_scores);
		fopen_s(&high_scores, "HIGH SCORE", "w+b");
		make_high_score_table(high_scores);
		check_cheats = 0;
	}

	rewind(high_scores);

	fread(&player, sizeof(high_score), 1, high_scores);
	for (i = 0; i < 10; i++) {
		if (strcmp(player.player_name, "Anonymous") != 0) {
			mvwprintw(stdscr, i + 1, 4, "%s %d\n", player.player_name, player.score);
			fread(&player, sizeof(high_score), 1, high_scores);
		}
		mvwprintw(stdscr, i + 1, 1, "%d.", i + 1);

	}
	mvwprintw(stdscr, i + 2, 1, "R-Reset Esc-Back \n");


	while ((c = getch()) != KEY_ESC ){
		if (c == 'R' || c == 'r') {
			make_high_score_table(high_scores);
			wattron(stdscr, COLOR_PAIR(1));
			mvwprintw(stdscr, i + 2, 1, "R-Reset");
			wattroff(stdscr, COLOR_PAIR(1));
			for (i = 0; i < 10; i++) {
				mvwprintw(stdscr, i + 1, 1, "%d.                                               ", i + 1);

			}
			
		}
		else  {
			wattron(stdscr, COLOR_PAIR(2));
			mvwprintw(stdscr, i + 2, 1, "R-Reset");
			wattroff(stdscr, COLOR_PAIR(2));
		}
		
	}
	fclose(high_scores);
	clrtoeol();
	endwin();

	//treba staviti da ostane ekran

	

}