#ifndef SCENE_H
#define SCENE_H

// Forward declarations
class FrameBuffer;
class GUI;
class Vector;
class Matrix;
class PlanarPinholeCamera;
class TriangleMesh;

class Scene
{
public:
    Scene();

    void DBG();
    void NewButton();

private:
    // Member variables
    FrameBuffer *fb;
    GUI *gui;
};

// Global scene pointer
extern Scene *scene;

#endif // SCENE_H