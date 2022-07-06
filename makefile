main: main.o maze.o algorithms.o stack.o random.o tree.o set.o point.o willson_path.o
	gcc main.o maze.o algorithms.o stack.o random.o tree.o set.o point.o willson_path.o -o main $$(pkg-config allegro-5 allegro_image-5 --libs --cflags)

main.o: main.c maze.h algorithms.h
	gcc -c main.c -o main.o

algorithms.o: algorithms.c stack.h maze.h random.h tree.h set.h willson_path.h
	gcc -c algorithms.c -o algorithms.o

maze.o: maze.c
	gcc -c maze.c -o maze.o

stack.o: stack.c point.h
	gcc -c stack.c -o stack.o

tree.o: tree.c random.h point.h
	gcc -c tree.c -o tree.o

random.o: random.c
	gcc -c random.c -o random.o

set.o: set.c point.h
	gcc -c set.c -o set.o

point.o: point.c
	gcc -c point.c -o point.o

willson_path.o: willson_path.c point.h maze.h
	gcc -c willson_path.c -o willson_path.o

run: main
	./main

debug: main_debug

main_debug: main_debug.o maze_debug.o algorithms_debug.o stack_debug.o random_debug.o tree_debug.o set_debug.o point_debug.o willson_path_debug.o
	gcc -g main_debug.o maze_debug.o algorithms_debug.o stack_debug.o random_debug.o tree_debug.o set_debug.o point_debug.o willson_path_debug.o -o main_debug $$(pkg-config allegro-5 allegro_image-5 --libs --cflags)

main_debug.o: main.c maze.h algorithms.h
	gcc -g -c main.c -o main_debug.o

algorithms_debug.o: algorithms.c stack.h maze.h random.h tree.h set.h
	gcc -g -c algorithms.c -o algorithms_debug.o

maze_debug.o: maze.c
	gcc -g -c maze.c -o maze_debug.o

stack_debug.o: stack.c point.h
	gcc -g -c stack.c -o stack_debug.o

tree_debug.o: tree.c random.h point.h
	gcc -g -c tree.c -o tree_debug.o

random_debug.o: random.c
	gcc -g -c random.c -o random_debug.o

set_debug.o: set.c point.h
	gcc -g -c set.c -o set_debug.o

point_debug.o: point.c
	gcc -g -c point.c -o point_debug.o

willson_path_debug.o: willson_path.c
	gcc -g -c willson_path.c -o willson_path_debug.o

run_debug: main_debug
	gdb ./main_debug

clean:
	rm main
	rm main_debug
	rm *.o
