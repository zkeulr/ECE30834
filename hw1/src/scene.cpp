#include "GL/glew.h"
#include "scene.h"
#include "framebuffer.h"
#include <iostream>
#include <fstream>
#include <strstream>
#include <thread>
#include <chrono>

Scene *scene;

Scene::Scene()
{
    int u0 = 0;
    int v0 = 0;
    int screenHeight = 480;
    int screenWidth = 640;

    fb = new FrameBuffer(u0, v0, screenWidth, screenHeight);
    fb->Set(0xFFFFFFFF);
}

/**
 * x: horizontal coordinates of upper left corner
 * y: vertical coordinates of upper left corner
 * h: height
 * w: width
 */
void Scene::drawRectangle(int x, int y, int h, int w, int color)
{
    int x0 = x;
    int x1 = x + w - 1;
    int y0 = y;
    int y1 = y + h - 1;

    // top
    for (int i = x0; i <= x1; ++i)
    {
        fb->Set(i, y0, color);
    }
    // right
    for (int i = y0; i <= y1; ++i)
    {
        fb->Set(x1, i, color);
    }
    // bottom
    for (int i = x0; i <= x1; ++i)
    {
        fb->Set(i, y1, color);
    }
    // left
    for (int i = y0; i <= y1; ++i)
    {
        fb->Set(x0, i, color);
    }
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
}

void Scene::writeName(int horizontalOffset, int color)
{

    int screenWidth = fb->w;
    int screenHeight = fb->h;

    /*
    == == |// ==
    // |= |<  |=
    == == |\\ == o
    */

    int barWidth = screenWidth / 4 - screenWidth / 16;
    int barHeight = screenHeight / 16;

    // Z
    drawRectangle(horizontalOffset + 10, screenHeight / 4, barHeight, barWidth);
    drawLine(horizontalOffset + 10, screenHeight * 3 / 4, horizontalOffset + barWidth - 30, screenHeight / 4 + barHeight);
    drawLine(horizontalOffset + 50, screenHeight * 3 / 4, horizontalOffset + 40 + barWidth - 30, screenHeight / 4 + barHeight);
    drawRectangle(horizontalOffset + 10, screenHeight * 3 / 4, barHeight, barWidth);

    // E
    drawRectangle(horizontalOffset + 50 + barWidth, screenHeight / 4, barHeight, barWidth);
    drawRectangle(horizontalOffset + 50 + barWidth, screenHeight / 2, barHeight, barWidth);
    drawRectangle(horizontalOffset + 50 + barWidth, screenHeight * 3 / 4, barHeight, barWidth);
    drawRectangle(horizontalOffset + 50 + barWidth, screenHeight / 4, screenHeight / 2, 40);

    // K
    drawRectangle(horizontalOffset + 90 + 2 * barWidth, screenHeight / 4, screenHeight / 2 + barHeight, 40);
    drawLine(horizontalOffset + 90 + 40 + 2 * barWidth, screenHeight / 2 + barHeight, horizontalOffset + 90 + 3 * barWidth, screenHeight / 4);
    drawLine(horizontalOffset + 90 + 40 + 2 * barWidth, screenHeight / 2, horizontalOffset + 90 + 3 * barWidth, screenHeight / 4);
    drawTriangle(horizontalOffset + 90 + 40 + 2 * barWidth, screenHeight / 2 + barHeight, screenHeight / 4, barWidth - 40);

    // E
    drawRectangle(horizontalOffset + 110 + 3 * barWidth, screenHeight / 4, barHeight, barWidth);
    drawRectangle(horizontalOffset + 110 + 3 * barWidth, screenHeight / 2, barHeight, barWidth);
    drawRectangle(horizontalOffset + 110 + 3 * barWidth, screenHeight * 3 / 4, barHeight, barWidth);
    drawRectangle(horizontalOffset + 110 + 3 * barWidth, screenHeight / 4, screenHeight / 2, 40);

    // .
    drawCircle(horizontalOffset + 615, 380, 10);
    fb->redraw();
}

// alternate more flexible approach is use three drawLines
// and three points, but since not specified will assume
// right triangularity
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