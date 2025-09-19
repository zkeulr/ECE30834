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

void TriangleMesh::DrawWireFrame(unsigned int color, int psize, PlanarPinholeCamera *ppc, FrameBuffer *fb)
{

    for (int tri = 0; tri < trisN; tri++)
    {
        Vector Vs[3], Cs[3];
        Vs[0] = verts[tris[tri * 3 + 0]];
        Vs[1] = verts[tris[tri * 3 + 1]];
        Vs[2] = verts[tris[tri * 3 + 2]];
        if (colors)
        {
            Cs[0] = colors[tris[tri * 3 + 0]];
            Cs[1] = colors[tris[tri * 3 + 1]];
            Cs[2] = colors[tris[tri * 3 + 2]];
        }
        for (int ei = 0; ei < 3; ei++)
        {
            if (colors)
                fb->Draw3DSegment(Cs[ei], Cs[(ei + 1) % 3], ppc,
                                  Vs[ei], Vs[(ei + 1) % 3]);
            else
                fb->Draw3DSegment(color, ppc, Vs[ei], Vs[(ei + 1) % 3]);
        }
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

void TriangleMesh::RotateAboutArbitraryAxis(Point origin, Direction direction, float theta)
{

    for (int vi = 0; vi < vertsN; vi++)
    {
        verts[vi] = verts[vi].rotated(origin, direction, theta);
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
    ifstream ifs(fname, ios::binary);
    if (ifs.fail())
    {
        cerr << "INFO: cannot open file: " << fname << endl;
        return;
    }

    // Clean up existing data
    if (verts)
    {
        delete[] verts;
        verts = nullptr;
    }
    if (colors)
    {
        delete[] colors;
        colors = nullptr;
    }
    if (normals)
    {
        delete[] normals;
        normals = nullptr;
    }
    if (tris)
    {
        delete[] tris;
        tris = nullptr;
    }

    ifs.read((char *)&vertsN, sizeof(int));
    char yn;
    ifs.read(&yn, 1); // always xyz
    if (yn != 'y')
    {
        cerr << "INTERNAL ERROR: there should always be vertex xyz data" << endl;
        return;
    }

    verts = new Vector[vertsN];

    ifs.read(&yn, 1); // cols 3 floats
    if (yn == 'y')
    {
        colors = new Vector[vertsN];
    }

    ifs.read(&yn, 1); // normals 3 floats
    if (yn == 'y')
    {
        normals = new Vector[vertsN];
    }

    ifs.read(&yn, 1); // texture coordinates 2 floats
    float *tcs = nullptr;
    if (yn == 'y')
    {
        tcs = new float[vertsN * 2];
    }

    float *tempVerts = new float[vertsN * 3];

    ifs.read((char *)tempVerts, vertsN * 3 * sizeof(float));

    // properly construct Vector objects
    for (int i = 0; i < vertsN; i++)
    {
        verts[i] = Vector(tempVerts[i * 3], tempVerts[i * 3 + 1], tempVerts[i * 3 + 2]);
    }
    delete[] tempVerts;

    if (colors)
    {
        ifs.read((char *)colors, vertsN * 3 * sizeof(float)); // load colors
    }

    if (normals)
        ifs.read((char *)normals, vertsN * 3 * sizeof(float)); // load normals

    if (tcs)
    {
        ifs.read((char *)tcs, vertsN * 2 * sizeof(float)); // load texture coordinates
        delete[] tcs;                                      // Clean up local tcs since it's not stored as member
    }

    ifs.read((char *)&trisN, sizeof(int));
    tris = new unsigned int[trisN * 3];
    ifs.read((char *)tris, trisN * 3 * sizeof(unsigned int)); // read triangles

    ifs.close();

    cerr << "INFO: loaded " << vertsN << " verts, " << trisN << " tris from " << endl
         << "      " << fname << endl;
    cerr << "      xyz " << ((colors) ? "rgb " : "") << ((normals) ? "nxnynz " : "") << ((tcs) ? "tcstct " : "") << endl;
}

TriangleMesh::~TriangleMesh()
{
    delete[] verts;
    delete[] colors;
    delete[] tris;
    delete[] normals;
}

void TriangleMesh::ComputeAABB(Vector &minP, Vector &maxP)
{
    if (vertsN <= 0 || verts == nullptr)
    {
        minP = Vector(0, 0, 0);
        maxP = Vector(0, 0, 0);
        return;
    }

    minP = verts[0];
    maxP = verts[0];

    for (int i = 1; i < vertsN; i++)
    {
        for (int k = 0; k < 3; k++)
        {
            if (verts[i][k] < minP[k])
                minP[k] = verts[i][k];
            if (verts[i][k] > maxP[k])
                maxP[k] = verts[i][k];
        }
    }
}

void TriangleMesh::DrawAABB(FrameBuffer *fb, PlanarPinholeCamera *ppc,
                            Vector minP, Vector maxP,
                            unsigned int color)
{
    // 8 corners of the box
    Vector p000(minP[0], minP[1], minP[2]);
    Vector p001(minP[0], minP[1], maxP[2]);
    Vector p010(minP[0], maxP[1], minP[2]);
    Vector p011(minP[0], maxP[1], maxP[2]);
    Vector p100(maxP[0], minP[1], minP[2]);
    Vector p101(maxP[0], minP[1], maxP[2]);
    Vector p110(maxP[0], maxP[1], minP[2]);
    Vector p111(maxP[0], maxP[1], maxP[2]);

    // bottom face
    fb->Draw3DSegment(color, ppc, p000, p100);
    fb->Draw3DSegment(color, ppc, p100, p110);
    fb->Draw3DSegment(color, ppc, p110, p010);
    fb->Draw3DSegment(color, ppc, p010, p000);

    // top face
    fb->Draw3DSegment(color, ppc, p001, p101);
    fb->Draw3DSegment(color, ppc, p101, p111);
    fb->Draw3DSegment(color, ppc, p111, p011);
    fb->Draw3DSegment(color, ppc, p011, p001);

    // vertical edges
    fb->Draw3DSegment(color, ppc, p000, p001);
    fb->Draw3DSegment(color, ppc, p100, p101);
    fb->Draw3DSegment(color, ppc, p110, p111);
    fb->Draw3DSegment(color, ppc, p010, p011);
}

void TriangleMesh::Scale(float s)
{
    Vector center = GetCenter();

    for (int vi = 0; vi < vertsN; vi++)
    {
        verts[vi] = center + s * (verts[vi] - center);
    }
}