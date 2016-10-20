#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include <curses.h>
#include <time.h>
#include <windows.h>
#define INFINITY 2147483647

int RAND(int min, int max) {
	srand(time(NULL));
	if (max < 0) return 0;
	return (rand() % (max + 1)) + min;
}
int rand_num(int min_num, int max_num)
{
	int result = 0, low_num = 0, hi_num = 0;
	if (min_num<max_num)
	{
		low_num = min_num;
		hi_num = max_num + 1; // this is done to include max_num in output.
	}
	else {
		low_num = max_num + 1;// this is done to include max_num in output.
		hi_num = min_num;
	}
	srand(time(NULL));
	result = (rand() % (hi_num - low_num)) + low_num;
	return result;
}
int dot_empty(Dot t) // prazna tacka sa (0,0)
{
	if (t.x == 0 && t.y == 0)
		return 1;

	return 0;
}
int set_empty(Dot *target, int n) { // provera da li je skup prazan
	int i;
	if (target != NULL) {
		for (i = 0; i < n; i++)
			if (!dot_empty(target[i]))
				return 0;
		return 1;
	}
	return 0;
}
int sets_num(Dot **sets, int n) { // ako je kompletirano odnosno ako imamo samo 1 skup
	int i = 0, num = 0;

	for (i = 0; i < n; i++) // prolazimo ceo 
		if (!set_empty(sets[i], n))
			num++;

	return num;

}
int in_set(Dot target, Dot dots[], int n) { // da li je u skupu
	int i;

	for (i = 0; i < n; i++) // prolazimo kroz sve tacke
	{
		if (dots == NULL)
			return 0;
		if ((dots[i].x == target.x) && (dots[i].y == target.y))
			return 1;

	}
	return 0;
}

int iter(int map_size) // broj iteracija
{
	return (map_size - 2)*(map_size - 2);
}
int inside(Dot target, int map_size) // proveravamo da li tacka nije na cosku
{
	if (target.x == 0 || target.y == 0 || target.y == (map_size - 1) || target.x == (map_size - 1)) // ako je na ivicama
		return 0;

	return 1;
}
int neighbors(Dot first, Dot second) {
	if (first.x == second.x)
		if (first.y == (second.y - 1) || first.y == (second.y + 1))
			return 1;

	if (first.y == second.y)
		if (first.x == (second.x - 1) || first.x == (second.x + 1))
			return 1;

	return 0;

}

int in_array(Dot target, Dot *arr, int k) {
	int i;

	for (i = 0; i < k; i++)
		if ((target.x == arr[i].x) && (target.y == arr[i].y))
			return 1;
	return 0;
}

Dot place_exit(MAP **map_info) {
	MAP *map = *map_info;
	Dot exit;
	int i;

	exit.x = 0;
	exit.y = RAND(1, map->map_size - 2);

	while (map->map_matrix[1][exit.y] == WALL) exit.y--;

	map->map_matrix[exit.x][exit.y] = DOOR;

	return exit;


}
Dot neighbor(Dot target, MAP *map, char direction, int cell) { // nalazimo gornji cvor susedni cvor
	Dot top_dot;
	// cell je tip tacke wall ili path

	if (direction == 't') // ako trazimo gornji
	{
		top_dot.x = target.x - 1;
		top_dot.y = target.y;
	}
	else if (direction == 'b') // trazimo donji
	{
		top_dot.x = target.x + 1;
		top_dot.y = target.y;
	}
	else if (direction == 'l') // trazimo levo
	{
		top_dot.x = target.x;
		top_dot.y = target.y - 1;
	}
	else if (direction == 'r') // trazimo desno
	{
		top_dot.x = target.x;
		top_dot.y = target.y + 1;
	}
	else {
		top_dot.x = 0;
		top_dot.y = 0;
	}

	// ako nije prazna ako je unutar i  ako je generisana vrednost
	if (!dot_empty(top_dot) && inside(top_dot, map->map_size) && (map->map_matrix[top_dot.x][top_dot.y] == cell))
		return top_dot;

	top_dot.x = 0;
	top_dot.y = 0;
	return top_dot; // vracamo praznu tacku

}

