#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>
#include "PlanarPinholeCamera.h"
#include "Vector.h"

class FrameBuffer : public Fl_Gl_Window
{
public:
    unsigned int *pix;
    int w, h;
    FrameBuffer(int u0, int v0, int _w, int _h);
    void draw();
    int handle(int guievent);
    void LoadTiff(char *fname);
    void SaveTiff(char *fname);
    void KeyboardHandle();
    void Set(unsigned int color);
    void Set(unsigned int u, unsigned int v, unsigned int color);
    void SetSafe(int u, int v, unsigned int color);
    void SetChecker(int cw, unsigned int col0, unsigned int col1);
    void DrawPoint2D(Vector P, int psize, unsigned int color);
    void DrawPoint3D(Vector P, PlanarPinholeCamera *ppc, int psize, unsigned int color);
    void Draw3DSegment(unsigned int color, PlanarPinholeCamera *ppc, Vector V0, Vector V1);
    void Draw2DSegment(unsigned int color, Vector pV0, Vector pV1);
    void Draw3DSegment(Vector C0, Vector C1, PlanarPinholeCamera *ppc, Vector V0, Vector V1);
    void Draw2DSegment(Vector C0, Vector C1, Vector pV0, Vector pV1);
};