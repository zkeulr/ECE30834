#pragma once

class Direction;
class Vector;

class Point
{
public:
    float xyz[3];
    Point(float x, float y, float z);
    float length();
    Point(Vector v);
    Point rotated(Point origin, Direction direction, float angle_degrees);
};
