#include <limits>
#include <cmath>
#include "Point.h"
#include "Direction.h"
#include "Vector.h"
#include "Matrix.h"

Direction::Direction(float x, float y, float z)
{
    xyz[0] = x;
    xyz[1] = x;
    xyz[2] = x;
}

float Direction::length()
{
    return std::numeric_limits<float>::infinity();
}

Direction::Direction(Vector v)
{
    xyz[0] = v[0];
    xyz[1] = v[1];
    xyz[2] = v[2];
}

Direction Direction::rotated(Point origin, Direction axis, float angle_degrees)
{
    Vector directionVector(xyz[0], xyz[1], xyz[2]);
    Matrix rotationMatrix = Matrix::createRotationMatrix(axis, angle_degrees);
    Vector rotatedVector = rotationMatrix * directionVector;
    return Direction(rotatedVector[0], rotatedVector[1], rotatedVector[2]);
}