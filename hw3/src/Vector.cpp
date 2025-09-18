#include <ostream>
#include <math.h>
#include "Vector.h"
#include "Direction.h"
#include "Point.h"
#include "Matrix.h"

Vector::Vector(float x, float y, float z)
{
    xyz[0] = x;
    xyz[1] = y;
    xyz[2] = z;
}

float Vector::length()
{
    return sqrtf(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]);
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

std::istream &operator>>(std::istream &istr, Vector &v)
{
    return istr >> v[0] >> v[1] >> v[2];
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

Vector Vector::operator-() const
{
    return Vector(-xyz[0], -xyz[1], -xyz[2]);
}

Vector operator*(float scalar, const Vector &v)
{
    return Vector(v.xyz[0] * scalar, v.xyz[1] * scalar, v.xyz[2] * scalar);
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
    return v / v.length();
}

Vector Vector::rotated(Point origin, Direction direction, float angle_degrees)
{
    Vector translated(xyz[0] - origin.xyz[0],
                      xyz[1] - origin.xyz[1],
                      xyz[2] - origin.xyz[2]);

    Matrix rotationMatrix = Matrix::createRotationMatrix(direction, angle_degrees);
    Vector rotatedVector = rotationMatrix * translated;

    return Vector(rotatedVector[0] + origin.xyz[0],
                  rotatedVector[1] + origin.xyz[1],
                  rotatedVector[2] + origin.xyz[2]);
}

unsigned int Vector::GetColor()
{

    // x is in [0 1] (red)
    // y is in [0 1] (green)
    // z is in [0 1] (blue)

    Vector &v = *this;
    unsigned char rgb[3];
    for (int ci = 0; ci < 3; ci++)
    {
        float tmp = v[ci];
        tmp = (tmp < 0.0f) ? 0.0f : tmp;
        tmp = (tmp > 1.0f) ? 1.0f : tmp;
        rgb[ci] = (unsigned char)(tmp * 255.0f + 0.5f);
    }

    unsigned int ret = 0xFF000000 + rgb[2] * 256 * 256 +
                       rgb[1] * 256 + rgb[0];
    return ret;
}