#pragma once
#include "Vector.h"

class Direction
{
public:
    Direction(float x, float y, float z);
    Direction(Vector v);
    Vector direction;
    float length(); // return infty
    Direction rotated(Point origin, Direction direction, float angle_degrees);
};
