#include "point.h"
#include "maze.h"

#ifndef WILLSON_PATH_H
#define WILLSON_PATH_H

#define WILLSON_PATH_SIZE_INIT 16
#define WILLSON_PATH_SIZE_INCREASE 1.5

typedef struct WILLSON_PATH
{
    POINT* points;
    enum Direction* dirs;
    int length;
    int size;
} WILLSON_PATH;

WILLSON_PATH* construct_willson_path();
void deconstruct_willson_path(WILLSON_PATH* path);
int willson_path_get_index(WILLSON_PATH* path, POINT p);
static void willson_path_increase_size(WILLSON_PATH* path);
void willson_path_add(MAZE maze, WILLSON_PATH* path, POINT p, enum Direction dir);

#endif
