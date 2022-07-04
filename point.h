#ifndef POINT_H
#define POINT_H

typedef struct POINT
{
    int x;
    int y;
} POINT;

unsigned char points_are_equal(POINT p1, POINT p2);

#endif
