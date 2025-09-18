#pragma once

class Point;
class Vector;

class Direction
{
public:
    float xyz[3];
    float length();
    Direction(float x, float y, float z);
    Direction(Vector v);
    Direction rotated(Point origin, Direction direction, float angle_degrees);
};
