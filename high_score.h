#pragma once

#define MAX_NAME 30

typedef struct high_score {
	char player_name[30];
	int score;
}high_score;

void make_high_score_table(FILE*);
int check_for_cheaters(FILE*);
int update_high_scores(int score);
void write_high_scores();
