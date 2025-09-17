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

PlanarPinholeCamera PlanarPinholeCamera::Interpolate(PlanarPinholeCamera other_camera, float t)
{

    Vector C_i = C + (other_camera.C - C) * t;
    Vector a_i = a + (other_camera.a - a) * t;
    Vector b_i = b + (other_camera.b - b) * t;
    Vector c_i = c + (other_camera.c - c) * t;

    PlanarPinholeCamera retPlanarPinholeCamera(*this);
    retPlanarPinholeCamera.C = C_i;
    retPlanarPinholeCamera.a = a;
    retPlanarPinholeCamera.b = b;
    retPlanarPinholeCamera.c = c;

    return retPlanarPinholeCamera;
}

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

// visualize (draw) "this" camera (on which you are calling visualize)
//    as seen by visPPC
//	  into fb
//    IMPORTANT: draw the camera to scale such that the focal length be visF

// elements that need to be shown:
// C
// a
// b
// c
// focal length visF

void PlanarPinholeCamera::Visualize(PlanarPinholeCamera *visPPC, FrameBuffer *fb, float visF)
{

    // draw image frame as a 3D rectangle projected with visPPC onto fb
    // (4 segments)
    Vector topLeft = C + c;
    Vector topRight = topLeft + a * (float)w;
    Vector bottomRight = topRight + b * (float)h;
    Vector bottomleft = bottomRight - a * (float)w;
    // render 4 3D segments between these 4 corners BUT SCALED DOWN
    float f = GetF();
    //	SCALING FACTOR = visF / f;

    // draw little c vector, i.e., a segment that starts at C and ends at C+c
    //			top left corner of the camera image frame
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