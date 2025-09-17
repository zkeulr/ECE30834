#include <cmath>
#include "PlanarPinholeCamera.h"
#include "Matrix.h"

PlanarPinholeCamera::PlanarPinholeCamera(float hfov, int _w, int _h)
{
    w = _w;
    h = _h;
    C = Vector(0.0f, 0.0f, 0.0f);
    a = Vector(1.0f, 0.0f, 0.0f);
    b = Vector(0.0f, -1.0f, 0.0f);
    float hfovr = hfov * 3.1415926f / 180.0f;
    c = Vector(-(float)w / 2.0f, (float)h / 2.0f,
               -(float)w / (2.0f * tan(hfovr / 2.0f)));
}

int PlanarPinholeCamera::Project(Vector P, Vector &PP)
{
    int ret = 1;

    Matrix M;
    M.setColumn(0, a);
    M.setColumn(1, b);
    M.setColumn(2, c);

    Vector q = M.inverted() * (P - C);

    if (q[2] <= 0.0f)
        return 0;

    PP[0] = q[0] / q[2];
    PP[1] = q[1] / q[2];
    PP[2] = 1.0f / q[2];

    return ret;
}

void PlanarPinholeCamera::Translate(Vector tv)
{
    C = C + tv;
}

void PlanarPinholeCamera::Pan(float theta)
{
    a = a.rotated(Point(C), Direction(b), theta);
    b = b.rotated(Point(C), Direction(b), theta);
    c = c.rotated(Point(C), Direction(b), theta);
}

void PlanarPinholeCamera::Tilt(float theta)
{
    a = a.rotated(Point(C), Direction(a), theta);
    b = b.rotated(Point(C), Direction(a), theta);
    c = c.rotated(Point(C), Direction(a), theta);
}

void PlanarPinholeCamera::Roll(float theta)
{
    Vector n = b ^ a;

    a = a.rotated(Point(C), Direction(n), theta);
    b = b.rotated(Point(C), Direction(n), theta);
    c = c.rotated(Point(C), Direction(n), theta);
}

void PlanarPinholeCamera::Zoom(float zoom_scalar)
{
    c[2] = c[2] * zoom_scalar;
}

PlanarPinholeCamera Interpolate(const PlanarPinholeCamera &other_camera, float t)
{
}