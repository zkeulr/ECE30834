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
    switch (event)
    {
    case FL_KEYBOARD:
    {
        KeyboardHandle();
        return 0;
    }
    case FL_MOVE:
    {
        int u = Fl::event_x();
        int v = Fl::event_y();
        if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
            return 0;
        cerr << u << " " << v << "         \r";
        return 0;
    }
    default:
        return 0;
    }
    return 0;
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
