maze: maze.c
	gcc maze.c -o maze $$(pkg-config allegro-5 allegro_image-5 --libs --cflags)

run:
	./maze

clean:
	rm maze
