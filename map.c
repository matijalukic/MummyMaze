#pragma comment(lib, "winmm.lib")
#include <windows.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <stdlib.h>

#include "high_score.h"



#define MUMMY -1
#define WALL 1
#define HUMAN -2
#define PATH 0
#define DOOR -3
#define MAX 100
#define HEIGHT 30
#define WIDTH 50
#define KEY_ESC 27
#define KEY_ENTER 10
#define MAX_MATRIX 24
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
typedef struct easy_mummy {
	int x_temp;
	int y_temp;

} MUMMY_COORDINATES;
typedef struct dot {
	int x, y;
} Dot;
typedef struct map {
	int cell;
	int visit;
}copy_map;
typedef struct coordinate {
	int x, y;
	int new_value;
	struct coordinate *next;
}coordinate;

void load_game(MAP*, PLAYER*);
void save_game(MAP*, PLAYER*);
void save_current_game(MAP*, PLAYER*);
void mummy_harder(WINDOW* map_win, MAP* map_info, PLAYER* player_info, int level);
void mummy_move_harder_mode(MAP* map_info, MUMMY_COORDINATES*mummy_info, int x_next, int y_next, copy_map copy_matrix[MAX_MATRIX][MAX_MATRIX]);
void mummy_movement(WINDOW*, MAP*);
Dot place_exit(MAP **);
Dot **generate_map(MAP **, int, int *);
Dot **extend_map(MAP **, Dot **, int, int);
void make_maze(MAP **);
Dot place_player(MAP **, Dot, int, int);


void print_map(WINDOW* map_win, MAP* map_info) {
	int i, j, k = 0;
	if (map_info->wall_color == 1) {
		init_pair(1, COLOR_RED, COLOR_WHITE);
	}
	else if (map_info->wall_color == 2) {
		init_pair(1, COLOR_BLUE, COLOR_BLUE);
	}
	else if (map_info->wall_color == 3) {
		init_pair(1, COLOR_GREEN, COLOR_GREEN);
	}
	else
		init_pair(1, COLOR_BLACK, COLOR_BLACK);

	init_pair(2, COLOR_GREEN, COLOR_YELLOW);
	init_pair(3, COLOR_RED, COLOR_YELLOW);
	init_pair(7, COLOR_WHITE, COLOR_YELLOW);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(8, COLOR_GREEN, COLOR_GREEN);
	for (i = 0; i < map_info->map_size; i++) {
		for (j = 0; j < map_info->map_size; j++) {
			if (map_info->map_matrix[i][j] == WALL) {
				wattron(map_win, COLOR_PAIR(1));
				mvwprintw(map_win, i, j, " ");
				wattroff(map_win, COLOR_PAIR(1));
				wrefresh(map_win);
			}
			else if (map_info->map_matrix[i][j] == HUMAN) {
				wattron(map_win, COLOR_PAIR(2));
				wattron(map_win, A_BOLD);
				mvwprintw(map_win, i, j, "%c", map_info->player_emoticon);
				wattroff(map_win, A_BOLD);
				wattroff(map_win, COLOR_PAIR(2));
				wrefresh(map_win);
			}
			else if (map_info->map_matrix[i][j] == MUMMY) {
				wattron(map_win, COLOR_PAIR(3));
				wattron(map_win, A_BOLD);
				mvwprintw(map_win, i, j, "M");
				wattroff(map_win, A_BOLD);
				wattroff(map_win, COLOR_PAIR(3));
				wrefresh(map_win);
			}
			else if (map_info->map_matrix[i][j] == DOOR) {
				wattron(map_win, COLOR_PAIR(7));
				wattron(map_win, A_BOLD);
				mvwprintw(map_win, i , j , "^");
				wattroff(map_win, A_BOLD);
				wattroff(map_win, COLOR_PAIR(7));
				wrefresh(map_win);
			}
			else if (map_info->map_matrix[i][j] == 'H') {
				wattron(map_win, COLOR_PAIR(8));
				wattron(map_win, A_BOLD);
				mvwprintw(map_win, i, j, " ");
				wattroff(map_win, A_BOLD);
				wattroff(map_win, COLOR_PAIR(8));
				wrefresh(map_win);
			}
			else {
				wattron(map_win, COLOR_PAIR(6));
				wattron(map_win, A_BOLD);
				mvwprintw(map_win, i , j, " ");
				wattroff(map_win, A_BOLD);
				wattroff(map_win, COLOR_PAIR(6));
				wrefresh(map_win);
			}
		}
		wprintw(map_win, "\n");
	}
	wattron(map_win, COLOR_PAIR(5));
	mvwprintw(map_win, map_info->map_size+1, 2, "S-Save\n");
	mvwprintw(map_win, map_info->map_size + 2, 2, "H-Hint\n");
	mvwprintw(map_win, map_info->map_size + 3, 2, "Esc-Back\n");
	wattroff(map_win, COLOR_PAIR(5));
	wrefresh(map_win);
}

