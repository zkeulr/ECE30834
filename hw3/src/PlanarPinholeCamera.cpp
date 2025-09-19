#include <cmath>
#include <fstream>
#include "PlanarPinholeCamera.h"
#include "framebuffer.h"
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

PlanarPinholeCamera::PlanarPinholeCamera(const PlanarPinholeCamera &other)
{
    w = other.w;
    h = other.h;
    C = other.C;
    a = other.a;
    b = other.b;
    c = other.c;
}

PlanarPinholeCamera &PlanarPinholeCamera::operator=(const PlanarPinholeCamera &other)
{
    if (this != &other)
    {
        w = other.w;
        h = other.h;
        C = other.C;
        a = other.a;
        b = other.b;
        c = other.c;
    }
    return *this;
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
    Direction up(-b);
    Matrix R = Matrix::createRotationMatrix(up, theta);

    a = R * a;
    b = R * b;
    c = R * c;
}

void PlanarPinholeCamera::Tilt(float theta)
{
    Direction right(a);
    Matrix R = Matrix::createRotationMatrix(right, theta);

    a = R * a;
    b = R * b;
    c = R * c;
}

void PlanarPinholeCamera::Roll(float theta)
{
    Vector vd = a ^ b;
    Direction view(vd.normalized());

    Matrix R = Matrix::createRotationMatrix(view, theta);

    a = R * a;
    b = R * b;
    c = R * c;
}

void PlanarPinholeCamera::Zoom(float zoom_scalar)
{
    c[2] = c[2] * zoom_scalar;
}

// nonlinear interpolation
PlanarPinholeCamera PlanarPinholeCamera::Interpolate(PlanarPinholeCamera other_camera, float t)
{
    float s = t * t * (3 - 2 * t); // smoother
    // float s = sin(t * M_PI_2);

    Vector C_i = C + (other_camera.C - C) * s;
    Vector a_i = a + (other_camera.a - a) * s;
    Vector b_i = b + (other_camera.b - b) * s;
    Vector c_i = c + (other_camera.c - c) * s;

    PlanarPinholeCamera retPlanarPinholeCamera(*this);
    retPlanarPinholeCamera.C = C_i;
    retPlanarPinholeCamera.a = a_i;
    retPlanarPinholeCamera.b = b_i;
    retPlanarPinholeCamera.c = c_i;

    return retPlanarPinholeCamera;
}

// linear interpolation
/*
PlanarPinholeCamera PlanarPinholeCamera::Interpolate(PlanarPinholeCamera other_camera, float t)
{

    Vector C_i = C + (other_camera.C - C) * t;
    Vector a_i = a + (other_camera.a - a) * t;
    Vector b_i = b + (other_camera.b - b) * t;
    Vector c_i = c + (other_camera.c - c) * t;

    PlanarPinholeCamera retPlanarPinholeCamera(*this);
    retPlanarPinholeCamera.C = C_i;
    retPlanarPinholeCamera.a = a_i;
    retPlanarPinholeCamera.b = b_i;
    retPlanarPinholeCamera.c = c_i;

    return retPlanarPinholeCamera;
}
*/

void PlanarPinholeCamera::SaveTextFile(const char *filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        return;
    }

    file << w << " " << h << std::endl;
    file << C << std::endl;
    file << a << std::endl;
    file << b << std::endl;
    file << c << std::endl;

    file.close();
}

void PlanarPinholeCamera::LoadTextFile(const char *filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        return;
    }

    file >> w >> h;
    file >> C;
    file >> a;
    file >> b;
    file >> c;

    file.close();
    return;
}

void PlanarPinholeCamera::Visualize(PlanarPinholeCamera *visPPC, FrameBuffer *fb, float visF)
{
    float f = GetF();
    float scale = visF / f;

    // camera center
    fb->DrawPoint3D(C, visPPC, 5, 0xFF00FF00);

    Vector scaled_a = a * (float)w * scale;
    Vector scaled_b = b * (float)h * scale;
    Vector scaled_c = c * scale;

    // image plane corners
    Vector topLeft = C + scaled_c;
    Vector topRight = topLeft + scaled_a;
    Vector bottomRight = topRight + scaled_b;
    Vector bottomLeft = bottomRight - scaled_a;

    // draw image plane
    fb->Draw3DSegment(0xFFFFFFFF, visPPC, topLeft, topRight);
    fb->Draw3DSegment(0xFFFFFFFF, visPPC, topRight, bottomRight);
    fb->Draw3DSegment(0xFFFFFFFF, visPPC, bottomRight, bottomLeft);
    fb->Draw3DSegment(0xFFFFFFFF, visPPC, bottomLeft, topLeft);

    // center of plane
    Vector center_of_plane = (topLeft + topRight + bottomRight + bottomLeft) * 0.25f;
    fb->Draw3DSegment(0xFFFFFF00, visPPC, C, center_of_plane);

    // axis indicators
    fb->Draw3DSegment(0xFF0000FF, visPPC, C, C + (c.normalized() * visF));        // view dir
    fb->Draw3DSegment(0xFFFF0000, visPPC, C, C + (a.normalized() * visF * 0.3f)); // right
    fb->Draw3DSegment(0xFF00FFFF, visPPC, C, C + (b.normalized() * visF * 0.3f)); // up
}

float PlanarPinholeCamera::GetF()
{
    Vector vd = GetVD();
    float ret = vd * c;
    return ret;
}

Vector PlanarPinholeCamera::GetVD()
{
    return (a ^ b).normalized();
}