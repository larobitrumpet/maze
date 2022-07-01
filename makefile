main: main.c
	gcc main.c -o main $$(pkg-config allegro-5 allegro_image-5 --libs --cflags)

run:
	./main

clean:
	rm main
