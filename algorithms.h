#ifndef ALGORITHMS_H
#define ALGORITHMS_H

void update_maze_display();
static inline int between(int lo, int hi);
int rand_comparison(const void* a, const void* b);
void shuffle(void* base, size_t nitems, size_t size);
void recursive_backtracking_carve_passage_from(MAZE maze, int x, int y);
void recursive_backtracking(MAZE maze);

#endif
