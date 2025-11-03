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


int main() {

		
		
		int placed_cubes [8][8];

		int num_placed_cubes = 0;

		setlocale(LC_ALL, "");
		initscr();
		raw();
		noecho();
		curs_set(0);

		init_colors_once();
		int ymax,xmax;
		getmaxyx(stdscr,ymax,xmax);
		
		WINDOW *my_win;
		int width=(10)*2, height=10,startx=(xmax-width)/2, starty=(ymax-height)/2;
		my_win = newwin(height, width,starty, startx);
		
		wrefresh(my_win);

		Shape *shape = new_shape(3, 3, 0);
		
		start_color();

		while (1) {
				wclear(my_win);
				box(my_win, 0, 0);


				for(int y = 1; y < height-1; y ++){
						for(int x = 1; x < width-1/2-3; x += 2) {
								mvwprintw(my_win, y,x," .");
						}
				}
				for(int i = 0; i < num_placed_cubes; i ++){
// 						point_render(my_win,placed_cubes[i]);
				}

				int is_colliding = 0;//collide(placed_cubes, num_placed_cubes, shape);
				int temp_color = shape->color;
				if(is_colliding){
						shape->color = COLOR_RED;
				}								
				shape_render(my_win,shape);
				shape->color = temp_color;

				
				mvwprintw(my_win,0,0,"%d",num_placed_cubes);
				
				wrefresh(my_win);

				
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
								placed_cubes[tmp.y][tmp.x] = shape->color;
						} 

						free_shape(shape);                                                                  
						shape = generate_shape(width/4-1,height/2-1,rand() % SHAPE_COUNT);
				}
        
				remove_row(placed_cubes,8,8);

				usleep(16*1000);
		}
		
//		getch();
		endwin();

		free_shape(shape);
		// free points;
		
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
		
}
