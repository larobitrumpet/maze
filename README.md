# Maze Generation

I have implemented various maze generation algorithms in C using the Allegro game library. All you have to do is give it a width and height, choose an algorithm, and it will start generating a maze.

## Dependencies

This application requires Allegro 5. Instructions for installing Allegro 5 can be found [here](https://github.com/liballeg/allegro_wiki/wiki/Quickstart).

This application uses the `cmake` build system.

## Build

Create a build directory and enter that directory
```
mkdir build
cd build
```

Run `cmake` and point it to the source directory
```
cmake ..
```

Run whatever build system is applicable to you (for example, for Linux this would likely be `make`).
```
make
```

## Usage

You will be asked the dimensions of the maze and what algorithm to use. To choose an algorithm, simply enter the number corresponding to that algorithm.
If you choose the Growing Tree algorithm, you will also be asked to provide weights for the different methods of choosing a cell. For example, if you want the algorithm to choose the newest cell added 50% of the time and choose a random cell 50% of the time, then you would give newest and random weights of 1, and all other methods weights of 0:
```
  Newest: 1
  Random: 1
  Middle: 0
  Oldest: 0
```
If you want to choose the newest cell added 75% of the time, a random cell 25% of the time, you would give newest a weight of 3 and random a weight of 1:
```
  Newest: 3
  Random: 1
  Middle: 0
  Oldest: 0
```
If you only want to choose the newest cell added, then give newest a weight of 1 and everything else a weight of 0:
```
  Newest: 1
  Random: 0
  Middle: 0
  Oldest: 0
```
Choosing only the newest is equivelent to the Recursive Backtracking algorithm and choosing only randomly is equivelent to Prim's algorithm.

You will then be asked to select a tile set. This will determine how the maze looks.

The completed maze will be saved to the file `maze.png`.

## License

This application is licensed under the [MIT License](LICENSE)