int has_neighbors(Dot target, MAP *map, int cell) { // provera da li tacka ima suseda

													// proveravamo da li ima gore
	if (!dot_empty(neighbor(target, map, 't', cell)))
		return 1;

	// donju tacku
	if (!dot_empty(neighbor(target, map, 'b', cell)))
		return 1;

	// desnu tacku
	if (!dot_empty(neighbor(target, map, 'r', cell)))
		return 1;

	// levu tacku
	if (!dot_empty(neighbor(target, map, 'l', cell)))
		return 1;

	return 0;
}

Dot * find_set(Dot target, Dot ** sets, int num_sets, int num_dots) { // nalazimo skup na osnovu tacke
	int i = 0;
	for (i = 0; i < num_sets; i++) // prolazimo kroz sve skupove
		if (in_set(target, sets[i], num_dots)) // ako postoji u skupu
			return sets[i];

	return NULL;

}
Dot * dot_to_set(Dot * target, Dot source, int n) { // ubacujemo tacku u skup
	int i;
	for (i = 0; i < n; i++) // dolazimo na poslednji
		if (dot_empty(target[i]))
			break;
	if (i < n) // ako nije doslo do kraja
	{
		target[i].x = source.x;
		target[i].y = source.y;
	}

	return target;


}
Dot * set_remove(Dot * target) { // ukloini skup
	Dot *t = target;
	while (!dot_empty(*t))
	{
		t->x = 0; t->y = 0;
		t++;
	}
	return target;
}
Dot * union_sets(Dot * target, Dot * source, int n) { // unija skupova
	if (target != NULL && source != NULL && target != source) {
		int i, j;
		for (i = 0; i < n; i++) // dolazimo na poslednji
			if (dot_empty(target[i]))
				break;
		for (j = 0; j < (n - i); j++) { // prepisujemo ostale
			target[i + j].x = source[j].x;
			target[i + j].y = source[j].y;
		}
	}
	return target;
}
Dot ** new_set(Dot new_dot, Dot **sets, int n) { // ubacujemo tacku u novi skup
	int i;

	for (i = 0; i < n; i++)
	{
		if (sets[i] != NULL && set_empty(sets[i], n))
			break;
	}
	if (sets[i] != NULL) {
		sets[i][0].x = new_dot.x;
		sets[i][0].y = new_dot.y;
	}

	return sets;



}



