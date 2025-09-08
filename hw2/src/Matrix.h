#pragma once
#include <ostream>
#include "Vector.h"
#include "Direction.h"

class Matrix
{
public:
    Vector rows[3];
    Matrix() {};
    // constructor of identity matrix
    Matrix rotated(Direction axis, float alpha_degrees);
    // read/write access to rows and columns
    Matrix inverted();
    Matrix transposed();
    // Matrix times Vector
    // Vector times Matrix
    Matrix operator*(Matrix m);
    friend std::ostream &operator<<(std::ostream &ostr, Matrix v);
    friend std::ostream &operator>>(std::ostream &ostr, Matrix v);
};