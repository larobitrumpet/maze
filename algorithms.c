#include <stdio.h>
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

int recursive_backtracking_carve_passage_from(MAZE maze, int x, int y)
{
    maze_set_pos(maze, x, y);
    maze_set_special_value(maze, x, y);
    maze_set_visited(maze, x, y);
    if (update_maze_display())
        return 1;
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
            if (recursive_backtracking_carve_passage_from(maze, new_x, new_y))
                return 1;
            maze_set_pos(maze, x, y);
            if (update_maze_display())
                return 1;
        }
    }
    maze_clear_special_value(maze, x, y);
    return 0;
}

int recursive_backtracking(MAZE maze)
{
    if (recursive_backtracking_carve_passage_from(maze, 0, 0))
        return 1;
    return 0;
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

static int eller_row(MAZE maze, int* row, int* next_row)
{
    int y = *(maze.pos_y);
    for (int x = 0; x < maze.width - 1; x++)
    {
        maze_set_pos(maze, x, y);
        if (update_maze_display())
            return 1;
        if (!(eller_same_set(row, x)) && between(0, 2))
            eller_join_right(maze, row);
    }
    maze_set_pos(maze, maze.width - 1, y);
    if (update_maze_display())
        return 1;
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
            if (update_maze_display())
                return 1;
        }
    }

    return 0;
}

static int eller_last_row(MAZE maze, int* row)
{
    int y = *(maze.pos_y);
    for (int x = 0; x < maze.width - 1; x++)
    {
        maze_set_pos(maze, x, y);
        if (update_maze_display())
            return 1;
        if (!(eller_same_set(row, x)))
            eller_join_right(maze, row);
    }
    maze_set_pos(maze, maze.width - 1, y);
    if (update_maze_display())
        return 1;

    return 0;
}

int eller(MAZE maze)
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
        if (eller_row(maze, row, next_row))
        {
            free(row);
            free(next_row);
            return 1;
        }
        free(row);
        row = next_row;
    }
    maze_set_pos(maze, 0, maze.height - 1);
    if (eller_last_row(maze, row))
    {
        free(row);
        return 1;
    }
    free(row);

    return 0;
}

static inline TREE* kruskal_get_set(TREE** sets, int x, int y, int maze_w)
{
    return sets[y * maze_w + x];
}

int kruskal(MAZE maze)
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
    int i = 0;
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
    if (update_maze_display())
    {
        int size = maze.width * maze.height;
        for (int i = 0; i < size; i++)
            deconstruct_TREE(sets[i]);
        free(sets);
        for (int i = 0; i < num_edges; i++)
            free(edges[i]);
        free(edges);
        return 1;
    }
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
        if (update_maze_display())
        {
            int size = maze.width * maze.height;
            for (int j = 0; j < size; j++)
                free(sets[j]);
            free(sets);
            for (int j = i; j < num_edges; j++)
                free(edges[j]);
            free(edges);
            return 1;
        }
        free(edges[i]);
    }

    free(edges);
    deconstruct_TREE(get_root(sets[0]));
    free(sets);

    return 0;
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

int prim(MAZE maze)
{
    SET* frontier = construct_set();
    maze_set_pos(maze, 0, 0);
    maze_set_visited(maze, 0, 0);
    prim_add_neighbors_to_frontier(maze, frontier);
    if (update_maze_display())
    {
        deconstruct_set(frontier);
        return 1;
    }
    while (!(set_is_empty(frontier)))
    {
        POINT p = set_pop_random(frontier);
        maze_set_pos(maze, p.x, p.y);
        prim_join_from_frontier(maze);
        prim_add_neighbors_to_frontier(maze, frontier);
        if (update_maze_display())
        {
            deconstruct_set(frontier);
            return 1;
        }
    }
    deconstruct_set(frontier);

    return 0;
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

static int recursive_division_recurse(MAZE maze, int x, int y, int width, int height)
{
    if (width == 1 || height == 1)
        return 0;
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
            if (update_maze_display())
                return 1;
            if (recursive_division_recurse(maze, x, y, wall + 1 - x, height))
                return 1;
            if (recursive_division_recurse(maze, wall + 1 , y, width - wall + x - 1, height))
                return 1;
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
            if (update_maze_display())
                return 1;
            if (recursive_division_recurse(maze, x, y, width, wall + 1 - y))
                return 1;
            if (recursive_division_recurse(maze, x, wall + 1, width, height - wall + y - 1))
                return 1;
            break;
        default:
            break;
    }

    return 0;
}

int recursive_division(MAZE maze)
{
    if(update_maze_display())
        return 1;
    if (recursive_division_recurse(maze, 0, 0, maze.width, maze.height))
        return 1;

    return 0;
}