Dot **generate_map(MAP **map, int generate_this, int *num_sets) { // generisemo MAP_SIZE tacki za pocetak kruskala vraca skupove
	MAP *map_point = *map; // pokazivac na mapu
	int i, j, k = 0, num_iter = iter(map_point->map_size), density = 3;
	Dot *dots, **sets;
	int wait1, wait2;

	// provera generisanje
	if (generate_this != WALL && generate_this != PATH) // ako nije ispravno setovan
		generate_this = PATH;
	initscr();
	clear();
	noecho();
	cbreak();
	start_color();
	curs_set(0);
	if (map_point->wall_color == 1) {
		init_pair(1, COLOR_WHITE, COLOR_WHITE);
	}
	else if (map_point->wall_color == 2) {
		init_pair(1, COLOR_BLUE, COLOR_BLUE);
	}
	if (map_point->wall_color == 3) {
		init_pair(1, COLOR_GREEN, COLOR_GREEN);
	}
	if (map_point->wall_color == 4) {
		init_pair(1, COLOR_BLACK, COLOR_BLACK);
	}

	init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
	// pocetno stvaranje zidova
	for (i = 0; i < map_point->map_size; i++) {
		for (j = 0; j < map_point->map_size; j++) {

			if (i == 0 || j == 0 || i == (map_point->map_size - 1) || j == (map_point->map_size - 1)) { // ako su coskovi
				map_point->map_matrix[i][j] = WALL; // prvo generise zid
				if (map_point->draw == 1) {
					wattron(stdscr,COLOR_PAIR(1));
					mvwprintw(stdscr,i, j, " ");
					wattroff(stdscr,COLOR_PAIR(1));
					Sleep(10);

					wrefresh(stdscr);
					
				}
			}
			else
				if (generate_this == WALL) {
					map_point->map_matrix[i][j] = PATH;
					if (map_point->draw == 1) {
						wattron(stdscr,COLOR_PAIR(2));
						mvwprintw(stdscr,i, j, " ");
						wattroff(stdscr,COLOR_PAIR(2));
						Sleep(10);
						wrefresh(stdscr);
					}
				}
				else {
					map_point->map_matrix[i][j] = WALL;
					if (map_point->draw == 1) {
						wattron(stdscr,COLOR_PAIR(1));
						mvwprintw(stdscr,i, j, " ");
						wattron(stdscr,COLOR_PAIR(1));
						Sleep(10);
						wrefresh(stdscr);
					}
				}
		}
	}



	// sve tacke
	dots = (Dot *)calloc(num_iter, sizeof(Dot));

	// svako drugo 
	for (i = 1; i < map_point->map_size - 1; i += density) {
		for (j = 1; j < map_point->map_size - 1; j += density)
		{
			dots[k].x = i;
			dots[k].y = j;

			k++;
		}
	}

	// broj skupova
	*num_sets = k;
	//ALOKACIJA SETOVA
	sets = (Dot **)calloc(k, sizeof(Dot *));



	// upisujemo u matricu i u skup
	for (i = 0; i < k; i++) {

		sets[i] = (Dot *)calloc(num_iter, sizeof(Dot));
		// upis matrice
		map_point->map_matrix[dots[i].x][dots[i].y] = generate_this;
		if (map_point->draw == 1) {
			wattron(stdscr,COLOR_PAIR(2));
			mvwprintw(stdscr,dots[i].x, dots[i].y, " ");
			wattroff(stdscr,COLOR_PAIR(2));
			Sleep(30);
			wrefresh(stdscr);
			
		}
		// upis u skup
		sets[i][0].x = dots[i].x;
		sets[i][0].y = dots[i].y;
	}

	free(dots);

	return sets;

}

