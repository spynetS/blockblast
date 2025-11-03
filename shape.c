#include "shape.h"

#include <curses.h>
#include <stdlib.h>
#include <stdio.h>

void set_shape(Shape *shape, int type) {
	switch(type) {
		// --- 1️⃣ Single Block ---
	case 0:
		shape->num_cubes = 1;
		shape->cubes = malloc(sizeof(Point) * shape->num_cubes);
		setPoint(&shape->cubes[0], 0, 0);
		break;

		// --- 2️⃣ Domino Pieces ---
	case 1: // 1x2 horizontal
		shape->num_cubes = 2;
		shape->cubes = malloc(sizeof(Point) * 2);
		setPoint(&shape->cubes[0], 0, 0);
		setPoint(&shape->cubes[1], 1, 0);
		break;

	case 2: // 2x1 vertical
		shape->num_cubes = 2;
		shape->cubes = malloc(sizeof(Point) * 2);
		setPoint(&shape->cubes[0], 0, 0);
		setPoint(&shape->cubes[1], 0, 1);
		break;

		// --- 3️⃣ Triominoes ---
	case 3: // 1x3 horizontal
		shape->num_cubes = 3;
		shape->cubes = malloc(sizeof(Point) * 3);
		setPoint(&shape->cubes[0], -1, 0);
		setPoint(&shape->cubes[1], 0, 0);
		setPoint(&shape->cubes[2], 1, 0);
		break;

	case 4: // 3x1 vertical
		shape->num_cubes = 3;
		shape->cubes = malloc(sizeof(Point) * 3);
		setPoint(&shape->cubes[0], 0, -1);
		setPoint(&shape->cubes[1], 0, 0);
		setPoint(&shape->cubes[2], 0, 1);
		break;

	case 5: // small L
		shape->num_cubes = 3;
		shape->cubes = malloc(sizeof(Point) * 3);
		setPoint(&shape->cubes[0], 0, 0);
		setPoint(&shape->cubes[1], 0, 1);
		setPoint(&shape->cubes[2], 1, 1);
		break;

	case 6: // small J
		shape->num_cubes = 3;
		shape->cubes = malloc(sizeof(Point) * 3);
		setPoint(&shape->cubes[0], 0, 0);
		setPoint(&shape->cubes[1], 0, 1);
		setPoint(&shape->cubes[2], -1, 1);
		break;

		// --- 4️⃣ Tetriminoes ---
	case 7: // I
		shape->num_cubes = 4;
		shape->cubes = malloc(sizeof(Point) * 4);
		setPoint(&shape->cubes[0], -1, 0);
		setPoint(&shape->cubes[1], 0, 0);
		setPoint(&shape->cubes[2], 1, 0);
		setPoint(&shape->cubes[3], 2, 0);
		break;

	case 8: // O
		shape->num_cubes = 4;
		shape->cubes = malloc(sizeof(Point) * 4);
		setPoint(&shape->cubes[0], 0, 0);
		setPoint(&shape->cubes[1], 1, 0);
		setPoint(&shape->cubes[2], 0, 1);
		setPoint(&shape->cubes[3], 1, 1);
		break;

		// ...continue with other shapes similarly...

	default:
		// fallback: single block
		shape->num_cubes = 1;
		shape->cubes = malloc(sizeof(Point));
		setPoint(&shape->cubes[0], 0, 0);
		break;
	}
}

Shape* new_shape(int x, int y, int type){
	Shape* shape = malloc(sizeof(Shape));
	shape->pos.x = x;
	shape->pos.y = y;
	shape->color = (int) rand() % 8;

	set_shape(shape,type);
	
	for (int i =0 ; i< shape->num_cubes; i++) {
		shape->cubes[i].color = shape->color;
	}
	return shape;
}
void free_shape(Shape* shape){
	free(shape->cubes);
	free(shape);
}

void setPoint(Point* point, int x, int y){
	point->x = x;
	point->y = y;
}

int color_to_pair(int color) {
    switch (color) {
        case COLOR_RED:     return 1;
        case COLOR_GREEN:   return 2;
        case COLOR_YELLOW:  return 3;
        case COLOR_BLUE:    return 4;
        case COLOR_MAGENTA: return 5;
        case COLOR_CYAN:    return 6;
        case COLOR_WHITE:   return 7;
        default:            return 7;
    }
}


void shape_render(WINDOW* win, Shape* shape){

	int pair = color_to_pair(shape->color);
	wattron(win,COLOR_PAIR(pair));
	for(int i = 0; i < shape->num_cubes; i ++) {
		mvwprintw(win,shape->pos.y+shape->cubes[i].y,
						 shape->pos.x*2+shape->cubes[i].x*2,"██");
	}
	wattroff(win, COLOR_PAIR(pair));
}

void point_render(WINDOW* win, Point* cube){
	int pair = color_to_pair(cube->color);
	wattron(win,COLOR_PAIR(pair));
	mvwprintw(win,cube->y,
						cube->x*2,"██");

	wattroff(win, COLOR_PAIR(pair));
}


void rotate_right(Shape *shape){
    // the o cant be rotated

	for(int i = 0; i < 4; i++){
		Point *cube = &shape->cubes[i];

		int x_temp = -cube->y;
		cube->y =  cube->x;
		cube->x =  x_temp;
	}

}


int collide(Point** placed_cubes, int num_placed_cubes, Shape* shape){
	for(int i = 0; i < num_placed_cubes; i ++){
		Point *placed = placed_cubes[i];
		for(int k = 0; k < shape->num_cubes; k++){
			if((placed->x) == (shape->cubes[k].x + shape->pos.x) &&
				 (placed->y) == (shape->cubes[k].y + shape->pos.y))
				return 1;
		}
	}
	return 0;
}

int outside(Shape shape, int x, int y, int width, int height) {
	shape.pos.x += x;
	shape.pos.y += y;

	for(int i = 0; i< shape.num_cubes; i ++){
		if(((shape.cubes[i].x + shape.pos.x) >= width/2-1) || ((shape.cubes[i].x + shape.pos.x) <= 0) && x != 0)
			return 1;
		if(((shape.cubes[i].y + shape.pos.y) >= height-1) || ((shape.cubes[i].y + shape.pos.y) <= 0) && y != 0)
			return 1;
	}
	return 0;
}


