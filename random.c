#include "random.h"

int between(int lo, int hi)
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