void extend_map(MAP **map, const Dot **sets, int generate_this, int num_iter) { // prosiri mapu
	MAP *map_point = *map;
	Dot new_dot, *posibleDots, posibleDot, *topSet = NULL, *bottomSet = NULL, *leftSet = NULL, *rightSet = NULL, *source_set, topDot, bottomDot, leftDot, rightDot;
	int i = 0, j, k, n, new_index, oposite_cell = (generate_this == WALL) ? PATH : WALL, num_dots = iter(map_point->map_size);
	char directions[4] = { 't', 'b', 'l', 'r' };
	int p;

	// sve moguce tacke odnosno susedi svih tacaka
	posibleDots = (Dot *)calloc(num_dots, sizeof(Dot));
	k = 0;
	for (i = 1; i < map_point->map_size - 1; i++)
	{
		for (j = 1; j < map_point->map_size - 1; j++)
		{
			posibleDot.x = i;
			posibleDot.y = j;

			if (has_neighbors(posibleDot, map_point, generate_this)) { // ako polje ima suseda


																	   // proveravamo za sve ostale
				for (n = 0; n < 4; n++) {	// za svaku stranu proveravamo da li ima suseda
					if (!dot_empty(neighbor(posibleDot, map_point, directions[n], generate_this)) && !in_array(posibleDot, posibleDots, k)) // ako ima gornjeg suseda
					{
						posibleDots[k] = posibleDot; // ubacujemo u niz
						k++;  break; // breakujemo posto je dovoljan jedan sused
					}
				}

			}
		}
	}

	// skupljanje moguch tacaka

	while (sets_num(sets, num_iter) != 1) // sve dok ima vise od 1 polja
	{

		// rand od mogucih
		new_index = rand() % k;
		new_dot = posibleDots[new_index];

		// updejtujemo posible dots
		if (has_neighbors(new_dot, map_point, oposite_cell)) // ako ima komsija koji nisu generisani
		{
			// susedne tacke nasumicno izabrane tacke sa negenerisanim poljem
			topDot = neighbor(new_dot, map_point, 't', oposite_cell);
			bottomDot = neighbor(new_dot, map_point, 'b', oposite_cell);
			leftDot = neighbor(new_dot, map_point, 'l', oposite_cell);
			rightDot = neighbor(new_dot, map_point, 'r', oposite_cell);

			// pronalazimo tacku koja ce popuniti bivsu tacku
			if (!dot_empty(topDot) && !in_array(topDot, posibleDots, k)) // ako ima gornjeg suseda i ako vec nije u nizu
				posibleDots[new_index] = topDot; // ubacujemo u niz

			else if (!dot_empty(bottomDot) && !in_array(bottomDot, posibleDots, k)) // ako ima donjeg suseda i ako vec nije u nizu
				posibleDots[new_index] = bottomDot; // ubacujemo u niz

			else if (!dot_empty(leftDot) && !in_array(leftDot, posibleDots, k)) // ako ima donjeg suseda i ako vec nije u nizu
				posibleDots[new_index] = leftDot; // ubacujemo u niz

			else if (!dot_empty(rightDot) && !in_array(rightDot, posibleDots, k)) // ako ima donjeg suseda i ako vec nije u nizu
				posibleDots[new_index] = rightDot; // ubacujemo u niz

												   // popunjavamo posle

			if (!dot_empty(bottomDot) && !in_array(bottomDot, posibleDots, k)) // ako ima donjeg suseda
			{
				posibleDots[k] = bottomDot; // ubacujemo u niz
				k++;
			}

			if (!dot_empty(leftDot) && !in_array(leftDot, posibleDots, k)) // ako ima levog suseda
			{
				posibleDots[k] = leftDot; // ubacujemo u niz
				k++;
			}

			if (!dot_empty(rightDot) && !in_array(rightDot, posibleDots, k)) // ako ima desnog suseda
			{
				posibleDots[k] = rightDot; // ubacujemo u niz
				k++;
			}

		}
		else // ako nema brisemo
		{
			for (i = new_index; i < k; i++)
				posibleDots[i] = posibleDots[i + 1];
			k--;
		}

		// susedi generisane tacke sa generisanim poljem
		topDot = neighbor(new_dot, map_point, 't', generate_this);
		bottomDot = neighbor(new_dot, map_point, 'b', generate_this);
		leftDot = neighbor(new_dot, map_point, 'l', generate_this);
		rightDot = neighbor(new_dot, map_point, 'r', generate_this);

		// susedni skupovi
		if (!dot_empty(topDot))
			topSet = find_set(topDot, sets, num_iter, num_dots);
		if (!dot_empty(bottomDot))
			bottomSet = find_set(bottomDot, sets, num_iter, num_dots);
		if (!dot_empty(leftDot))
			leftSet = find_set(leftDot, sets, num_iter, num_dots);
		if (!dot_empty(rightDot))
			rightSet = find_set(rightDot, sets, num_iter, num_dots);




		//  trazimo skup suseda
		if (topSet != NULL)
			source_set = topSet;
		else if (bottomSet != NULL)
			source_set = bottomSet;
		else if (leftSet != NULL)
			source_set = leftSet;
		else if (rightSet != NULL)
			source_set = rightSet;
		else // na null samo da compiler ne pravi probleme
			source_set = 0;

		// ako ne dodajemo tacku 
		if (!source_set) break;
		// prikljucujemo mu novu tacku
		source_set = dot_to_set(source_set, new_dot, num_dots);


		// skupove ostalih suseda dodajemo u UNIJU prvog

		if (bottomSet != NULL && bottomSet != source_set) {  // bottom 
			source_set = union_sets(source_set, bottomSet, num_dots); // unija
			bottomSet = set_remove(bottomSet);
		}
		if (rightSet != NULL && rightSet != source_set) {  // bottom 
			source_set = union_sets(source_set, rightSet, num_dots); // unija
			rightSet = set_remove(rightSet);
		}
		if (leftSet != NULL && leftSet != source_set) {  // bottom 
			source_set = union_sets(source_set, leftSet, num_dots); // unija
			leftSet = set_remove(leftSet);
		}


		// reset vrednosti
		topDot.x = topDot.y = 0;
		bottomDot.x = bottomDot.y = 0;
		leftDot.x = leftDot.y = 0;
		rightDot.x = rightDot.y = 0;

		topSet = NULL;
		bottomSet = NULL;
		leftSet = NULL;
		rightSet = NULL;

		// azuriranje matrice
		map_point->map_matrix[new_dot.x][new_dot.y] = generate_this;
		if (map_point->draw == 1) {
			attron(COLOR_PAIR(2));
			mvprintw(new_dot.x, new_dot.y, " ");
			attroff(COLOR_PAIR(2));
			refresh();
			Sleep(30);
		}


	}
	//clear();
	endwin();
	// OSLOBADJANJE
	for (i = 0; i < num_iter; i++)
		free(sets[i]);
	free(sets);
}