Dot init_map(MAP* map_info) {  //za sada hardkodovana mapa
	Dot **sets, exit, player, mummy, mummy2; // belezenje skupova
	int num_sets, i;
	if (map_info->alg == 1)
	{
		sets = generate_map(&map_info, PATH, &num_sets); // generisemo pocetna polja
		extend_map(&map_info, sets, PATH, num_sets);		
	}
	else if (map_info->alg == 2) {
		make_maze(&map_info);
	}


	// postavljamo igrace
	exit = place_exit(&map_info);
	
	if (map_info->mummy_number > 1) // ako ima 2 mumije stavljamo na pola puta
		player = place_player(&map_info, exit, map_info->map_size / 2, HUMAN); // postavljamo HUMANA
	else
		player = place_player(&map_info, exit, map_info->map_size, HUMAN);

	mummy = place_player(&map_info, player, 2*map_info->map_size, MUMMY); // postavljamo MUMMY
	if(map_info->mummy_number > 1)
		mummy2 = place_player(&map_info, player, map_info->map_size, MUMMY);


	return exit;
}

void find_player(MAP* map_info, PLAYER* player_info) {
	int i, j, go_out = 0;
	for (i = 0; i < map_info->map_size; i++) {
		for (j = 0; j < map_info->map_size; j++) {
			if (map_info->map_matrix[i][j] == HUMAN) {
				player_info->x_temp = i;
				player_info->y_temp = j;
				go_out = 1;
				break;
			}
		}
		if (go_out)
			break;
	}
}

