#include "point.h"

unsigned char points_are_equal(POINT p1, POINT p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}
