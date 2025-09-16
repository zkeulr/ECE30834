#include <iostream>
#include <fstream>
#include <strstream>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <cmath>
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

    Direction direction = Direction(-4, 3, 2);
    Point originalPoint = Point(1, 0, 1);
    Point origin = Point(0, 0, 0);

    std::vector<double> angles;
    std::vector<double> xCoords;
    std::vector<double> yCoords;
    std::vector<double> zCoords;

    // points
    for (int angle = 0; angle <= 720; angle += 2)
    {
        Point rotatedPoint = originalPoint.rotated(origin, direction, angle);

        std::cout
            << "x: "
            << rotatedPoint.xyz[0]
            << " y: " << rotatedPoint.xyz[1]
            << " z: " << rotatedPoint.xyz[2]
            << std::endl;

        angles.push_back(angle);
        xCoords.push_back(rotatedPoint.xyz[0]);
        yCoords.push_back(rotatedPoint.xyz[1]);
        zCoords.push_back(rotatedPoint.xyz[2]);
    }

    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    int scale = 100;

    for (int i = 0; i <= 360; i++)
    {
        fb->Set(50 + i, centerY + xCoords[i] * scale, 0xFF0000FF);
        fb->Set(50 + i, centerY + yCoords[i] * scale, 0x000000FF);
        fb->Set(50 + i, centerY + zCoords[i] * scale, 0x0000FFFF);
    }

    fb->SaveTiff("GRAPH.tiff");
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