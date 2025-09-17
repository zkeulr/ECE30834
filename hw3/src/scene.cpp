#include "scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "PlanarPinholeCamera.h"
#include "TriangleMesh.h"
#include "gui.h"

#include <iostream>
#include <fstream>
#include <strstream>

using namespace std;

Scene *scene;

Scene::Scene()
{

    int u0 = 20;
    int v0 = 40;
    int h = 800;
    int w = 1200;
    fb = new FrameBuffer(u0, v0, w, h);
    fb->position(u0, v0);
    fb->label("Framebuffer");
    fb->Set(0xFFFFFFFF);
    fb->show();
    fb->redraw();

    float hfov = 60.0f;
    ppc = new PlanarPinholeCamera(hfov, w, h);
    ppc->Translate(Vector(0, 0, 250)); // start a bit back

    gui = new GUI();
    gui->show();
    gui->uiw->position(u0, v0 + fb->h + v0);
}

void Scene::DBG()
{

    // load triangle mesh
    TriangleMesh TriangleMesh;
    TriangleMesh.LoadBin("geometry/teapot57K.bin");
    TriangleMesh.Position(Vector(0.0f, 0.0f, -150.0f));

    // clear framebuffer
    fb->Set(0xFFFFFFFF);

    // render mesh with one point per vertex; // no triangles (yet)
    TriangleMesh.DrawPoints(0xFF000000, 3, ppc, fb);

    // refresh window
    fb->redraw();

    return;
}

void Scene::redraw()
{
    fb->Set(0xFFFFFFFF);

    TriangleMesh mesh;
    mesh.LoadBin("geometry/teapot57K.bin");
    mesh.Position(Vector(0, 0, -150));
    mesh.DrawPoints(0xFF000000, 3, ppc, fb);

    fb->redraw();
}

void Scene::MoveForward()
{
    std::cerr << "INFO: Move Forward (W)" << std::endl;
    ppc->Translate(Vector(0, 0, -5)); // negative Z = forward
    redraw();
}

void Scene::MoveBackward()
{
    std::cerr << "INFO: Move Backward (S)" << std::endl;
    ppc->Translate(Vector(0, 0, 5));
    redraw();
}

void Scene::MoveLeft()
{
    std::cerr << "INFO: Move Left (A)" << std::endl;
    ppc->Translate(Vector(-5, 0, 0));
    redraw();
}

void Scene::MoveRight()
{
    std::cerr << "INFO: Move Right (D)" << std::endl;
    ppc->Translate(Vector(5, 0, 0));
    redraw();
}

void Scene::Pan(float theta)
{
    std::cerr << "INFO: Pan " << theta << std::endl;
    ppc->Pan(theta);
    redraw();
}

void Scene::Tilt(float theta)
{
    std::cerr << "INFO: Tilt " << theta << std::endl;
    ppc->Tilt(theta);
    redraw();
}

void Scene::Roll(float theta)
{
    std::cerr << "INFO: Roll " << theta << std::endl;
    ppc->Roll(theta);
    redraw();
}
