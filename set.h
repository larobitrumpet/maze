#include "point.h"
#include "random.h"

#ifndef SET_H
#define SET_H

#define SET_INIT_SIZE 16
#define SET_SIZE_INCREASE 1.5

typedef struct SET
{
    POINT* points;
    int length;
    int size;
} SET;

SET* construct_set();
void deconstruct_set(SET* set);
unsigned char in_set(SET* set, POINT item);
unsigned char set_is_empty(SET* set);
static void set_increase_size(SET* set);
void set_add(SET* set, POINT p);
POINT set_pop_random(SET* set);
void set_remove(SET* set, POINT p);

#endif
