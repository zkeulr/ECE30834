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

    /**
     * @brief Unary negation operator
     * @return Vector with negated components
     */
    Vector operator-() const;

    /**
     * @brief Scalar multiplication operator (scalar * vector)
     * @param scalar The scalar value to multiply by
     * @param v The vector to multiply
     * @return Vector with each component multiplied by scalar
     */
    friend Vector operator*(float scalar, const Vector &v);
    Vector operator^(Vector v);
    Vector operator/(float scalar);
    Vector operator*(float scalar);
    Vector operator+(Vector v);
    Vector operator-(Vector v);
    Vector rotated(Point origin, Direction direction, float angle_degrees);
};