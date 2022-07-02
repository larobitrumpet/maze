#include <stdlib.h>
#include "maze.h"
#include "stack.h"
#include "algorithms.h"

void update_maze_display();

static inline int between(int lo, int hi)
{
    return lo + (rand() % (hi - lo));
}

int rand_comparison(const void* a, const void* b)
{
    return rand() % 2 ? +1 : -1;
}

void shuffle(void* base, size_t nitems, size_t size)
{
    qsort(base, nitems, size, rand_comparison);
}

void recursive_backtracking_carve_passage_from(MAZE maze, int x, int y)
{
    maze_set_pos(maze, x, y);
    maze_set_special_value(maze, x, y);
    maze_set_visited(maze, x, y);
    update_maze_display();
    enum Direction directions[4] = {up, left, down, right};
    shuffle(directions, 4, sizeof(enum Direction));

    for (int i = 0; i < 4; i++)
    {
        int new_x = x;
        int new_y = y;
        unsigned char valid = 0;
        switch (directions[i])
        {
            case up:
                new_y -= 1;
                if (new_y >= 0)
                    valid = 1;
                break;
            case left:
                new_x += 1;
                if (new_x >= 0)
                    valid = 1;
                break;
            case down:
                new_y += 1;
                if (new_y < maze.height)
                    valid = 1;
                break;
            case right:
                new_x -= 1;
                if (new_x >= maze.width)
                    valid = 1;
                break;
            default:
                break;
        }
        if (valid && !(maze_get_visited(maze, new_x, new_y)))
        {
            maze_carve_passage(maze, directions[i]);
            recursive_backtracking_carve_passage_from(maze, new_x, new_y);
        }
    }
    maze_clear_special_value(maze, x, y);
}

void recursive_backtracking(MAZE maze)
{
    recursive_backtracking_carve_passage_from(maze, 0, 0);
}
