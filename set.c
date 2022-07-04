#include "set.h"

SET* construct_set()
{
    SET* set = (SET*)malloc(sizeof(SET));
    POINT* points = (POINT*)malloc(sizeof(POINT) * SET_INIT_SIZE);
    set->points = points;
    set->length = 0;
    set->size = SET_INIT_SIZE;
}

void deconstruct_set(SET* set)
{
    free(set->points);
    free(set);
}

unsigned char in_set(SET* set, POINT item)
{
    for (int i = 0; i < set->length; i++)
    {
        if (set->points[i] == item)
            return 1;
    }
    return 0;
}

unsigned char set_is_empty(SET* set)
{
    return set->length == 0;
}

static void set_increase_size(SET* set)
{
    set->size = (int)(set->size * SET_SIZE_INCREASE);
    set->points = (POINT*)realloc(set->points, sizeof(POINT) * set->size);
}

void set_add(SET* set, POINT p)
{
    if (!(in_set(set, p)))
    {
        if (set->length == set->size)
            set_increase_size(set);
        set->points[set->length] = p;
        set->length++;
    }
}

POINT set_pop_random(SET* set)
{
    if (set_is_empty(set))
    {
        POINT p;
        p.x = -1;
        p.y = -1;
        return p;
    }
    shuffle(set->points, set->length, sizeof(POINT));
    set->length--;
    return set->points[set->length];
}
