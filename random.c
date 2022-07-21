#include "random.h"

int between(int lo, int hi)
{
    if (lo == hi)
        return lo;
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

// Returns a random number bases on weights.
// `int* weights` is an array of weights with
// the last element being the sum of the weights.
// A number corresponding to the index of the
// weight is returned.
int rand_weighted(int* weights, int size)
{
    if (weights[size - 1] <= 0)
        return -1;
    if (weights[size - 1] == 1)
    {
        for (int i = 0; i < weights[size]; i++)
        {
            if (weights[i] == 1)
                return i;
        }
        return -1;
    }
    int rand = between(0, weights[size - 1]);
    int sum = 0;
    int i;
    for (i = 0; i < size - 2; i++)
    {
        if (rand < weights[i] + sum)
            return i;
        sum += weights[i];
    }
    return i;
}
