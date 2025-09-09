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

Point rotated(Point origin, Direction direction, float angle_degrees)
{
}