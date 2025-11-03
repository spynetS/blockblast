#ifndef SHAPE_H
#define SHAPE_H

#include <curses.h>

enum color {
	RED,
	BLUE,
	GREEN,
	BLACK,
	YELLOW
};

typedef struct {
	int x;
	int y;
	int color;
} Point;

typedef struct {
	Point pos;
	Point *cubes;
	int num_cubes;
	int color;
} Shape;

void setPoint(Point* point, int x, int y);
Shape* new_shape(int x, int y, int type);
void free_shape(Shape* shape);
void shape_render(WINDOW* win, Shape* shape);
void point_render(WINDOW* win, Point* shape);
void rotate_right(Shape* shape);

int collide(Point** placed_cubes, int num_placed_cubes, Shape* shape);
int outside(Shape shape, int x, int y, int width, int height);


#endif
