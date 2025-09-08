#pragma once
#include <ostream>
#include "Direction.h"

class Vector
{
public:
    float xyz[3];
    float length;
    Vector(float x, float y, float z);
    Vector() {};
    float &operator[](int i);
    friend std::ostream &operator<<(std::ostream &ostr, Vector v);
    friend std::ostream &operator>>(std::ostream &ostr, Vector v);
    Vector normalized();
    float length();
    float operator*(Vector v);
    float operator^(Vector v1);
    Vector operator/(float scalar);
    Vector operator*(float scalar);
    Vector operator+(Vector v);
    Vector operator-(Vector v);
    Vector rotated(Vector origin, Direction direction, float angle_degrees);
};