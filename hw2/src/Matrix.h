#pragma once
#include <ostream>
#include <istream>
#include "Direction.h"
#include "Vector.h"

class Matrix
{
public:
    Vector rows[3];
    Matrix();
    Matrix identity();
    Matrix(Vector row0, Vector row1, Vector row2);
    static Matrix createRotationMatrix(Direction axis, float angle_degrees);
    void setRotationMatrix(Direction axis, float angle_degrees);
    Vector &operator[](int i);
    Vector getColumn(int i);
    Matrix inverted();
    Matrix transposed();
    Vector operator*(Vector v);
    Matrix operator*(Matrix m);
    friend std::ostream &operator<<(std::ostream &ostr, Matrix m);
    friend std::istream &operator>>(std::istream &istr, Matrix m);
};