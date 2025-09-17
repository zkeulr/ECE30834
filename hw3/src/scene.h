#ifndef SCENE_H
#define SCENE_H
#include "framebuffer.h"

class GUI;

class Scene
{
public:
    FrameBuffer *fb;
    GUI *gui;
    PlanarPinholeCamera *ppc;

    Scene();
    void redraw();

    void DBG();
    void Pan(float theta);
    void Tilt(float theta);
    void Roll(float theta);
    void MoveForward();
    void MoveBackward();
    void MoveLeft();
    void MoveRight();
};

// Global scene pointer
extern Scene *scene;

#endif