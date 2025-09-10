#include <iostream>
#include <fstream>
#include <strstream>
#include <thread>
#include <chrono>
#include "GL/glew.h"
#include "scene.h"
#include "framebuffer.h"
#include "Vector.h"
#include "Matrix.h"
#include "Point.h"
#include "Direction.h"

Scene *scene;

Scene::Scene()
{
    int u0 = 0;
    int v0 = 0;
    int screenHeight = 480;
    int screenWidth = 640;

    fb = new FrameBuffer(u0, v0, screenWidth, screenHeight);
    fb->Set(0xFFFFFFFF);

    Direction direction = Direction(-3.14, 3.14, 2.73);
    Point point = Point(1, 1, 1);
    Point origin = Point(0, 0, 0);

    for (int angle_degrees = 0; angle_degrees <= 720; angle_degrees += 2)
    {
        point = point.rotated(origin, direction, angle_degrees);
        std::cout << "x: " << point.xyz[0] << " y: " << point.xyz[1] << " z: " << point.xyz[2] << std::endl;
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
