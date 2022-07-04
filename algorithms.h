#include <stdlib.h>
#include "point.h"
#include "maze.h"
#include "stack.h"
#include "random.h"
#include "tree.h"

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

// structure to hold edges used in kruskal's algorithm
typedef struct EDGE
{
    int x;
    int y;
    enum Direction dir;
} EDGE;

void update_maze_display();
static unsigned char in_list(int* list, int length, int item);
void recursive_backtracking_carve_passage_from(MAZE maze, int x, int y);
void recursive_backtracking(MAZE maze);
static inline unsigned char eller_same_set(int* row, int x);
static void eller_merge_sets(int* row, int width, int set_from, int set_to);
static void eller_join_right(MAZE maze, int* row);
static void eller_join_down(MAZE maze, int* row, int* next_row);
static int* eller_row(MAZE maze, int* row, int* next_row);
static void eller_last_row(MAZE maze, int* row);
void eller(MAZE maze);
static inline TREE* kruskal_get_set(TREE** sets, int x, int y, int maze_w);
void kruskal(MAZE maze);

#endif
