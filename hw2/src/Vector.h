#pragma once
#include <ostream>
#include <istream>

class Direction;
class Point;

class Vector
{
public:
    float xyz[3];
    Vector() {};
    Vector(float x, float y, float z);
    float &operator[](int i);
    friend std::ostream &operator<<(std::ostream &ostr, Vector v);
    friend std::istream &operator>>(std::ostream &ostr, Vector v);
    Vector normalized();
    float length;
    float operator*(Vector v);
    Vector operator^(Vector v);
    Vector operator/(float scalar);
    Vector operator*(float scalar);
    Vector operator+(Vector v);
    Vector operator-(Vector v);
};