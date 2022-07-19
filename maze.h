#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "set.h"

#ifndef MAZE_H
#define MAZE_H

enum Direction {up, right, down, left};

typedef struct MAZE
{
    unsigned char* values;
    int* pos_x;
    int* pos_y;
    int width;
    int height;
    SET* update;
} MAZE;

MAZE construct_maze(int width, int height);
void deconstruct_maze(MAZE maze);
static inline unsigned char get_maze_value(MAZE maze, int x, int y);
static inline void set_maze_value(MAZE maze, int x, int y, unsigned char val);
static inline unsigned char set_bit(unsigned char num, unsigned char bit);
static inline unsigned char clear_bit(unsigned char num, unsigned char bit);
static inline void set_maze_bit(MAZE maze, int x, int y, unsigned char bit);
static inline void clear_maze_bit(MAZE, int x, int y, unsigned char bit);
static inline void maze_set_open(MAZE maze, int x, int y, enum Direction dir);
static inline void maze_set_close(MAZE maze, int x, int y, enum Direction dir);
void maze_carve_passage(MAZE maze, enum Direction dir);
void maze_fill_passage(MAZE maze, enum Direction dir);
static inline void maze_set_pos_value(MAZE maze, int x, int y);
static inline void maze_clear_pose_value(MAZE maze, int x, int y);
void maze_set_pos(MAZE maze, int x, int y);
void maze_set_special_value(MAZE maze, int x, int y);
void maze_clear_special_value(MAZE maze, int x, int y);
void maze_set_visited(MAZE maze, int x, int y);
void maze_clear_visited(MAZE maze, int x, int y);
unsigned char maze_get_visited(MAZE maze, int x, int y);
unsigned char get_maze_passage_value(MAZE maze, int x, int y);
unsigned char get_maze_type_value(MAZE maze, int x, int y);
void print_maze(MAZE maze);

#endif
