#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include "framebuffer.h"

class GUI;

class Scene
{
public:
    FrameBuffer *fb;
    GUI *gui;
    PlanarPinholeCamera *ppc;
    bool isRecording = false;
    std::vector<PlanarPinholeCamera> recordedFrames;

    Scene();
    void redraw();

    void Record();
    void Play();
    void Pan(float theta);
    void Tilt(float theta);
    void RollLeft(float theta);
    void RollRight(float theta);
    void MoveForward();
    void MoveBackward();
    void MoveLeft();
    void MoveRight();
    void Zoom(float zoom);
};

// Global scene pointer
extern Scene *scene;

#endif