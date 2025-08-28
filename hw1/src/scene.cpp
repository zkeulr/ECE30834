#include "GL/glew.h"
#include "scene.h"
#include "framebuffer.h"
#include <iostream>
#include <fstream>
#include <strstream>

Scene *scene;

Scene::Scene()
{
    int u0 = 0;
    int v0 = 0;
    int screenHeight = 480;
    int screenWidth = 640;

    fb = new FrameBuffer(u0, v0, screenWidth, screenHeight);
    fb->Set(0xFFFFFFFF);

    writeName(screenWidth, screenHeight);

    fb->show();
}

/**
 * x: horizontal coordinates of upper left corner
 * y: vertical coordinates of upper left corner
 * h: height
 * w: width
 */
void Scene::drawRectangle(int x, int y, int h, int w, int color)
{
    // top
    for (int i = x; i < x + w; i++)
    {
        fb->Set(i, y, color);
    }

    // right
    for (int i = y + h; i > y; i--)
    {
        fb->Set(x + w, i, color);
    }

    // bottom
    for (int i = x; i < x + w; i++)
    {
        fb->Set(i, y + h, color);
    }

    // left
    for (int i = y + h; i > y; i--)
    {
        fb->Set(x, i, color);
    }

    fb->redraw();
}

/**
 * h: horizontal coordinates of center
 * k: vertical coordinates of center
 * r: radius
 */
void Scene::drawCircle(int h, int k, int r, int color)
{
    for (int x = h - r; x <= h + r; x++)
    {
        for (int y = k - r; y <= k + r; y++)
        {
            int dx = x - h;
            int dy = y - k;
            int distanceSquared = dx * dx + dy * dy;

            if (distanceSquared >= (r - 1) * (r - 1) && distanceSquared <= r * r)
            {
                fb->Set(x, y, color);
            }
        }
    }
    fb->redraw();
}

void Scene::drawLine(int x1, int y1, int x2, int y2, int color)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        fb->Set(x1, y1, color);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }

    fb->redraw();
}

void Scene::writeName(int screenWidth, int screenHeight, int color)
{

    /*
    == == |// ==
    // |= |<  |=
    == == |\\ ==
    */

    int barWidth = screenWidth / 4 - screenWidth / 16;
    int barHeight = screenHeight / 16;

    // Z
    drawRectangle(40, screenHeight / 4, barHeight, barWidth);
    drawLine(40, screenHeight * 3 / 4, barWidth, screenHeight / 4 + barHeight);
    drawLine(80, screenHeight * 3 / 4, 40 + barWidth, screenHeight / 4 + barHeight);
    drawRectangle(40, screenHeight * 3 / 4, barHeight, barWidth);

    // E
    drawRectangle(80 + barWidth, screenHeight / 4, barHeight, barWidth);
    drawRectangle(80 + barWidth, screenHeight / 2, barHeight, barWidth);
    drawRectangle(80 + barWidth, screenHeight * 3 / 4, barHeight, barWidth);
    drawRectangle(80 + barWidth, screenHeight / 4, screenHeight - 80, barWidth);

    // K

    // E

    fb->redraw();
    fb->SaveTiff("name.tiff");
}

void Scene::scrollName(int color)
{
    int frameRate = 30; // Hz
    int duration = 10;  // s

    fb->Set(0xFFFFFFFF);

    // this is very simple. just white out page and writeName
    // some amount of pixels over, as determined by frameRate
    // and duration
}

void Scene::drawTriangle(int x, int y, int h, int w, int color)
{
    // bottom
    for (int i = x; i < x + w; i++)
    {
        fb->Set(i, y + h, color);
    }

    // left
    for (int i = y + h; i > y; i--)
    {
        fb->Set(x, i, color);
    }

    // hypotenuse
    int x0 = x;
    int y0 = y;
    int x1 = x + w;
    int y1 = y + h;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int currX = x0, currY = y0;

    while (true)
    {
        fb->Set(currX, currY, color);

        if (currX == x1 && currY == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            currX += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            currY += sy;
        }
    }
}

/*
void tetris()
{
}

void pong()
{
}

*/