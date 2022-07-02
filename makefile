main: main.o maze.o
	gcc main.o maze.o -o main $$(pkg-config allegro-5 allegro_image-5 --libs --cflags)

main.o: main.c
	gcc -c main.c -o main.o $$(pkg-config allegro-5 allegro_image-5 --libs --cflags)

maze.o: maze.c
	gcc -c maze.c -o maze.o

stack.o: stack.c
	gcc -c stack.c -o stack.o

run:
	./main

clean:
	rm main
