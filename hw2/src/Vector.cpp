#include <ostream>
#include <math.h>
#include "Vector.h"

Vector::Vector(float x, float y, float z)
{
    xyz[0] = x;
    xyz[1] = y;
    xyz[2] = z;

    length = sqrtf(x * x + y * y + z * z);
}

Vector Vector::operator+(Vector v)
{

    Vector ret;
    Vector &u = (*this);
    for (int i = 0; i < 3; i++)
    {
        ret[i] = u[i] + v[i];
    }

    return ret;
}

Vector Vector::operator-(Vector v)
{

    Vector ret;
    Vector &u = (*this);
    for (int i = 0; i < 3; i++)
    {
        ret[i] = u[i] - v[i];
    }

    return ret;
}

std::ostream &operator<<(std::ostream &ostr, Vector v)
{

    return ostr << v[0] << " " << v[1] << " " << v[2];
}

float &Vector::operator[](int i)
{

    return xyz[i];
}

float Vector::operator*(Vector v)
{

    Vector &u = *this;

    float ret = u[0] * v[0] + u[1] * v[1] + u[2] * v[2];

    return ret;
}

Vector Vector::operator/(float scalar)
{
    Vector ret;
    Vector &v = *this;
    return Vector(v[0] / scalar, v[1] / scalar, v[2] / scalar);
}

Vector Vector::operator*(float scalar)
{
    Vector ret;
    Vector &v = *this;
    ret[0] = v[0] * scalar;
    ret[1] = v[1] * scalar;
    ret[2] = v[2] * scalar;
    return ret;
}

Vector Vector::operator^(Vector v)
{
    Vector &u = *this;
    return Vector(u[1] * v[2] - u[2] * v[1],
                  u[2] * v[0] - u[0] * v[2],
                  u[0] * v[1] - u[1] * v[0]);
}

Vector Vector::normalized()
{
    Vector &v = *this;
    return v / v.length;
}

/*
Vector rotated(Point origin, Direction direction, float angle_degrees)
{

}
*/