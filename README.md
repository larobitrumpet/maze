# Maze Generation

I have implemented various maze generation algorithms in C using the Allegro game library. All you have to do is give it a width and height, choose an algorithm, and it will start generating a maze.

## Dependencies

This application requires Allegro 5. Instructions for installing Allegro 5 can be found [here](https://github.com/liballeg/allegro_wiki/wiki/Quickstart).

This application also requires `gcc` or some other way of compiling C (the makefile uses `gcc`).

`make` is recommended.

## Build

To build on Linux, run `make`.

To build on other platforms, [this](https://github.com/liballeg/allegro_wiki/wiki/Quickstart) will help you figure out how to build Allegro applicaions. This application uses the image addon.

## Run

If you have `make`, you can run `make run`. You can also run the binary executable `./main` or `./main.exe` on Windows.

## License

This application is licensed under the [MIT License](LICENSE)