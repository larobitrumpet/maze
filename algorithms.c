#include "algorithms.h"

static unsigned char in_list(int* list, int length, int item)
{
    for (int i = 0; i < length; i++)
    {
        if (list[i] == item)
            return 1;
    }
    return 0;
}

void recursive_backtracking_carve_passage_from(MAZE maze, int x, int y)
{
    maze_set_pos(maze, x, y);
    maze_set_special_value(maze, x, y);
    maze_set_visited(maze, x, y);
    update_maze_display();
    enum Direction directions[4] = {up, right, down, left};
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
            case right:
                new_x += 1;
                if (new_x < maze.width)
                    valid = 1;
                break;
            case down:
                new_y += 1;
                if (new_y < maze.height)
                    valid = 1;
                break;
            case left:
                new_x -= 1;
                if (new_x >= 0)
                    valid = 1;
                break;
            default:
                break;
        }
        if (valid && !(maze_get_visited(maze, new_x, new_y)))
        {
            maze_carve_passage(maze, directions[i]);
            recursive_backtracking_carve_passage_from(maze, new_x, new_y);
            maze_set_pos(maze, x, y);
            update_maze_display();
        }
    }
    maze_clear_special_value(maze, x, y);
}

void recursive_backtracking(MAZE maze)
{
    recursive_backtracking_carve_passage_from(maze, 0, 0);
}

static inline unsigned char eller_same_set(int* row, int x)
{
    return row[x] == row[x + 1];
}

static void eller_merge_sets(int* row, int width, int set_from, int set_to)
{
    for (int i = 0; i < width; i++)
    {
        if (row[i] == set_from)
            row[i] = set_to;
    }
}

static void eller_join_right(MAZE maze, int* row)
{
    eller_merge_sets(row, maze.width, row[*(maze.pos_x) + 1], row[*(maze.pos_x)]);
    maze_carve_passage(maze, right);
}

static void eller_join_down(MAZE maze, int* row, int* next_row)
{
    next_row[*(maze.pos_x)] = row[*(maze.pos_x)];
    maze_set_pos(maze, *(maze.pos_x), *(maze.pos_y) + 1);
    maze_carve_passage(maze, up);
}

static int* eller_row(MAZE maze, int* row, int* next_row)
{
    int y = *(maze.pos_y);
    for (int x = 0; x < maze.width - 1; x++)
    {
        maze_set_pos(maze, x, y);
        update_maze_display();
        if (!(eller_same_set(row, x)) && between(0, 2))
            eller_join_right(maze, row);
    }
    maze_set_pos(maze, maze.width - 1, y);
    update_maze_display();
    int sets[maze.width];
    int num_sets = 0;
    for (int i = 0; i < maze.width; i++)
        sets[i] = -1;
    for (int x = 0; x < maze.width; x++)
    {
        if (!(in_list(sets, maze.width, row[x])))
        {
            sets[num_sets] = row[x];
            num_sets++;
        }
    }
    for (int i = 0; i < num_sets; i++)
    {
        int set_indexes[maze.width];
        int num_in_set = 0;
        for (int x = 0; x < maze.width; x++)
        {
            if (row[x] == sets[i])
            {
                set_indexes[num_in_set] = x;
                num_in_set++;
            }
        }
        shuffle(set_indexes, num_in_set, sizeof(int));
        int last = between(1, num_in_set + 1);
        for (int j = 0; j < last; j++)
        {
            maze_set_pos(maze, set_indexes[j], y);
            eller_join_down(maze, row, next_row);
            update_maze_display();
        }
    }
}

static void eller_last_row(MAZE maze, int* row)
{
    int y = *(maze.pos_y);
    for (int x = 0; x < maze.width - 1; x++)
    {
        maze_set_pos(maze, x, y);
        update_maze_display();
        if (!(eller_same_set(row, x)))
            eller_join_right(maze, row);
    }
    maze_set_pos(maze, maze.width - 1, y);
    update_maze_display();
}

void eller(MAZE maze)
{
    int next_set = 0;
    int* row = (int*)malloc(sizeof(int) * maze.width);
    for (int x = 0; x < maze.width; x++)
    {
        row[x] = next_set;
        next_set++;
    }
    for (int y = 0; y < maze.height - 1; y++)
    {
        maze_set_pos(maze, 0, y);
        int* next_row = (int*)malloc(sizeof(int) * maze.width);
        for (int x = 0; x < maze.width; x++)
        {
            next_row[x] = next_set;
            next_set++;
        }
        eller_row(maze, row, next_row);
        free(row);
        row = next_row;
    }
    maze_set_pos(maze, 0, maze.height - 1);
    eller_last_row(maze, row);
    free(row);
}
