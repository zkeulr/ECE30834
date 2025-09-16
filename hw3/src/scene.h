#pragma once
#include "framebuffer.h"

class Scene
{
public:
    FrameBuffer *fb;
    Scene();
    void drawLine(int x1, int y1, int x2, int y2, int color = 0xFFFFFFFF);
};