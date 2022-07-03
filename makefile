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

clean:
	rm main
