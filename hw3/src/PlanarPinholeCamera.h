#include "Vector.h"

#pragma once

class PlanarPinholeCamera
{
public:
    Vector a, b, c, C;
    int w, h;
    PlanarPinholeCamera() {};
    PlanarPinholeCamera(float hfov, int _w, int _h);
    int Project(Vector P, Vector &PP);
    void Translate(Vector tv);
};