#pragma once
#include <ostream>
#include "Direction.h"
#include "Vector.h"

class Matrix
{
public:
    Vector rows[3];
    Matrix() {};
    Matrix::Matrix(Vector row0, Vector row1, Vector row2);
    // constructor of identity matrix
    Matrix rotated(Direction axis, float alpha_degrees);
    Vector &Matrix::operator[](int i);
    Vector Matrix::getColumn(int i);
    Matrix inverted();
    Matrix transposed();
    Vector Matrix::operator*(Vector v);
    Matrix operator*(Matrix m);
    friend std::ostream &operator<<(std::ostream &ostr, Matrix v);
    friend std::ostream &operator>>(std::ostream &ostr, Matrix v);
    static Matrix createRotationMatrix(Direction axis, float angle_degrees);
};