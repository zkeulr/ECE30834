#pragma once
#include "framebuffer.h"

class Scene
{
public:
    FrameBuffer *fb;
    Scene();
    void drawRectangle(int x, int y, int h, int w, int color = 0xFF000000);
    void drawCircle(int h, int k, int r, int color = 0xFF000000);
    void drawLine(int x1, int y1, int x2, int y2, int color = 0xFF000000);
    void writeName(int horizontalOffset = 0, int color = 0xFF000000);
    void scrollName(int color = 0xFF000000);
    void drawTriangle(int x, int y, int h, int w, int color = 0xFF000000);

    void DBG();
    void NewButton();
};

extern Scene *scene;