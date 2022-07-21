#include <stdlib.h>

#ifndef RANDOM_H
#define RANDOM_H

int between(int lo, int hi);
int rand_comparison(const void* a, const void* b);
void shuffle(void* base, size_t nitems, size_t size);
int rand_weighted(int* weights, int size);

#endif
