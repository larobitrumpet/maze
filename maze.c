#include <stdlib.h>
#include "maze.h"

MAZE construct_maze(int width, int height)
{
    MAZE maze;
    int size = width * height;
    unsigned char* values = (unsigned char*)malloc(sizeof(unsigned char) * size);
    for (int i = 0; i < size; i++)
        values[i] = 0;
    maze.values = values;
    maze.width = width;
    maze.height = height;
    maze.pos_x = (int*)malloc(sizeof(int));
    maze.pos_y = (int*)malloc(sizeof(int));
    *(maze.pos_x) = -1;
    *(maze.pos_y) = -1;
    return maze;
}

void deconstruct_maze(MAZE maze)
{
    free(maze.values);
    free(maze.pos_x);
    free(maze.pos_y);
}

static inline unsigned char get_maze_value(MAZE maze, int x, int y)
{
    return maze.values[y * maze.height + x];
}

static inline void set_maze_value(MAZE maze, int x, int y, unsigned char val)
{
    maze.values[y * maze.height + x] = val;
}

static inline unsigned char set_bit(unsigned char num, unsigned char bit)
{
    return num | 1 << bit;
}

static inline unsigned char clear_bit(unsigned char num, unsigned char bit)
{
    return num & ~(1 << bit);
}

static inline void set_maze_bit(MAZE maze, int x, int y, unsigned char bit)
{
    set_maze_value(maze, x, y, set_bit(get_maze_value(maze, x, y), bit));
}

static inline void clear_maze_bit(MAZE maze, int x, int y, unsigned char bit)
{
    set_maze_value(maze, x, y, clear_bit(get_maze_value(maze, x, y), bit));
}

void maze_set_up_open(MAZE maze, int x, int y)
{
    set_maze_bit(maze, x, y, 0);
}

void maze_set_up_close(MAZE maze, int x, int y)
{
    clear_maze_bit(maze, x, y, 0);
}

void maze_set_left_open(MAZE maze, int x, int y)
{
    set_maze_bit(maze, x, y, 1);
}

void maze_set_left_close(MAZE maze, int x, int y)
{
    clear_maze_bit(maze, x, y, 1);
}

void maze_set_down_open(MAZE maze, int x, int y)
{
    set_maze_bit(maze, x, y, 2);
}

void maze_set_down_close(MAZE maze, int x, int y)
{
    clear_maze_bit(maze, x, y, 2);
}

void maze_set_right_open(MAZE maze, int x, int y)
{
    set_maze_bit(maze, x, y, 3);
}

void maze_set_right_close(MAZE maze, int x, int y)
{
    clear_maze_bit(maze, x, y, 3);
}

static inline void maze_set_pos_value(MAZE maze, int x, int y)
{
    set_maze_bit(maze, x, y, 4);
}

static inline void maze_clear_pos_value(MAZE maze, int x, int y)
{
    clear_maze_bit(maze, x, y, 4);
}

void maze_set_pos(MAZE maze, int x, int y)
{
    if (*(maze.pos_x) >= 0 && *(maze.pos_y) >= 0)
        maze_clear_pos_value(maze, *(maze.pos_x), *(maze.pos_y));
    if (x >= 0 && y >= 0)
        maze_set_pos_value(maze, x, y);
    *(maze.pos_x) = x;
    *(maze.pos_y) = y;
}

void maze_set_special_value(MAZE maze, int x, int y)
{
    set_maze_bit(maze, x, y, 5);
}

void maze_clear_special_value(MAZE maze, int x, int y)
{
    clear_maze_bit(maze, x, y, 5);
}

void maze_set_visited(MAZE maze, int x, int y)
{
    set_maze_bit(maze, x, y, 6);
}

void maze_clear_visited(MAZE maze, int x, int y)
{
    clear_maze_bit(maze, x, y, 6);
}

unsigned char get_maze_passage_value(MAZE maze, int x, int y)
{
    return get_maze_value(maze, x, y) & 15; // 15 dec == 00001111 bin
}

unsigned char get_maze_type_value(MAZE maze, int x, int y)
{
    unsigned char num = (get_maze_value(maze, x, y) & 48) >> 4; // 48 dec == 00110000 bin
    if (num % 2)
    {
        return clear_bit(num, 1);
    }
    else
    {
        return num;
    }
}
