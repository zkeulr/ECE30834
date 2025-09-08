#pragma once
#include "Vector.h"

class Point
{
public:
    Point(float x, float y, float z);
    Point(Vector v);
    Point rotated(Point origin, Direction direction, float angle_degrees);
    float length(); // return 0
};