void player_movement(MAP* map_info, PLAYER* player_info) {
	map_info->hit = 0;

	find_player(map_info, player_info);
	player_info->x_prev = player_info->x_temp;
	player_info->x_prev = player_info->x_temp;
	switch (map_info->next_char) {
	case KEY_UP: if (map_info->map_matrix[player_info->x_temp - 1][player_info->y_temp] == PATH) { // if : ako igrac prelazi na slobodno polje
		map_info->map_matrix[player_info->x_temp - 1][player_info->y_temp] = HUMAN;				// else if : ako udara u mumiju
		map_info->map_matrix[player_info->x_temp][player_info->y_temp] = PATH;
		(player_info->number_of_moves)++;
		
		player_info->x_temp = player_info->x_temp - 1;
	}																							// else if : ako udara u zid
				 else if (map_info->map_matrix[player_info->x_temp - 1][player_info->y_temp] == MUMMY)
					 map_info->hit = MUMMY;
				 else if (map_info->map_matrix[player_info->x_temp - 1][player_info->y_temp] == DOOR) {
					 (player_info->number_of_moves)++;
					 map_info->hit = DOOR;
				 }
				 else if (map_info->map_matrix[player_info->x_temp - 1][player_info->y_temp] == WALL)
					 map_info->hit = WALL;
		break;
	case KEY_LEFT: if (map_info->map_matrix[player_info->x_temp][player_info->y_temp - 1] == PATH) {
		map_info->map_matrix[player_info->x_temp][player_info->y_temp - 1] = HUMAN;
		map_info->map_matrix[player_info->x_temp][player_info->y_temp] = PATH;
		(player_info->number_of_moves)++;  //nakon svakog poteza uvecava broj poteza za 1, za HIGH SCORE potrebno
		player_info->y_temp = player_info->y_temp - 1;
	}
				   else if (map_info->map_matrix[player_info->x_temp][player_info->y_temp - 1] == MUMMY)
					   map_info->hit = MUMMY;
				   else if (map_info->map_matrix[player_info->x_temp][player_info->y_temp - 1] == DOOR) {
					   (player_info->number_of_moves)++;
					   map_info->hit = DOOR;
				   }
				   else if (map_info->map_matrix[player_info->x_temp][player_info->y_temp - 1] == WALL)
					   map_info->hit = WALL;
		break;
	case KEY_DOWN: if (map_info->map_matrix[player_info->x_temp + 1][player_info->y_temp] == PATH) {
		map_info->map_matrix[player_info->x_temp + 1][player_info->y_temp] = HUMAN;
		map_info->map_matrix[player_info->x_temp][player_info->y_temp] = PATH;
		(player_info->number_of_moves)++;  //nakon svakog poteza uvecava broj poteza za 1, za HIGH SCORE potrebno
		player_info->x_temp = player_info->x_temp + 1;
	}
				   else if (map_info->map_matrix[player_info->x_temp + 1][player_info->y_temp] == MUMMY)
					   map_info->hit = MUMMY;
				   else if (map_info->map_matrix[player_info->x_temp + 1][player_info->y_temp] == DOOR) {
					   (player_info->number_of_moves)++;
					   map_info->hit = DOOR;
				   }
				   else if (map_info->map_matrix[player_info->x_temp + 1][player_info->y_temp] == WALL)
					   map_info->hit = WALL;
		break;
	case KEY_RIGHT: if (map_info->map_matrix[player_info->x_temp][player_info->y_temp + 1] == PATH) {
		map_info->map_matrix[player_info->x_temp][player_info->y_temp + 1] = HUMAN;
		map_info->map_matrix[player_info->x_temp][player_info->y_temp] = PATH;
		(player_info->number_of_moves)++;  //nakon svakog poteza uvecava broj poteza za 1, za HIGH SCORE potrebno
		player_info->y_temp = player_info->y_temp + 1;
	}
					else if (map_info->map_matrix[player_info->x_temp][player_info->y_temp + 1] == MUMMY)
						map_info->hit = MUMMY;
					else if (map_info->map_matrix[player_info->x_temp][player_info->y_temp + 1] == DOOR) {
						map_info->hit = DOOR;
						(player_info->number_of_moves)++;
					}
					else if (map_info->map_matrix[player_info->x_temp][player_info->y_temp + 1] == WALL)
						map_info->hit = WALL;
		break;
	default: break;
	}
}
void gameplay(MAP* map_info, PLAYER* player_info) {//koristi sve funkcije iz ovog c fajla i realizuje igru
	int minutes, seconds, sec, i;
	Dot exit;
	int startx = 0, starty = 0;
	clock_t start, end, temp_end;
	WINDOW *map_win;
	int level;
	int high_score;
	int flag = 0;
	initscr();
	clear();
	noecho();
	cbreak();
	start_color();
	curs_set(0);

	map_win = newwin(HEIGHT, WIDTH, 0, 0);
	resize_term(30, 50);
	keypad(map_win, TRUE);
	refresh();
	init_pair(1, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(6, COLOR_GREEN, COLOR_GREEN);
	if (map_info->load == 0) //mapa se generise samo ako se ne koristi ucitana mapa
		exit = init_map(map_info);
	print_map(map_win, map_info);

	if (map_info->load == 0) {
		player_info->number_of_moves = 0;		//koraci se resetuju ako se ne ucitava partija 
	}
	level = map_info->game_level;

	start = clock();
	while (1) { //zavrsava se u slucaju pobede,poraza ili ESC
		map_info->next_char = wgetch(map_win);

		if (map_info->next_char == KEY_ESC)
			break;
		else if (map_info->next_char == 's' || map_info->next_char == 'S') {
			temp_end = clock();
			seconds = (int)(temp_end - start) / CLOCKS_PER_SEC;
			map_info->minutes_played += seconds / 60;
			map_info->seconds_played += seconds % 60;
			save_game(map_info, player_info);
			init_pair(5, COLOR_WHITE, COLOR_BLACK);
			init_pair(4, COLOR_RED, COLOR_BLACK);
			attron(COLOR_PAIR(5));
			mvwprintw(map_win, map_info->map_size+1, 2, "S-Save\n");
			attroff(COLOR_PAIR(5));
			wattron(map_win, COLOR_PAIR(4));
			mvwprintw(map_win, map_info->map_size + 2, 2, "H-Hint\n");
			mvwprintw(map_win, map_info->map_size + 3, 2, "Esc-Back\n");
			wattroff(map_win, COLOR_PAIR(4));

		}
		else if (map_info->next_char == 'h' || map_info->next_char == 'H') { // hint poziv
			Dot place, *hints;

			init_pair(5, COLOR_WHITE, COLOR_BLACK);
			init_pair(4, COLOR_RED, COLOR_BLACK);
			attron(COLOR_PAIR(5));
			mvwprintw(map_win, map_info->map_size + 2, 2, "H-Hint\n");
			attroff(COLOR_PAIR(5));
			wattron(map_win, COLOR_PAIR(4));
			mvwprintw(map_win, map_info->map_size + 1, 2, "S-Save\n");
			mvwprintw(map_win, map_info->map_size + 3, 2, "Esc-Back\n");
			wattroff(map_win, COLOR_PAIR(4));

			for (i = 1; i < map_info->map_size - 1; i++)
				if (map_info->map_matrix[0][i] == DOOR)
					break;

			find_player(map_info, player_info);
			place.x = player_info->x_temp;
			place.y = player_info->y_temp;
				
			hints = (Dot*)malloc(3 * sizeof(Dot));
			hints = hint(map_info, place, 0, i);

			
			for (int i = 0; i < 3; i++) {
				map_info->map_matrix[hints[i].x][hints[i].y] = 'H';
			}
			print_map(map_win, map_info);
			for (int i = 0; i < 3; i++) {
				map_info->map_matrix[hints[i].x][hints[i].y] = PATH;
			}

			free(hints);
		}
		else if (map_info->next_char == 'b') {
			
		}
		if (map_info->next_char == KEY_DOWN || map_info->next_char == KEY_UP || map_info->next_char == KEY_LEFT || map_info->next_char == KEY_RIGHT) {
			player_movement(map_info, player_info);
		}
		if (map_info->hit != DOOR && (map_info->next_char == KEY_UP || map_info->next_char == KEY_DOWN || map_info->next_char == KEY_LEFT || map_info->next_char == KEY_RIGHT) && map_info->hit != WALL) {
			switch (level) {
				case 1: mummy_movement(map_win, map_info); break;
				case 2: mummy_harder(map_win, map_info, player_info, level); break;
				case 3: mummy_harder(map_win, map_info, player_info, level); break;
			}
	//		mummy_movement(map_win, map_info);	//potez mumije, krece se samo ako igrac nije na vratima,ako su kliknute strelice i ako ne udara u zid
		}
		if (map_info->hit == MUMMY || map_info->hit == DOOR) {		//prepoynaje da li je gotova partija
			end = clock();			//zavrsava se partija i vreme se zaustavlja
			break;
		}
		else if (map_info->hit == WALL)
			(player_info->number_of_moves)--; //ukoliko udara u zid taj potez ne treba da se broji
	}
	endwin(map_win);
	if (map_info->next_char != KEY_ESC) {
		initscr();
		clear();
		noecho();
		cbreak();
		start_color();
		curs_set(0);
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		if (map_info->hit == MUMMY) {
			attron(COLOR_PAIR(1));
			mvwprintw(stdscr, 10, 10, "You lost :( \n");
			attroff(COLOR_PAIR(1));
		}
		else if (map_info->hit == DOOR) {
			int coefficient_mummy, coefficient_map_size;

			seconds = sec = (int)(end - start) / CLOCKS_PER_SEC;
			minutes = seconds / 60 + map_info->minutes_played;
			seconds = seconds % 60 + map_info->seconds_played;
			attron(COLOR_PAIR(1));
			mvwprintw(stdscr, 3, 2, "You won in %d moves ! ! !", player_info->number_of_moves);
			mvwprintw(stdscr, 4, 2, "Time: %02d : %02d", minutes, seconds);
			attroff(COLOR_PAIR(1));
			map_info->hit = 0;

			switch (map_info->game_level) {
			case 1: coefficient_mummy = 300 * map_info->mummy_number; break;				//broj mumija
			case 2: coefficient_mummy = 400 * map_info->mummy_number; break;
			case 3: coefficient_mummy = 500 * map_info->mummy_number; break;
			}

			high_score =(map_info->map_size *coefficient_mummy)/(player_info->number_of_moves*sec);
			flag=update_high_scores(high_score);
		}
		if (!flag)
			getch();
		refresh();
		endwin();
	}
	else
		save_current_game(map_info, player_info); // ako se izlazi na esc cuva se trenutna igra
}
