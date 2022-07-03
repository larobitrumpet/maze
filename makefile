main: main.o maze.o algorithms.o stack.o
	gcc main.o maze.o algorithms.o stack.o -o main $$(pkg-config allegro-5 allegro_image-5 --libs --cflags)

main.o: main.c
	gcc -c main.c -o main.o

algorithms.o: algorithms.c stack.o maze.o
	gcc -c algorithms.c -o algorithms.o

maze.o: maze.c
	gcc -c maze.c -o maze.o

stack.o: stack.c
	gcc -c stack.c -o stack.o

run: main
	./main

debug: main_debug

main_debug: main_debug.o maze_debug.o algorithms_debug.o stack_debug.o
	gcc -g main_debug.o maze_debug.o algorithms_debug.o stack_debug.o -o main_debug $$(pkg-config allegro-5 allegro_image-5 --libs --cflags)

main_debug.o: main.c
	gcc -g -c main.c -o main_debug.o

algorithms_debug.o: algorithms.c stack_debug.o maze_debug.o
	gcc -g -c algorithms.c -o algorithms_debug.o

maze_debug.o: maze.c
	gcc -g -c maze.c -o maze_debug.o

stack_debug.o: stack.c
	gcc -g -c stack.c -o stack_debug.o

run_debug: main_debug
	gdb ./main_debug

clean:
	rm main
	rm main_debug
	rm *.o
