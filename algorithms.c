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

static inline TREE* kruskal_get_set(TREE** sets, int x, int y, int maze_w)
{
    return sets[y * maze_w + x];
}

void kruskal(MAZE maze)
{
    TREE** sets = (TREE**)malloc(sizeof(TREE*) * maze.width * maze.height);
    for (int y = 0; y < maze.height; y++)
    {
        for (int x = 0; x < maze.width; x++)
        {
            POINT p;
            p.x = x;
            p.y = y;
            sets[y * maze.width + x] = construct_TREE(p);
        }
    }

    int num_edges = maze.width * maze.height * 2 - maze.width - maze.height;
    EDGE** edges = (EDGE**)malloc(sizeof(EDGE*) * num_edges);
    enum Direction dirs[2] = {right, down};
    int i;
    for (int y = 0; y < maze.height - 1; y++)
    {
        for (int x = 0; x < maze.width - 1; x++)
        {
            for (int d = 0; d < 2; d++)
            {
                EDGE* ed = (EDGE*)malloc(sizeof(EDGE));
                ed->x = x;
                ed->y = y;
                ed->dir = dirs[d];
                edges[i] = ed;
                i++;
            }
        }
        int x = maze.width - 1;
        EDGE* ed = (EDGE*)malloc(sizeof(EDGE));
        ed->x = x;
        ed->y = y;
        ed->dir = down;
        edges[i] = ed;
        i++;
    }
    {
    int y = maze.height - 1;
    for (int x = 0; x < maze.width - 1; x++)
    {
        EDGE* ed = (EDGE*)malloc(sizeof(EDGE));
        ed->x = x;
        ed->y = y;
        ed->dir = right;
        edges[i] = ed;
        i++;
    }
    }

    shuffle(edges, num_edges, sizeof(EDGE*));

    maze_set_pos(maze, 0, 0);
    update_maze_display();
    for (int i = 0; i < num_edges; i++)
    {
        int x1 = edges[i]->x;
        int y1 = edges[i]->y;
        int x2;
        int y2;
        switch (edges[i]->dir)
        {
            case right:
                x2 = x1 + 1;
                y2 = y1;
                break;
            case down:
                x2 = x1;
                y2 = y1 + 1;
                break;
            default:
                printf("Something has gone wrong: %p: %d\n", edges[i], edges[i]->dir);
                break;
        }
        maze_set_pos(maze, x1, y1);
        if (!(in_same_tree(kruskal_get_set(sets, x1, y1, maze.width), kruskal_get_set(sets, x2, y2, maze.width))))
        {
            tree_union(kruskal_get_set(sets, x1, y1, maze.width), kruskal_get_set(sets, x2, y2, maze.width));
            maze_carve_passage(maze, edges[i]->dir);
        }
        update_maze_display();
        free(edges[i]);
    }
    free(edges);
}

static void prim_add_neighbors_to_frontier(MAZE maze, SET* frontier)
{
    int x = *(maze.pos_x);
    int y = *(maze.pos_y);
    enum Direction dirs[4] = {up, right, down, left};
    for (int i = 0; i < 4; i++)
    {
        int new_x = x;
        int new_y = y;
        unsigned char valid = 0;
        switch (dirs[i])
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
            POINT p;
            p.x = new_x;
            p.y = new_y;
            set_add(frontier, p);
            maze_set_special_value(maze, new_x, new_y);
        }
    }
}

static void prim_join_from_frontier(MAZE maze)
{
    int x = *(maze.pos_x);
    int y = *(maze.pos_y);
    maze_set_visited(maze, x, y);
    maze_clear_special_value(maze, x, y);
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
        if (valid && maze_get_visited(maze, new_x, new_y))
        {
            maze_carve_passage(maze, directions[i]);
            break;
        }
    }
}

void prim(MAZE maze)
{
    SET* frontier = construct_set();
    maze_set_pos(maze, 0, 0);
    maze_set_visited(maze, 0, 0);
    prim_add_neighbors_to_frontier(maze, frontier);
    update_maze_display();
    while (!(set_is_empty(frontier)))
    {
        POINT p = set_pop_random(frontier);
        maze_set_pos(maze, p.x, p.y);
        prim_join_from_frontier(maze);
        prim_add_neighbors_to_frontier(maze, frontier);
        update_maze_display();
    }
    deconstruct_set(frontier);
}

static enum Orientation recursive_division_pick_orientation(int width, int height)
{
    if (width > height + 1)
        return verticle;
    else if (height > width + 1)
        return horizontal;
    else if (between(0, 2))
        return verticle;
    else
        return horizontal;
}

static void recursive_division_recurse(MAZE maze, int x, int y, int width, int height)
{
    if (width == 1 || height == 1)
        return;
    enum Orientation or = recursive_division_pick_orientation(width, height);
    int wall;
    int i;
    switch (or)
    {
        case verticle:
            wall = between(x, width + x - 2);
            for (i = y; i < height + y; i++)
            {
                maze_set_pos(maze, wall, i);
                maze_fill_passage(maze, right);
            }
            i = between(y, height + y - 1);
            maze_set_pos(maze, wall, i);
            maze_carve_passage(maze, right);
            update_maze_display();
            recursive_division_recurse(maze, x, y, wall + 1 - x, height);
            recursive_division_recurse(maze, wall + 1 , y, width - wall + x - 1, height);
            break;
        case horizontal:
            wall = between(y, height + y - 2);
            for (i = x; i < width + x; i++)
            {
                maze_set_pos(maze, i, wall);
                maze_fill_passage(maze, down);
            }
            i = between(x, width + x - 1);
            maze_set_pos(maze, i, wall);
            maze_carve_passage(maze, down);
            update_maze_display();
            recursive_division_recurse(maze, x, y, width, wall + 1 - y);
            recursive_division_recurse(maze, x, wall + 1, width, height - wall + y - 1);
            break;
        default:
            break;
    }
}

void recursive_division(MAZE maze)
{
    maze.values[0] = 6;
    maze.values[maze.width - 1] = 12;
    maze.values[(maze.height - 1) * maze.width] = 3;
    maze.values[maze.height * maze.width - 1] = 9;
    for (int x = 1; x < maze.width - 1; x++)
    {
        maze.values[x] = 14;
        maze.values[(maze.height - 1) * maze.width + x] = 11;
    }
    for (int y = 1; y < maze.height - 1; y++)
    {
        maze.values[y * maze.width + 0] = 7;
        for (int x = 1; x < maze.width - 1; x++)
        {
            maze.values[y * maze.width + x] = 15;
        }
        maze.values[y * maze.width + maze.width - 1] = 13;
    }
    update_maze_display();
    recursive_division_recurse(maze, 0, 0, maze.width, maze.height);
}
