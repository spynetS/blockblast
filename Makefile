
CC_FLAGS = -lncursesw
CC = gcc

main : main.c shape.c
	$(CC) $(CC_FLAGS) -o main main.c shape.c

run : main
	alacritty --hold -e ./main