void make_maze(MAP **map) { // rekurzivno deljenje mape
	int i, j, p, threshold = 2, startQ, endQ, halfX, halfY, leftRand, rightRand, topRand, bottomRand, *posible, n;
	MAP *map_info = *map;
	Dot topLeft, bottomRight, firstTop, firstBtm, secondTop, secondBtm, thirdTop, thirdBtm, fourthTop, fourthBtm,
		*topDots = (Dot *)calloc(map_info->map_size, sizeof(Dot)),
		*btmDots = (Dot *)calloc(map_info->map_size, sizeof(Dot));

	threshold = 2;

	initscr();
	clear();
	noecho();
	cbreak();
	start_color();
	curs_set(0);
	if (map_info->wall_color == 1) {
		init_pair(1, COLOR_WHITE, COLOR_WHITE);
	}
	else if (map_info->wall_color == 2) {
		init_pair(1, COLOR_BLUE, COLOR_BLUE);
	}
	else if (map_info->wall_color == 2) {
		init_pair(1, COLOR_GREEN, COLOR_GREEN);
	}
	else {
		init_pair(1, COLOR_BLACK, COLOR_BLACK);
	}
	
	init_pair(2, COLOR_YELLOW, COLOR_YELLOW);


	// pocetno popunjavanje lavirinta
	for (i = 0; i < map_info->map_size; i++) {
		for (j = 0; j < map_info->map_size; j++) {

			if (i == 0 || j == 0 || i == (map_info->map_size - 1) || j == (map_info->map_size - 1)) { // ako su coskovi
				map_info->map_matrix[i][j] = WALL; // prvo generise zid
				if (map_info->draw == 1) {
					attron(COLOR_PAIR(1));
					mvprintw(i, j, " ");
					attroff(COLOR_PAIR(1));
					refresh();
					Sleep(10);

				}
			}
			else {
				map_info->map_matrix[i][j] = PATH;
				if (map_info->draw == 1) {
					attron(COLOR_PAIR(2));
					mvprintw(i, j, " ");
					attroff(COLOR_PAIR(2));
					refresh();
					Sleep(10);

				}
			}
		}
	}




	// pocetne tacke
	topLeft.x = topLeft.y = 1;
	bottomRight.x = bottomRight.y = map_info->map_size - 2;


	startQ = endQ = 0;

	//insert u red
	topDots[endQ] = topLeft;
	btmDots[endQ] = bottomRight;
	endQ++;

	while (startQ != endQ) // dok red nije prazan
	{
		// delete reda
		firstTop = topDots[startQ];
		fourthBtm = btmDots[startQ];
		startQ++;

		// izracunavanje
		halfX = (fourthBtm.x + firstTop.x) / 2;
		halfY = (fourthBtm.y + firstTop.y) / 2;

		if (map_info->map_matrix[halfX][fourthBtm.x + 1] == PATH || map_info->map_matrix[halfX][firstTop.x - 1] == PATH) halfX++;
		if (map_info->map_matrix[fourthBtm.y + 1][halfY] == PATH || map_info->map_matrix[firstTop.y - 1][halfY] == PATH) halfY++;

		// iscrtavanje uspravne ose
		if ((fourthBtm.x - firstTop.x) > threshold)
			for (i = firstTop.x; i <= fourthBtm.x; i++) {
				map_info->map_matrix[i][halfY] = WALL;

				if (map_info->draw == 1) {
					attron(COLOR_PAIR(1));
					mvprintw(i, halfY, " ");
					attroff(COLOR_PAIR(1));
					refresh();
					Sleep(10);
				}

			}

		// iscrtavanje horizontalne ose
		if ((fourthBtm.y - firstTop.y) > threshold)
			for (i = firstTop.y; i <= fourthBtm.y; i++) {
				map_info->map_matrix[halfX][i] = WALL;

				if (map_info->draw == 1) {
					attron(COLOR_PAIR(1));
					mvprintw(halfX, i, " ");
					attroff(COLOR_PAIR(1));
					refresh();
					Sleep(10);
				}

			}

		// definisanje alternativnih tacaka
		firstBtm.x = halfX - 1;
		firstBtm.y = halfY - 1;

		secondTop.x = firstTop.x;
		secondTop.y = halfY + 1;

		secondBtm.x = halfX - 1;
		secondBtm.y = fourthBtm.y;

		thirdTop.x = halfX + 1;
		thirdTop.y = firstTop.y;

		thirdBtm.x = fourthBtm.x;
		thirdBtm.y = halfY - 1;

		fourthTop.x = halfX + 1;
		fourthTop.y = halfY + 1;


		// provera i upisivanje
		/*
		1|2
		3|4
		*/

		// 1
		if ((firstBtm.x - firstTop.x) > threshold || (firstBtm.y - firstTop.y) > threshold)
		{
			topDots[endQ] = firstTop;
			btmDots[endQ] = firstBtm;
			endQ++;
		}

		// 2
		if ((secondBtm.x - secondTop.x) > threshold || (secondBtm.y - secondTop.y) > threshold)
		{
			topDots[endQ] = secondTop;
			btmDots[endQ] = secondBtm;
			endQ++;
		}

		// 3
		if ((thirdBtm.x - thirdTop.x) > threshold || (thirdBtm.y - thirdTop.y) > threshold)
		{
			topDots[endQ] = thirdTop;
			btmDots[endQ] = thirdBtm;
			endQ++;
		}

		// 4
		if ((fourthBtm.x - fourthTop.x) > threshold || (fourthBtm.y - fourthTop.y) > threshold)
		{
			topDots[endQ] = fourthTop;
			btmDots[endQ] = fourthBtm;
			endQ++;
		}


	}

	posible = (int *)calloc(map_info->map_size / 2, sizeof(int));

	// upisivanje prolaza
	for (i = 0; i < endQ; i++) { // za svaki prolaz
		firstTop = topDots[i];
		fourthBtm = btmDots[i];

		// izracunavanje polovine
		halfX = (fourthBtm.x + firstTop.x) / 2;
		halfY = (fourthBtm.y + firstTop.y) / 2;

		// sakupljanje mogucih za top btm left i right odabir
		// top
		n = 0;
		for (j = firstTop.x; j <= halfX - 1; j++)
			if (map_info->map_matrix[j][halfY - 1] == PATH && map_info->map_matrix[j][halfY + 1] == PATH) // ako su sa obe strane path
			{
				posible[n] = j;
				n++;
			}

		topRand = posible[RAND(0, n - 1)]; // biramo jedan od random

		map_info->map_matrix[topRand][halfY] = PATH;

		if (map_info->draw == 1) {
			attron(COLOR_PAIR(2));
			mvprintw(topRand, halfY, " ");
			attroff(COLOR_PAIR(2));
			refresh();
			Sleep(10);

		}

		// btm 
		n = 0;

		for (j = halfX + 1; j <= fourthBtm.x; j++)
			if (map_info->map_matrix[j][halfY - 1] == PATH && map_info->map_matrix[j][halfY + 1] == PATH) // ako su sa obe strane path
			{
				posible[n] = j;
				n++;
			}

		bottomRand = posible[RAND(0, n - 1)]; // biramo jedan od random

		map_info->map_matrix[bottomRand][halfY] = PATH;

		if (map_info->draw == 1) {
			attron(COLOR_PAIR(2));
			mvprintw(bottomRand, halfY, " ");
			attroff(COLOR_PAIR(2));
			refresh();
			Sleep(10);

		}


		// left
		n = 0;

		for (j = firstTop.y; j <= halfY - 1; j++)
			if (map_info->map_matrix[halfX - 1][j] == PATH && map_info->map_matrix[halfX + 1][j] == PATH) // ako su sa obe strane path
			{
				posible[n] = j;
				n++;
			}

		leftRand = posible[RAND(0, n - 1)];

		map_info->map_matrix[halfX][leftRand] = PATH;
		if (map_info->draw == 1) {
			attron(COLOR_PAIR(2));
			mvprintw(halfX, leftRand, " ");
			attroff(COLOR_PAIR(2));
			refresh();
			Sleep(10);

		}

		// right
		n = 0;

		for (j = halfY + 1; j <= fourthBtm.y; j++)
			if (map_info->map_matrix[halfX - 1][j] == PATH && map_info->map_matrix[halfX + 1][j] == PATH) // ako su sa obe strane path
			{
				posible[n] = j;
				n++;
			}

		rightRand = posible[RAND(0, n - 1)];

		map_info->map_matrix[halfX][rightRand] = PATH;
		if (map_info->draw == 1) {
			attron(COLOR_PAIR(2));
			mvprintw(halfX, rightRand, " ");
			attroff(COLOR_PAIR(2));
			refresh();
			Sleep(10);
		}

	}
	endwin();
	free(posible);
	free(topDots);
	free(btmDots);
}

