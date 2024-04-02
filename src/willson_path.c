#include "willson_path.h"

WILLSON_PATH* construct_willson_path()
{
    WILLSON_PATH* path = (WILLSON_PATH*)malloc(sizeof(WILLSON_PATH));
    path->points = (POINT*)malloc(sizeof(POINT) * WILLSON_PATH_SIZE_INIT);
    path->dirs = (enum Direction*)malloc(sizeof(enum Direction) * WILLSON_PATH_SIZE_INIT);
    path->length = 0;
    path->size = WILLSON_PATH_SIZE_INIT;
    return path;
}

void deconstruct_willson_path(WILLSON_PATH* path)
{
    free(path->points);
    free(path->dirs);
    free(path);
}

int willson_path_get_index(WILLSON_PATH* path, POINT p)
{
    for (int i = 0; i < path->length; i++)
    {
        if (points_are_equal(path->points[i], p))
            return i;
    }
    return -1;
}

static void willson_path_increase_size(WILLSON_PATH* path)
{
    path->size = (int)(path->size * WILLSON_PATH_SIZE_INCREASE);
    path->points = (POINT*)realloc(path->points, sizeof(POINT) * path->size);
    path->dirs = (enum Direction*)realloc(path->dirs, sizeof(enum Direction) * path->size);
}

void willson_path_add(MAZE maze, WILLSON_PATH* path, POINT p, enum Direction dir)
{
    int index = willson_path_get_index(path, p);
    if (index >= 0)
    {
        for (int i = index + 1; i < path->length; i++)
        {
            maze_clear_special_value(maze, path->points[i].x, path->points[i].y);
        }
        path->dirs[index] = dir;
        path->length = index + 1;
    }
    else
    {
        if (path->length == path->size)
            willson_path_increase_size(path);
        path->points[path->length] = p;
        path->dirs[path->length] = dir;
        path->length++;
        maze_set_special_value(maze, p.x, p.y);
    }
}
