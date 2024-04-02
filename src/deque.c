#include "deque.h"

static inline int mod(int a, int b) {
    return ((a % b) + b) % b;
}

DEQUE* construct_deque()
{
    DEQUE* deque = (DEQUE*)malloc(sizeof(DEQUE));
    POINT* arr = (POINT*)malloc(sizeof(POINT) * DEQUE_INIT_SIZE);
    deque->arr = arr;
    deque->size = DEQUE_INIT_SIZE;
    deque->front = 0;
    deque->back = 0;
    deque->random = 0;
}

void deconstruct_deque(DEQUE* deque)
{
    free(deque->arr);
    free(deque);
}

unsigned char deque_is_empty(DEQUE* deque)
{
    return deque->front == deque->back;
}

int deque_length(DEQUE* deque)
{
    return mod(deque->back - deque->front, deque->size);
}

static void deque_increase_size(DEQUE* deque)
{
    int old_size = deque->size;
    deque->size = (int)(old_size * DEQUE_SIZE_INCREASE);
    POINT* new_arr = (POINT*)malloc(sizeof(POINT) * deque->size);
    int i = deque->front;
    int j = 0;
    while (i != deque->back)
    {
        new_arr[j] = deque->arr[i];
        i = mod(i + 1, old_size);
        j++;
    }
    free(deque->arr);
    deque->arr = new_arr;
    deque->front = 0;
    deque->back = j;
}

// Choose a random index that will be used in
// `deque_peak_random()` and `deque_pop_random()`.
// This is so calling `deque_peak_random()` and then
// `deque_pop_random()` uses the same index both times
// instead of generating a new index. This is so
// we can look at a random index and then decide
// if we want to pop it or not.
void deque_choose_random(DEQUE* deque)
{
    deque->random = between(0, deque_length(deque));
}

void deque_push_front(DEQUE* deque, POINT p)
{
    if (mod(deque->front - 1, deque->size) == deque->back)
        deque_increase_size(deque);
    deque->front = mod(deque->front - 1, deque->size);
    deque->arr[deque->front] = p;
    // Keep index of random consistent
    deque->random = mod(deque->random - 1, deque->size);
}

void deque_push_back(DEQUE* deque, POINT p)
{
    if (deque->front == mod(deque->back + 1, deque->size))
        deque_increase_size(deque);
    deque->arr[deque->back] = p;
    deque->back = mod(deque->back + 1, deque->size);
}

POINT deque_peak_front(DEQUE* deque)
{
    if (deque_is_empty(deque))
    {
        POINT p = {-1, -1};
        return p;
    }
    return deque->arr[deque->front];
}

POINT deque_peak_back(DEQUE* deque)
{
    if (deque_is_empty(deque))
    {
        POINT p = {-1, -1};
        return p;
    }
    return deque->arr[mod(deque->back - 1, deque->size)];
}

POINT deque_peak_middle(DEQUE* deque)
{
    if (deque_is_empty(deque))
    {
        POINT p = {-1, -1};
        return p;
    }
    return deque->arr[mod((int)(deque_length(deque) / 2) + deque->front, deque->size)];
}

// Peak at the value chosen with `deque_choose_random()`
POINT deque_peak_random(DEQUE* deque)
{
    if (deque_is_empty(deque))
    {
        POINT p = {-1, -1};
        return p;
    }
    return deque->arr[mod(deque->random + deque->front, deque->size)];
}

POINT deque_pop_front(DEQUE* deque)
{
    if (deque_is_empty(deque))
    {
        POINT p = {-1, -1};
        return p;
    }
    POINT p = deque_peak_front(deque);
    deque->front = mod(deque->front + 1, deque->size);
    return p;
}

POINT deque_pop_back(DEQUE* deque)
{
    if (deque_is_empty(deque))
    {
        POINT p = {-1, -1};
        return p;
    }
    POINT p = deque_peak_back(deque);
    deque->back = mod(deque->back - 1, deque->size);
    return p;
}

POINT deque_pop_middle(DEQUE* deque)
{
    if (deque_is_empty(deque))
    {
        POINT p = {-1, -1};
        return p;
    }
    POINT p = deque_peak_middle(deque);
    int i = mod((int)(deque_length(deque) / 2) + deque->front, deque->size);
    int end = mod(deque->back - 1, deque->size);
    while (i != end)
    {
        int j = mod(i + 1, deque->size);
        deque->arr[i] = deque->arr[j];
        i = j;
    }
    deque->back = end;
    return p;
}

// Pop value chosen with `deque_choose_random()`
POINT deque_pop_random(DEQUE* deque)
{
    if (deque_is_empty(deque))
    {
        POINT p = {-1, -1};
        return p;
    }
    POINT p = deque_peak_random(deque);
    int i = mod(deque->random + deque->front, deque->size);
    int end = mod(deque->back - 1, deque->size);
    while (i != end)
    {
        int j = mod(i + 1, deque->size);
        deque->arr[i] = deque->arr[j];
        i = j;
    }
    deque->back = end;
    return p;
}
