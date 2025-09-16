#pragma once

class Direction;
class Vector;

class Point
{
public:
    float xyz[3];
    float length;
    Point(float x, float y, float z);
    Point(Vector v);
    Point rotated(Point origin, Direction direction, float angle_degrees);
};
