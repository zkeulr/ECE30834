#include "Matrix.h"
#include <cmath>

Matrix::Matrix()
{
    rows[0] = Vector(0, 0, 0);
    rows[1] = Vector(0, 0, 0);
    rows[2] = Vector(0, 0, 0);
}

Matrix::Matrix(Vector row0, Vector row1, Vector row2)
{
    rows[0] = row0;
    rows[1] = row1;
    rows[2] = row2;
}

Matrix Matrix::identity()
{
    rows[0] = Vector(1, 0, 0);
    rows[1] = Vector(0, 1, 0);
    rows[2] = Vector(0, 0, 1);
}

Vector &Matrix::operator[](int i)
{
    return rows[i];
}

Vector Matrix::getColumn(int i)
{
    Vector ret;
    Matrix &m = *this;
    ret[0] = m[0][i];
    ret[1] = m[1][i];
    ret[2] = m[2][i];
    return ret;
}

Vector Matrix::operator*(Vector v)
{
    Vector ret;
    Matrix &m = *this;
    ret[0] = m[0] * v;
    ret[1] = m[1] * v;
    ret[2] = m[2] * v;
    return ret;
}

Matrix Matrix::operator*(Matrix m)
{
    Matrix ret;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            ret[i][j] = rows[i][0] * m.rows[0][j] +
                        rows[i][1] * m.rows[1][j] +
                        rows[i][2] * m.rows[2][j];
        }
    }

    return ret;
}

Matrix Matrix::inverted()
{
    Matrix ret;
    Vector a = getColumn(0), b = getColumn(1), c = getColumn(2);
    Vector _a = b ^ c;
    _a = _a / (a * _a);
    Vector _b = c ^ a;
    _b = _b / (b * _b);
    Vector _c = a ^ b;
    _c = _c / (c * _c);
    ret[0] = _a;
    ret[1] = _b;
    ret[2] = _c;

    return ret;
}

Matrix Matrix::transposed()
{
    Matrix ret;

    ret.rows[0] = getColumn(0);
    ret.rows[1] = getColumn(1);
    ret.rows[2] = getColumn(2);

    return ret;
}

Matrix Matrix::createRotationMatrix(Direction axis, float angle_degrees)
{
    float angle_radians = angle_degrees * M_PI / 180.0f;

    Vector axisVector(axis.xyz[0], axis.xyz[1], axis.xyz[2]);
    axisVector = axisVector.normalized();

    float x = axisVector[0];
    float y = axisVector[1];
    float z = axisVector[2];

    float c = std::cos(angle_radians);
    float s = std::sin(angle_radians);
    float omc = 1.0f - c;

    Vector row0(c + x * x * omc, x * y * omc - z * s, x * z * omc + y * s);
    Vector row1(y * x * omc + z * s, c + y * y * omc, y * z * omc - x * s);
    Vector row2(z * x * omc - y * s, z * y * omc + x * s, c + z * z * omc);

    return Matrix(row0, row1, row2);
}

void Matrix::setRotationMatrix(Direction axis, float angle_degrees)
{
    *this = createRotationMatrix(axis, angle_degrees);
}

std::ostream &operator<<(std::ostream &ostr, const Matrix &m)
{
    for (int i = 0; i < 3; i++)
    {
        ostr << "[ ";
        for (int j = 0; j < 3; j++)
        {
            ostr << m.rows[i][j];
            if (j < 2)
                ostr << ", ";
        }
        ostr << " ]";
        if (i < 2)
            ostr << std::endl;
    }
    return ostr;
}

std::istream &operator>>(std::istream &istr, Matrix &m)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            istr >> m.rows[i][j];
        }
    }
    return istr;
}