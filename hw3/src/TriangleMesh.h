#pragma once
#include "Vector.h"
#include "framebuffer.h"
#include "PlanarPinholeCamera.h"

class TriangleMesh
{
public:
    Vector *verts;
    int vertsN;
    Vector *colors;     // vertex colors in Vector format (one float in [0.0f, 1.0f] per R, G, and B channel)
    unsigned int *tris; // triples of vertex indices
    int trisN;

    TriangleMesh() {};
    void LoadBin(char *fname); // load from file
    void DrawPoints(unsigned int color, int psize, PlanarPinholeCamera *ppc, FrameBuffer *fb);
    void RotateAboutArbitraryAxis(Vector aO, Vector ad, float theta);
    Vector GetCenter(); // return the average of all vertices
    void Position(Vector newCenter);
    void Translate(Vector tv);
};