Dot place_player(MAP **map_info, Dot exit, int length, int player) { // postavljamo playera
	MAP *map = *map_info;
	int i, j, startQ, endQ, n, *distance, dist, newDist = -1;
	Dot *visitedDots, start, tempDot, neigh, outDot, mummy;
	char dirs[] = { 't', 'l', 'b', 'r' }, dir;

	// alokacija vektora
	n = iter(map->map_size) * 2;
	visitedDots = (Dot *)calloc(n, sizeof(Dot));
	distance = (int *)calloc(n, sizeof(int));


	if (player != MUMMY && player != HUMAN) // podesavamo default playera
		player = HUMAN;

	// ako je mumija druge direkcije
	if (map->mummy_number == 2 && player == MUMMY) { // za drugu mumiju
		dirs[0] = 'r';
		dirs[1] = 't';
		dirs[2] = 'l';
		dirs[3] = 'b';
	}


	// podesavamo pocetnu tacku
	start = exit;
	start.x = 1;

	// insert u red
	startQ = 0;
	distance[startQ] = dist = 0;
	visitedDots[startQ] = start;
	endQ = 1;


	// sve dok ne stignemo do odredjene razdaljine
	while ((dist != length) && (startQ != endQ) && (endQ < n)) {

		// delete iz reda
		tempDot = visitedDots[startQ];
		dist = distance[startQ];

		outDot = tempDot;
		if (dist == length) {
			break; // ako smo stigli na toliku udaljenost
		}
		startQ++;


		for (i = 0; i < 4; i++) // za svaki smer
		{
			neigh = neighbor(tempDot, map, dirs[i], PATH); // nalazimo suseda
			if (!dot_empty(neigh) && !in_array(neigh, visitedDots, endQ)) // ako ga je pronasao i ako vec nije stavljen
			{
				// insert
				visitedDots[endQ] = neigh;
				distance[endQ] = dist + 1;
				if (distance[endQ] == length) // ako smo stigli do odredjene razdaljine
				{
					outDot = neigh;
					break;
				}					

				endQ++;

			}
			
		}

		// pronasli smo ga ako je dist + 1
		if (distance[endQ] == length)  break;
	}

	map->map_matrix[outDot.x][outDot.y] = player;
	

	free(distance);
	free(visitedDots);

	return outDot;


}