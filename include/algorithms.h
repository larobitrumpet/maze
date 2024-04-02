#include <stdlib.h>
#include "point.h"
#include "maze.h"
#include "random.h"
#include "tree.h"
#include "set.h"
#include "willson_path.h"
#include "deque.h"

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

// orientation enum used in recursive division
enum Orientation{horizontal, verticle};

// structure to hold edges used in kruskal's algorithm
typedef struct EDGE
{
    int x;
    int y;
    enum Direction dir;
} EDGE;

int update_maze_display();
static unsigned char in_list(int* list, int length, int item);
int recursive_backtracking_carve_passage_from(MAZE maze, int x, int y);
int recursive_backtracking(MAZE maze);
static inline unsigned char eller_same_set(int* row, int x);
static void eller_merge_sets(int* row, int width, int set_from, int set_to);
static void eller_join_right(MAZE maze, int* row);
static void eller_join_down(MAZE maze, int* row, int* next_row);
static int eller_row(MAZE maze, int* row, int* next_row);
static int eller_last_row(MAZE maze, int* row);
int eller(MAZE maze);
static inline TREE* kruskal_get_set(TREE** sets, int x, int y, int maze_w);
int kruskal(MAZE maze);
static void prim_add_neighbors_to_frontier(MAZE maze, SET* frontier);
static void prim_join_from_frontier(MAZE maze);
int prim(MAZE maze);
static enum Orientation recursive_division_pick_orientation(int width, int height);
static int recursive_division_recurse(MAZE maze, int x, int y, int width, int height);
int recursive_division(MAZE maze);
int aldous_broder(MAZE maze);
static WILLSON_PATH* willson_get_willson_path(MAZE maze, SET* not_in_maze, POINT start);
static int willson_follow_willson_path(MAZE maze, SET* not_in_maze, WILLSON_PATH* path);
int willson(MAZE maze);
static int hunt_and_kill_walk(MAZE maze, int x, int y);
static unsigned char hunt_and_kill_unvisited_with_visited_neighbors(MAZE maze, int x, int y);
static POINT hunt_and_kill_hunt(MAZE maze, int start_x, int start_y);
int hunt_and_kill(MAZE maze);
static unsigned char growing_tree_step(MAZE maze, DEQUE* frontier, POINT p);
int growing_tree(MAZE maze, int weights[5]);
int binary_tree(MAZE maze);
int sidewinder(MAZE maze);

#endif
