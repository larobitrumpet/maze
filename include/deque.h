#include "point.h"
#include "random.h"

#ifndef DEQUE_H
#define DEQUE_H

#define DEQUE_INIT_SIZE 16
#define DEQUE_SIZE_INCREASE 1.5

typedef struct DEQUE
{
    POINT* arr;
    int size;
    int front;
    int back;
    int random;
} DEQUE;

static inline int mod(int a, int b);
DEQUE* construct_deque();
void deconstruct_deque(DEQUE* deque);
unsigned char deque_is_empty(DEQUE* deque);
int deque_length(DEQUE* deque);
static void deque_increase_size(DEQUE* deque);
void deque_choose_random(DEQUE* deque);
void deque_push_front(DEQUE* deque, POINT p);
void deque_push_back(DEQUE* deque, POINT p);
POINT deque_peak_front(DEQUE* deque);
POINT deque_peak_back(DEQUE* deque);
POINT deque_peak_middle(DEQUE* deque);
POINT deque_peak_random(DEQUE* deque);
POINT deque_pop_front(DEQUE* deque);
POINT deque_pop_back(DEQUE* deque);
POINT deque_pop_middle(DEQUE* deque);
POINT deque_pop_random(DEQUE* deque);

#endif
