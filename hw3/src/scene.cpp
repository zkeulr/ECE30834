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

    /*
    PlanarPinholeCamera visCam(60, w, h);
    visCam.Translate(Vector(0, 0, 500));
    ppc->Visualize(&visCam, fb, 200);
    fb->redraw();
    */

    gui = new GUI();
    gui->show();
    gui->uiw->position(u0, v0 + fb->h + v0);
}

void Scene::Record()
{
    if (!isRecording)
    {
        std::cerr << "Recording started." << std::endl;
        isRecording = true;
        recordedFrames.clear();
    }
    else
    {
        std::cerr << "Recording stopped. Saving..." << std::endl;
        isRecording = false;

        std::ofstream file("camera_path.txt");

        for (auto &cam : recordedFrames)
        {

            file << cam.w << " " << cam.h << "\n";
            file << cam.C << "\n";
            file << cam.a << "\n";
            file << cam.b << "\n";
            file << cam.c << "\n";
        }
        file.close();

        std::cerr << "Recording saved!" << std::endl;
    }
}

void Scene::Play()
{
    std::ifstream file("camera_path.txt");
    if (!file.is_open())
        return;

    std::vector<PlanarPinholeCamera> keyframes;
    while (true)
    {
        PlanarPinholeCamera cam(60, fb->w, fb->h);
        if (!(file >> cam.w >> cam.h))
            break;
        if (!(file >> cam.C))
            break;
        if (!(file >> cam.a))
            break;
        if (!(file >> cam.b))
            break;
        if (!(file >> cam.c))
            break;
        keyframes.push_back(cam);
    }
    file.close();

    if (keyframes.size() < 2)
        return;

    int totalFrames = 300; // total playback length, not per segment
    for (int frame = 0; frame < totalFrames; frame++)
    {
        float t = (float)frame / (totalFrames - 1);
        float segf = t * (keyframes.size() - 1);
        int i = (int)segf;
        float localT = segf - i;

        if (i >= (int)keyframes.size() - 1)
            i = keyframes.size() - 2;

        *ppc = keyframes[i].Interpolate(keyframes[i + 1], localT);
        redraw();
        Fl::check();
        Fl::wait(0.01);

        // save frames to convert with ffmpeg
        char fname[256];
        sprintf(fname, "frames/frame_%04d.tif", frame);
        fb->SaveTiff(fname);
    }
}

void Scene::redraw()
{
    fb->Set(0xFFFFFFFF);

    TriangleMesh mesh;
    mesh.LoadBin("geometry/teapot1K.bin");
    mesh.Position(Vector(0, 0, -150));
    mesh.DrawWireFrame(0xFF000000, 3, ppc, fb);

    if (isRecording)
    {
        recordedFrames.push_back(*ppc);
    }

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

void Scene::RollLeft(float theta)
{
    std::cerr << "INFO: Roll " << theta << std::endl;
    ppc->Roll(theta);
    redraw();
}

void Scene::RollRight(float theta)
{
    std::cerr << "INFO: Roll " << -theta << std::endl;
    ppc->Roll(-theta);
    redraw();
}

void Scene::Zoom(float zoom)
{
    std::cerr << "INFO: Zoom " << zoom << std::endl;
    ppc->Zoom(zoom);
    redraw();
}