int aldous_broder(MAZE maze)
{
    int remaining = maze.width * maze.height;
    maze_set_pos(maze, 0, 0);
    maze_set_visited(maze, 0, 0);
    remaining--;
    while (remaining)
    {
        enum Direction directions[4] = {up, right, down, left};
        shuffle(directions, 4, sizeof(enum Direction));

        for (int i = 0; i < 4; i++)
        {
            int new_x = *(maze.pos_x);
            int new_y = *(maze.pos_y);
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
            if (valid)
            {
                if (!(maze_get_visited(maze, new_x, new_y)))
                {
                    maze_carve_passage(maze, directions[i]);
                    maze_set_visited(maze, new_x, new_y);
                    remaining--;
                }
                maze_set_pos(maze, new_x, new_y);
                if (update_maze_display())
                    return 1;
                break;
            }
        }
    }

    return 0;
}

static WILLSON_PATH* willson_get_willson_path(MAZE maze, SET* not_in_maze, POINT start)
{
    WILLSON_PATH* path = construct_willson_path();
    POINT current = start;
    while (in_set(not_in_maze, current))
    {
        int new_x;
        int new_y;
        int i = 0;
        enum Direction directions[4] = {up, right, down, left};
        shuffle(directions, 4, sizeof(enum Direction));

        for (i = 0; i < 4; i++)
        {
            unsigned char valid = 0;
            new_x = current.x;
            new_y = current.y;
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
            if (valid)
                break;
        }
        willson_path_add(maze, path, current, directions[i]);
        current.x = new_x;
        current.y = new_y;
        maze_set_pos(maze, current.x, current.y);
        if (update_maze_display())
        {
            free(path);
            return NULL;
        }
    }
    return path;
}

static int willson_follow_willson_path(MAZE maze, SET* not_in_maze, WILLSON_PATH* path)
{
    for (int i = 0; i < path->length; i++)
    {
        maze_set_pos(maze, path->points[i].x, path->points[i].y);
        maze_carve_passage(maze, path->dirs[i]);
        maze_clear_special_value(maze, path->points[i].x, path->points[i].y);
        set_remove(not_in_maze, path->points[i]);
        if (update_maze_display())
            return 1;
    }

    return 0;
}

int willson(MAZE maze)
{
    SET* not_in_maze = construct_set();
    for (int y = 0; y < maze.height; y++)
    {
        for (int x = 0; x < maze.width; x++)
        {
            POINT p;
            p.x = x;
            p.y = y;
            set_add(not_in_maze, p);
        }
    }
    {
        POINT init = set_pop_random(not_in_maze);
        maze_set_pos(maze, init.x, init.y);
        if (update_maze_display())
        {
            deconstruct_set(not_in_maze);
            return 1;
        }
    }
    while (!(set_is_empty(not_in_maze)))
    {
        POINT start = set_peak_random(not_in_maze);
        WILLSON_PATH* path = willson_get_willson_path(maze, not_in_maze, start);
        if (path == NULL)
        {
            deconstruct_set(not_in_maze);
            return 1;
        }
        if (willson_follow_willson_path(maze, not_in_maze, path))
        {
            deconstruct_willson_path(path);
            deconstruct_set(not_in_maze);
            return 1;
        }
        deconstruct_willson_path(path);
    }
    deconstruct_set(not_in_maze);

    return 0;
}

static int hunt_and_kill_walk(MAZE maze, int x, int y)
{
    maze_set_pos(maze, x, y);
    maze_set_visited(maze, x, y);
    if (update_maze_display())
        return 1;
    while (1)
    {
        enum Direction directions[4] = {up, right, down, left};
        shuffle(directions, 4, sizeof(enum Direction));

        unsigned char dead_end = 1;
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
                dead_end = 0;
                maze_carve_passage(maze, directions[i]);
                x = new_x;
                y = new_y;
                maze_set_pos(maze, x, y);
                maze_set_visited(maze, x, y);
                if (update_maze_display())
                    return 1;
                break;
            }
        }
        if (dead_end)
            break;
    }

    return 0;
}

static unsigned char hunt_and_kill_unvisited_with_visited_neighbors(MAZE maze, int x, int y)
{
    if (maze_get_visited(maze, x, y))
        return 0;
    
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
            maze_set_pos(maze, x, y);
            maze_set_visited(maze, x, y);
            maze_carve_passage(maze, directions[i]);
            return 1;
        }
    }

    return 0;
}

