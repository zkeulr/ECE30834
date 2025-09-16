#include <fstream>
#include <iostream>
#include "TriangleMesh.h"
#include "PlanarPinholeCamera.h"
#include "Vector.h"
#include "Matrix.h"
#include "Point.h"
#include "Direction.h"

using namespace std;

void TriangleMesh::DrawPoints(unsigned int color, int psize, PlanarPinholeCamera *ppc,
                              FrameBuffer *fb)
{

    for (int vi = 0; vi < vertsN; vi++)
    {
        fb->DrawPoint3D(verts[vi], ppc, psize, color);
    }
}

void TriangleMesh::Translate(Vector tv)
{

    for (int vi = 0; vi < vertsN; vi++)
    {
        verts[vi] = verts[vi] + tv;
    }
}

Vector TriangleMesh::GetCenter()
{

    Vector ret(0.0f, 0.0f, 0.0f);
    for (int vi = 0; vi < vertsN; vi++)
    {
        ret = ret + verts[vi];
    }
    return ret / (float)vertsN;
}

void TriangleMesh::RotateAboutArbitraryAxis(Vector aO, Vector ad, float theta)
{

    for (int vi = 0; vi < vertsN; vi++)
    {
        verts[vi] = verts[vi].rotated(Point(aO), Direction(ad), theta);
    }
}

void TriangleMesh::Position(Vector newCenter)
{

    Vector oldCenter = GetCenter();
    Translate(newCenter - oldCenter);
}

// loading triangle mesh from a binary file, i.e., a .bin file from geometry folder
void TriangleMesh::LoadBin(char *fname)
{

    Vector *normals = 0;

    ifstream ifs(fname, ios::binary);
    if (ifs.fail())
    {
        cerr << "INFO: cannot open file: " << fname << endl;
        return;
    }

    ifs.read((char *)&vertsN, sizeof(int));
    char yn;
    ifs.read(&yn, 1); // always xyz
    if (yn != 'y')
    {
        cerr << "INTERNAL ERROR: there should always be vertex xyz data" << endl;
        return;
    }
    if (verts)
        delete verts;
    verts = new Vector[vertsN];

    ifs.read(&yn, 1); // cols 3 floats
    if (colors)
        delete colors;
    colors = 0;
    if (yn == 'y')
    {
        colors = new Vector[vertsN];
    }

    ifs.read(&yn, 1); // normals 3 floats
    if (normals)
        delete[] normals;
    normals = 0;
    if (yn == 'y')
    {
        normals = new Vector[vertsN];
    }

    ifs.read(&yn, 1); // texture coordinates 2 floats
    float *tcs = 0;   // don't have texture coordinates for now
    if (tcs)
        delete[] tcs;
    tcs = 0;
    if (yn == 'y')
    {
        tcs = new float[vertsN * 2];
    }

    ifs.read((char *)verts, vertsN * 3 * sizeof(float)); // load verts

    if (colors)
    {
        ifs.read((char *)colors, vertsN * 3 * sizeof(float)); // load colors
    }

    if (normals)
        ifs.read((char *)normals, vertsN * 3 * sizeof(float)); // load normals

    if (tcs)
        ifs.read((char *)tcs, vertsN * 2 * sizeof(float)); // load texture coordinates

    ifs.read((char *)&trisN, sizeof(int));
    if (tris)
        delete tris;
    tris = new unsigned int[trisN * 3];
    ifs.read((char *)tris, trisN * 3 * sizeof(unsigned int)); // read tiangles

    ifs.close();

    cerr << "INFO: loaded " << vertsN << " verts, " << trisN << " tris from " << endl
         << "      " << fname << endl;
    cerr << "      xyz " << ((colors) ? "rgb " : "") << ((normals) ? "nxnynz " : "") << ((tcs) ? "tcstct " : "") << endl;
}
