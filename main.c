#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include<locale.h>
#include "shape.h"
#include <unistd.h>
#define SHAPE_COUNT 26

void init_colors_once(void);
void remove_row(int **placed_cubes, int w, int h);
Shape *generate_shape(int x, int y, int type);
void render_cubes(WINDOW *win, int **placed_cubes);

int main() {

		
	// init placed cubes array	
	int **placed_cubes = malloc(9 * sizeof(int*));
	for(int i = 0; i <= 8; i++)
    placed_cubes[i] = malloc(9 * sizeof(int));
	
	int num_placed_cubes = 0;

	setlocale(LC_ALL, "");
	initscr();
	raw();
	noecho();
	curs_set(0);

	init_colors_once();
	int ymax,xmax;
	getmaxyx(stdscr,ymax,xmax);
		
	WINDOW *game_win;
	int width=(10)*2, height=10,startx=(xmax-width)/2, starty=(ymax-height)/2;
	
	game_win = newwin(height, width,starty, startx);

	
	wrefresh(game_win);

	Shape *shape = new_shape(3, 3, 7);
		
	start_color();

	while (1) {
		wclear(game_win);
		box(game_win, 0, 0);


		for(int y = 1; y < height-1; y ++){
			for(int x = 1; x < width-1/2-3; x += 2) {
				mvwprintw(game_win, y,x," .");
			}
		}

		render_cubes(game_win,placed_cubes);


		int is_colliding = collide(placed_cubes, shape);
		int temp_color = shape->color;
		if(is_colliding){
			shape->color = COLOR_RED;
		}								
		shape_render(game_win,shape);
		shape->color = temp_color;


		wrefresh(game_win);
				
		int ch = getch();
		if (ch == 'd' && !outside(*shape, 1, 0, width, height)) 
			shape->pos.x+=1;
		if (ch == 'a' && !outside(*shape, -1, 0, width, height))
			shape->pos.x-=1;
		if (ch == 's' && !outside(*shape, 0, 1, width, height)) 
			shape->pos.y++;
		if (ch == 'w' && !outside(*shape, 0, -1, width, height))
			shape->pos.y--;

		if (ch == ' ' && !is_colliding) {
			for(int i = 0; i < shape->num_cubes; i ++) {
				Point tmp = shape->cubes[i];
				tmp.x += shape->pos.x;
				tmp.y += shape->pos.y;
				int x = tmp.x - 1;
				int y = tmp.y - 1;
				if (shape->color >= 1)
					placed_cubes[y][x] = shape->color;
				else
					printw("Out of bounds: %d %d %d\n", y, x, shape->color);

			}

			free_shape(shape);                                                                  
			shape = generate_shape(width/4-1,height/2-1,rand() % 8);
		}
        
		remove_row(placed_cubes,8,8);

		usleep(16*1000);
	}
		
	//		getch();
	endwin();

	free_shape(shape);
	// free points;-
		
	return 0;
}


Shape *generate_shape(int x, int y, int type) {

	Shape *shape = new_shape(x, y, type);
	for (int i = 0; i < (rand()%4); i++) {
		rotate_right(shape);
	}
	return shape;
}


void init_colors_once(void) {
	if (!has_colors()) return;
	start_color();
	use_default_colors(); // optional: allows using -1 as default background
	// Predefine a few color pairs (1..n). Pair 0 is default and can't be set with init_pair.
	init_pair(1, COLOR_RED,   COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW,COLOR_BLACK);
	init_pair(4, COLOR_BLUE,  COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA,COLOR_BLACK);
	init_pair(6, COLOR_CYAN,  COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
}

void remove_row(int **placed_cubes, int w, int h) {

	int rows_to_clear[8] = {0};
	int cols_to_clear[8] = {0};

	for(int y = 0; y < 8; y++){
    int row_count = 0;
    int col_count = 0;
    for(int x = 0; x < 8; x++){
			if(placed_cubes[y][x] != 0) row_count++;
			if(placed_cubes[x][y] != 0) col_count++;
    }
    if(row_count == 8) rows_to_clear[y] = 1;
    if(col_count == 8) cols_to_clear[y] = 1;
	}

	// Clear rows
	for(int y = 0; y < 8; y++){
    if(rows_to_clear[y]){
			for(int x = 0; x < 8; x++) placed_cubes[y][x] = 0;
    }
	}

	// Clear columns
	for(int x = 0; x < 8; x++){
    if(cols_to_clear[x]){
			for(int y = 0; y < 8; y++) placed_cubes[y][x] = 0;
    }
	}


}
void render_cubes(WINDOW *win, int **placed_cubes){
	for(int y = 0; y < 8; y ++ ){
		for(int x = 0; x < 8; x ++ ){
			if(placed_cubes[y][x] != 0) {
				int pair = placed_cubes[y][x];
				wattron(win,COLOR_PAIR(pair));
				mvwprintw(win,y+1,x*2+2,"██");
				wattroff(win, COLOR_PAIR(pair));
			}

		}
	}
}
