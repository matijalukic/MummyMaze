#ifndef MAP_H_
#define MAP_H_
#define MUMMY -1
#define WALL 1
#define HUMAN -2
#define PATH 0
#define DOOR -3
#define MAX 100
#define MUMMY_TURN 2
#include <curses.h>

typedef struct MAP {
	int map_matrix[MAX][MAX];
	int map_size;
	int next_char;
	int hit;
	int load;
	int minutes_played;
	int seconds_played;
	int player_emoticon;
	int wall_color;
	int game_level;
	int mummy_number;
	int alg;
	int draw;
}MAP;
typedef struct PLAYER {
	int x_temp;
	int y_temp;
	int x_prev;
	int y_prev;
	int number_of_moves;
}PLAYER;
typedef struct OPTIONS {
	int map_size;
	int game_level;
	int mummy_number;
	int player_emoticon;
	int wall_color;
}OPTIONS;
typedef struct dot {
	int x, y;
} Dot;


void print_map(WINDOW*, MAP*);
void init_map(MAP*);
void find_player(MAP*, PLAYER*);
void player_movement(MAP*, PLAYER*);
void gameplay(MAP*, PLAYER*);


#endif