static POINT hunt_and_kill_hunt(MAZE maze, int start_x, int start_y)
{
    POINT p;
    p.x = -1;
    p.y = -1;
    for (int x = start_x; x < maze.width; x++)
    {
        maze_set_pos(maze, x, start_y);
        if (update_maze_display())
        {
            p.x = -2;
            p.y = -2;
            return p;
        }
        if (hunt_and_kill_unvisited_with_visited_neighbors(maze, x, start_y))
        {
            if (update_maze_display())
            {
                p.x = -2;
                p.y = -2;
                return p;
            }
            p.x = x;
            p.y = start_y;
            return p;
        }
    }
    for (int y = start_y + 1; y < maze.height; y++)
    {
        for (int x = 0; x < maze.width; x++)
        {
            maze_set_pos(maze, x, y);
            if (update_maze_display())
            {
                p.x = -2;
                p.y = -2;
                return p;
            }
            if (hunt_and_kill_unvisited_with_visited_neighbors(maze, x, y))
            {
                if (update_maze_display())
                {
                    p.x = -2;
                    p.y = -2;
                    return p;
                }
                p.x = x;
                p.y = y;
                return p;
            }
        }
    }
    return p;
}

int hunt_and_kill(MAZE maze)
{
    int x = 0;
    int y = 0;
    while (1)
    {
        if (hunt_and_kill_walk(maze, x, y))
            return 1;
        POINT p = hunt_and_kill_hunt(maze, x + 1, y);
        if (p.x == -2 || p.y == -2)
            return 1;
        if (p.x == -1 || p.y == -1)
            return 0;
        x = p.x;
        y = p.y;
    }

    return 0;
}

static unsigned char growing_tree_step(MAZE maze, DEQUE* frontier, POINT p)
{
    maze_set_pos(maze, p.x, p.y);

    enum Direction directions[4] = {up, right, down, left};
    shuffle(directions, 4, sizeof(enum Direction));

    for (int i = 0; i < 4; i++)
    {
        int new_x = p.x;
        int new_y = p.y;
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
            maze_set_special_value(maze, new_x, new_y);
            maze_set_visited(maze, new_x, new_y);
            POINT p_add = {new_x, new_y};
            deque_push_back(frontier, p_add);
            return 0;
        }
    }
    maze_clear_special_value(maze, p.x, p.y);
    return 1;
}

int growing_tree(MAZE maze, int weights[5])
{
    DEQUE* frontier = construct_deque();
    POINT p = {0, 0};
    maze_set_special_value(maze, 0, 0);
    maze_set_visited(maze, 0, 0);
    deque_push_back(frontier, p);
    while (!(deque_is_empty(frontier)))
    {
        int pop_type = rand_weighted(weights, 5);
        switch (pop_type)
        {
            case 0:
                p = deque_peak_front(frontier);
                if (growing_tree_step(maze, frontier, p))
                    deque_pop_front(frontier);
                break;
            case 1:
                p = deque_peak_back(frontier);
                if (growing_tree_step(maze, frontier, p))
                    deque_pop_back(frontier);
                break;
            case 2:
                p = deque_peak_middle(frontier);
                if (growing_tree_step(maze, frontier, p))
                    deque_pop_middle(frontier);
                break;
            case 3:
                deque_choose_random(frontier);
                p = deque_peak_random(frontier);
                if (growing_tree_step(maze, frontier, p))
                    deque_pop_random(frontier);
                break;
            default:
                break;
        }
        if (update_maze_display())
        {
            deconstruct_deque(frontier);
            return 1;
        }
    }
    deconstruct_deque(frontier);
    return 0;
}

int binary_tree(MAZE maze)
{
    enum Direction dirs[2] = {up, left};
    for (int x = 1; x < maze.width; x++)
    {
        maze_set_pos(maze, x, 0);
        maze_carve_passage(maze, left);
        if (update_maze_display())
            return 1;
    }
    for (int y = 1; y < maze.height; y++)
    {
        maze_set_pos(maze, 0, y);
        maze_carve_passage(maze, up);
        if (update_maze_display())
            return 1;
        for (int x = 1; x < maze.width; x++)
        {
            maze_set_pos(maze, x, y);
            maze_carve_passage(maze, dirs[between(0, 2)]);
            if (update_maze_display())
                return 1;
        }
    }
    
    return 0;
}

int sidewinder(MAZE maze)
{
    for (int x = 0; x < maze.width - 1; x++)
    {
        maze_set_pos(maze, x, 0);
        maze_carve_passage(maze, right);
        if (update_maze_display())
            return 1;
    }
    for (int y = 1; y < maze.height; y++)
    {
        int run_start = 0;
        for (int x = 0; x < maze.width - 1; x++)
        {
            if (between(0, 2))
            {
                maze_set_pos(maze, x, y);
                maze_carve_passage(maze, right);
            }
            else
            {
                maze_set_pos(maze, between(run_start, x + 1), y);
                maze_carve_passage(maze, up);
                run_start = x + 1;
            }
            if (update_maze_display())
                return 1;
        }
        maze_set_pos(maze, between(run_start, maze.width), y);
        maze_carve_passage(maze, up);
        if (update_maze_display())
            return 1;
    }

    return 0;
}
