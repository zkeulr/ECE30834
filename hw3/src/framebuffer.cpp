#include <GL/glew.h>
#include <tiffio.h>
#include <iostream>
#include <fstream>
#include <strstream>
#include "framebuffer.h"
#include "scene.h"
#include "Vector.h"

using namespace std;

FrameBuffer::FrameBuffer(int u0, int v0, int _w, int _h) : Fl_Gl_Window(u0, v0, _w, _h, 0)
{
    w = _w;
    h = _h;
    pix = new unsigned int[w * h];
}

void FrameBuffer::draw()
{
    glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);
}

int FrameBuffer::handle(int event)
{
    if (event == FL_KEYDOWN)
    {
        int key = Fl::event_key();
        std::cerr << "Key pressed: " << key << std::endl;

        if (!scene)
            return 0;

        switch (key)
        {
        case 'w':
        case 'W':
            scene->MoveForward();
            break;
        case 's':
        case 'S':
            scene->MoveBackward();
            break;
        case 'a':
        case 'A':
            scene->MoveLeft();
            break;
        case 'd':
        case 'D':
            scene->MoveRight();
            break;
        case FL_Left:
            scene->Pan(-1);
            break;
        case FL_Right:
            scene->Pan(1);
            break;
        case FL_Up:
            scene->Tilt(-1);
            break;
        case FL_Down:
            scene->Tilt(1);
            break;
        default:
            return 0; // let FLTK propagate
        }

        redraw();
        return 1;
    }

    return Fl_Gl_Window::handle(event);
}

void FrameBuffer::KeyboardHandle()
{

    int key = Fl::event_key();
    switch (key)
    {
    case FL_Left:
    {
        cerr << "INFO: pressed left arrow key";
        break;
    }
    default:
        cerr << "INFO: do not understand keypress" << endl;
        return;
    }
}

// load a tiff image to pixel buffer
void FrameBuffer::LoadTiff(char *fname)
{
    TIFF *in = TIFFOpen(fname, "r");
    if (in == NULL)
    {
        cerr << fname << " could not be opened" << endl;
        return;
    }

    int width, height;
    TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
    if (w != width || h != height)
    {
        w = width;
        h = height;
        delete[] pix;
        pix = new unsigned int[w * h];
        size(w, h);
        glFlush();
        glFlush();
    }

    if (TIFFReadRGBAImage(in, w, h, pix, 0) == 0)
    {
        cerr << "failed to load " << fname << endl;
    }

    TIFFClose(in);
}

// save as tiff image
void FrameBuffer::SaveTiff(char *fname)
{

    TIFF *out = TIFFOpen(fname, "w");

    if (out == NULL)
    {
        cerr << fname << " could not be opened" << endl;
        return;
    }

    TIFFSetField(out, TIFFTAG_IMAGEWIDTH, w);
    TIFFSetField(out, TIFFTAG_IMAGELENGTH, h);
    TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 4);
    TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

    for (uint32 row = 0; row < (unsigned int)h; row++)
    {
        TIFFWriteScanline(out, &pix[(h - row - 1) * w], row);
    }

    TIFFClose(out);
}

void FrameBuffer::Set(unsigned int color)
{

    for (int uv = 0; uv < w * h; uv++)
        pix[uv] = color;
}

void FrameBuffer::Set(unsigned int u, unsigned int v, unsigned int color)
{
    if (u >= w || v >= h)
    {
        return;
    }
    pix[(h - 1 - v) * w + u] = color;
}

void FrameBuffer::SetChecker(int cw, unsigned int col0,
                             unsigned int col1)
{

    for (int v = 0; v < h; v++)
    {
        for (int u = 0; u < w; u++)
        {
            int cu, cv;
            cu = u / cw;
            cv = v / cw;
            if ((cu + cv) % 2)
                Set(u, v, col0);
            else
                Set(u, v, col1);
        }
    }
}

void FrameBuffer::SetSafe(int u, int v, unsigned int color)
{
    if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
        return;

    Set(u, v, color);
}

void FrameBuffer::DrawPoint2D(Vector P, int psize, unsigned int color)
{

    int up = (int)P[0];
    int vp = (int)P[1];

    for (int v = vp - psize / 2; v < vp + psize / 2; v++)
    {
        for (int u = up - psize / 2; u < up + psize / 2; u++)
        {
            SetSafe(u, v, color);
        }
    }
}

void FrameBuffer::DrawPoint3D(Vector P, PlanarPinholeCamera *ppc, int psize, unsigned int color)
{
    Vector Pp;
    if (!ppc->Project(P, Pp))
        return;

    DrawPoint2D(Pp, psize, color);
}

void FrameBuffer::Draw3DSegment(unsigned int color, PlanarPinholeCamera *ppc, Vector V0, Vector V1)
{
    Vector pV0, pV1;
    if (!ppc->Project(V0, pV0))
        return;
    if (!ppc->Project(V1, pV1))
        return;
    Draw2DSegment(color, pV0, pV1);
}

void FrameBuffer::Draw3DSegment(Vector C0, Vector C1, PlanarPinholeCamera *ppc,
                                Vector V0, Vector V1)
{

    Vector pV0, pV1;
    if (!ppc->Project(V0, pV0))
        return;
    if (!ppc->Project(V1, pV1))
        return;
    Draw2DSegment(C0, C1, pV0, pV1);
}

void FrameBuffer::Draw2DSegment(unsigned int color, Vector pV0, Vector pV1)
{

    pV0[2] = 0.0f;
    pV1[2] = 0.0f;
    int pixn = (int)((pV1 - pV0).length + 2);
    for (int si = 0; si < pixn; si++)
    {
        Vector currP = pV0 + (pV1 - pV0) * (float)si / (float)(pixn - 1);
        SetSafe((int)currP[0], (int)currP[1], color);
    }
}

void FrameBuffer::Draw2DSegment(Vector C0, Vector C1, Vector pV0, Vector pV1)
{

    pV0[2] = 0.0f;
    pV1[2] = 0.0f;
    int pixn = (int)((pV1 - pV0).length + 2);
    for (int si = 0; si < pixn; si++)
    {
        Vector currP = pV0 + (pV1 - pV0) * (float)si / (float)(pixn - 1);
        Vector currC = C0 + (C1 - C0) * (float)si / (float)(pixn - 1);
        unsigned int color = currC.GetColor();
        SetSafe((int)currP[0], (int)currP[1], color);
    }
}
