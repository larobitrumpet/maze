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

#endif
