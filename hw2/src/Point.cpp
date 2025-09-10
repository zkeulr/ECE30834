#include "Point.h"
#include "Direction.h"
#include "Vector.h"
#include "Matrix.h"

Point::Point(float x, float y, float z)
{
    xyz[0] = x;
    xyz[1] = y;
    xyz[2] = z;

    length = 0;
}

Point::Point(Vector v)
{
    xyz[0] = v[0];
    xyz[1] = v[1];
    xyz[2] = v[2];

    length = 0;
}

Point Point::rotated(Point origin, Direction direction, float angle_degrees)
{
    Vector translated(xyz[0] - origin.xyz[0],
                      xyz[1] - origin.xyz[1],
                      xyz[2] - origin.xyz[2]);

    Matrix rotationMatrix = Matrix::createRotationMatrix(direction, angle_degrees);
    Vector rotatedVector = rotationMatrix * translated;

    Point rotatedPoint(rotatedVector[0] + origin.xyz[0],
                       rotatedVector[1] + origin.xyz[1],
                       rotatedVector[2] + origin.xyz[2]);

    return rotatedPoint;
}