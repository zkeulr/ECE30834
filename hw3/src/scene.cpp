#include "GL/glew.h"

#include "scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "PlanarPinholeCamera.h"
#include "TriangleMesh.h"
#include "gui.h"

Scene *scene;

#include <iostream>
#include <fstream>
#include <strstream>

using namespace std;

Scene::Scene()
{

    int u0 = 20;
    int v0 = 40;
    int h = 400;
    int w = 600;
    fb = new FrameBuffer(u0, v0, w, h);
    fb->position(u0, v0);
    fb->label("SW Framebuffer");
    fb->show();
    fb->redraw();

    gui = new GUI();
    gui->show();
    gui->uiw->position(u0, v0 + fb->h + v0);
}

void Scene::DBG()
{

    cerr << endl;

    {
        // load triangle mesh
        TriangleMesh TriangleMesh;
        TriangleMesh.LoadBin("geometry/teapot1K.bin");

        TriangleMesh.Position(Vector(0.0f, 0.0f, -150.0f));

        // create camera
        float hfov = 60.0f;
        PlanarPinholeCamera ppc(hfov, fb->w, fb->h);
        // translate backwards to teapot
        //		PlanarPinholeCamera.Translate(Vector(0.0f, 0.0f, 250.0f));
        // clear framebuffer
        fb->Set(0xFFFFFFFF);

        // render mesh with one point per vertex; // no triangles (yet)
        TriangleMesh.DrawPoints(0xFF000000, 3, &ppc, fb);

        // refresh window
        fb->redraw();

        //		return;

        // spin the teapot in place (about a vertical axis passing through its center)
        Vector ad(0.0f, 1.0f, 0.0f);
        Vector aO = TriangleMesh.GetCenter();
        for (int fi = 0; fi < 360; fi++)
        {
            fb->Set(0xFFFFFFFF);
            TriangleMesh.DrawPoints(0xFF000000, 3, &ppc, fb);
            fb->redraw();
            Fl::check();
            TriangleMesh.RotateAboutArbitraryAxis(aO, ad, 1.0f);

            // ppc.Pan(1);
            // ppc.Tilt(1);
            // ppc.Roll(1);
            ppc.Zoom(1.005);
        }

        return;
    }

    {
        float hfov = 60.0f;
        PlanarPinholeCamera PlanarPinholeCamera(hfov, fb->w, fb->h);
        Vector PL(-5.0f, -5.0f, -20.0f);
        Vector PR(+5.0f, -5.0f, -20.0f);
        Vector PP;

        Vector cube[8];
        Vector cubeCenter(0.0f, 0.0f, -10.0f);
        float a = 2.0f;
        cube[0] = cubeCenter + Vector(-a, a, a);
        cube[1] = cube[0] + Vector(0.0f, -2.0f * a, 0.0f);
        cube[2] = cube[1] + Vector(2.0f * a, 0.0f, 0.0f);
        cube[3] = cube[2] + Vector(0.0f, 2.0f * a, 0.0f);

        int framesN = 3000;
        Vector tv(0.001f, 0.0f, 0.0f);
        for (int fi = 0; fi < framesN; fi++)
        {

            for (int vi = 0; vi < 4; vi++)
            {
                cube[vi + 4] = cube[vi] + Vector(0, 0, -2.0f * a);
            }
            fb->Set(0xFFFFFFFF);
            for (int vi = 0; vi < 8; vi++)
            {
                Vector PP;
                PlanarPinholeCamera.Project(cube[vi], PP);
                if (vi < 4)
                    fb->DrawPoint2D(PP, 13, 0xFF0000FF);
                else
                    fb->DrawPoint2D(PP, 13, 0xFFFF0000);
            }
            fb->redraw();
            Fl::check();
            PlanarPinholeCamera.Translate(tv);
        }

        return;

        framesN = 1000;
        float tstep = 0.01f;
        for (int fi = 0; fi < framesN; fi++)
        {
            fb->Set(0xFFFFFFFF);
            PlanarPinholeCamera.Project(PL, PP);
            fb->DrawPoint2D(PP, 13, 0xFFFF0000);
            PlanarPinholeCamera.Project(PR, PP);
            fb->DrawPoint2D(PP, 13, 0xFFFF0000);
            fb->redraw();
            Fl::check();
            PL[2] -= tstep;
            PR[2] -= tstep;
        }

        return;
    }

    {
        Vector v0(10.0f, -1.0f, 0.0f);
        Vector v1(3.0f, 1.0f, 2.0f);
        Vector v2(-7.0f, 0.0f, 1.0f);
        Matrix m(v0, v1, v2), minv;
        minv = m.inverted();
        cerr << minv * m.getColumn(0) << endl;
        cerr << minv * m.getColumn(1) << endl;
        cerr << minv * m.getColumn(2) << endl;
        return;

        cerr << m[0][0] << endl;
        m[0][0] = -1.0f;
        cerr << m[0][0] << endl;
        cerr << m * v2 << endl;
        return;
    }

    {
        Vector v(1.0f, 2.0f, 3.0f);
        cerr << "vector v: " << v << endl;
        cerr << "length of v: " << v.length << endl;
        cerr << "vx3: " << v * 3.0f << endl;
        cerr << "v.Normalized(): " << v.normalized() << endl;
        cerr << v.normalized().length << endl;
        return;
    }

    int stepsN = 10000;
    for (int si = 0; si < stepsN; si++)
    {
        fb->Set(0xFFFFFFFF);
        int v = fb->h / 2;
        for (int u = 100; u < 200; u++)
        {
            fb->SetSafe(u + si, v, 0xFF000000);
        }
        fb->redraw();
        Fl::check();
    }
    return;

    Vector v0(1.0f, 2.0f, 3.0f);
    Vector v1(2.0f, 4.0f, 6.0f);
    Vector v;
    v = v0 + v1;
    return;

    fb->SetChecker(32, 0xFF000000, 0xFFFFFFFF);
    fb->redraw();
    return;

    fb->Set(0xFF00FF00);
    fb->redraw();
    return;

    for (int i = 0; i < fb->w * fb->h; i++)
    {
        fb->pix[i] = 0xFF0000FF;
    }

    fb->redraw();

    cerr << endl;
    cerr << "INFO: pressed DBG button on GUI" << endl;
}

void Scene::NewButton()
{
    cerr << "INFO: pressed New button on GUI" << endl;
}
