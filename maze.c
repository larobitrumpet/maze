#include "maze.h"

MAZE construct_maze(int width, int height, unsigned char wall_adder)
{
    MAZE maze;
    int size = width * height;
    unsigned char* values = (unsigned char*)malloc(sizeof(unsigned char) * size);
    if (wall_adder)
    {
        values[0] = 6;
        values[width - 1] = 12;
        values[(height - 1) * width] = 3;
        values[height * width - 1] = 9;
        for (int x = 1; x < width - 1; x++)
        {
            values[x] = 14;
            values[(height - 1) * width + x] = 11;
        }
        for (int y = 1; y < height - 1; y++)
        {
            values[y * width + 0] = 7;
            for (int x = 1; x < width - 1; x++)
            {
                values[y * width + x] = 15;
            }
            values[y * width + width - 1] = 13;
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
            values[i] = 0;
    }
    maze.values = values;
    maze.width = width;
    maze.height = height;
    maze.pos_x = (int*)malloc(sizeof(int));
    maze.pos_y = (int*)malloc(sizeof(int));
    *(maze.pos_x) = -1;
    *(maze.pos_y) = -1;
    maze.update = construct_set();
    return maze;
}

void deconstruct_maze(MAZE maze)
{
    deconstruct_set(maze.update);
    free(maze.values);
    free(maze.pos_x);
    free(maze.pos_y);
}

static inline unsigned char get_maze_value(MAZE maze, int x, int y)
{
    return maze.values[y * maze.width + x];
}

static inline void set_maze_value(MAZE maze, int x, int y, unsigned char val)
{
    maze.values[y * maze.width + x] = val;
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

static inline void maze_set_open(MAZE maze, int x, int y, enum Direction dir)
{
    set_maze_bit(maze, x, y, dir);
}

static inline void maze_set_close(MAZE maze, int x, int y, enum Direction dir)
{
    clear_maze_bit(maze, x, y, dir);
}

void maze_carve_passage(MAZE maze, enum Direction dir)
{
    POINT p;
    p.x = *(maze.pos_x);
    p.y = *(maze.pos_y);
    set_add(maze.update, p);
    maze_set_open(maze, p.x, p.y, dir);
    enum Direction op_dir;
    switch (dir)
    {
        case up:
            p.y--;
            op_dir = down;
            break;
        case right:
            p.x++;
            op_dir = left;
            break;
        case down:
            p.y++;
            op_dir = up;
            break;
        case left:
            p.x--;
            op_dir = right;
            break;
        default:
            break;
    }
    maze_set_open(maze, p.x, p.y, op_dir);
    set_add(maze.update, p);
}

void maze_fill_passage(MAZE maze, enum Direction dir)
{
    POINT p;
    p.x = *(maze.pos_x);
    p.y = *(maze.pos_y);
    set_add(maze.update, p);
    maze_set_close(maze, p.x, p.y, dir);
    enum Direction op_dir;
    switch (dir)
    {
        case up:
            p.y--;
            op_dir = down;
            break;
        case right:
            p.x++;
            op_dir = left;
            break;
        case down:
            p.y++;
            op_dir = up;
            break;
        case left:
            p.x--;
            op_dir = right;
            break;
        default:
            break;
    }
    maze_set_close(maze, p.x, p.y, op_dir);
    set_add(maze.update, p);
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
    POINT p;
    if (*(maze.pos_x) >= 0 && *(maze.pos_y) >= 0)
    {
        maze_clear_pos_value(maze, *(maze.pos_x), *(maze.pos_y));
        p.x = *(maze.pos_x);
        p.y = *(maze.pos_y);
        set_add(maze.update, p);
    }
    if (x >= 0 && y >= 0)
    {
        maze_set_pos_value(maze, x, y);
        p.x = x;
        p.y = y;
        set_add(maze.update, p);
    }
    *(maze.pos_x) = x;
    *(maze.pos_y) = y;
}

void maze_set_special_value(MAZE maze, int x, int y)
{
    set_maze_bit(maze, x, y, 5);
    POINT p;
    p.x = x;
    p.y = y;
    set_add(maze.update, p);
}

void maze_clear_special_value(MAZE maze, int x, int y)
{
    clear_maze_bit(maze, x, y, 5);
    POINT p;
    p.x = x;
    p.y = y;
    set_add(maze.update, p);
}

void maze_set_visited(MAZE maze, int x, int y)
{
    set_maze_bit(maze, x, y, 6);
}

void maze_clear_visited(MAZE maze, int x, int y)
{
    clear_maze_bit(maze, x, y, 6);
}

unsigned char maze_get_visited(MAZE maze, int x, int y)
{
    return get_maze_value(maze, x, y) & 64; // 64 dec == 01000000 bin
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

void print_maze(MAZE maze)
{
    for (int y = 0; y < maze.height; y++)
    {
        for (int x = 0; x < maze.width; x++)
        {
            printf("%0 2x ", get_maze_value(maze, x, y));
        }
        printf("\n");
    }
}
