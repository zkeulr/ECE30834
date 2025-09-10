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

    Direction direction = Direction(-3.14, 3.14, 2.73);
    Point originalPoint = Point(1, 1, 1);
    Point origin = Point(0, 0, 0);

    // Store rotation data
    std::vector<double> angles;
    std::vector<double> xCoords;
    std::vector<double> yCoords;
    std::vector<double> zCoords;

    // Rotate 360 degrees with 2 degree increments
    for (int angle_degrees = 0; angle_degrees <= 360; angle_degrees += 2)
    {
        Point rotatedPoint = originalPoint.rotated(origin, direction, angle_degrees);
        angles.push_back(angle_degrees);
        xCoords.push_back(rotatedPoint.xyz[0]);
        yCoords.push_back(rotatedPoint.xyz[1]);
        zCoords.push_back(rotatedPoint.xyz[2]);
    }

    // Find min/max values for scaling
    double minVal = std::min({*std::min_element(xCoords.begin(), xCoords.end()),
                              *std::min_element(yCoords.begin(), yCoords.end()),
                              *std::min_element(zCoords.begin(), zCoords.end())});
    double maxVal = std::max({*std::max_element(xCoords.begin(), xCoords.end()),
                              *std::max_element(yCoords.begin(), yCoords.end()),
                              *std::max_element(zCoords.begin(), zCoords.end())});

    // Plot dimensions
    int margin = 50;
    int plotWidth = screenWidth - 2 * margin;
    int plotHeight = screenHeight - 2 * margin;

    // Draw axes
    drawLine(margin, screenHeight - margin, screenWidth - margin, screenHeight - margin, 0xFF000000); // X axis
    drawLine(margin, margin, margin, screenHeight - margin, 0xFF000000);                              // Y axis

    // Plot the three curves
    for (size_t i = 1; i < angles.size(); i++)
    {
        // Scale coordinates to screen space
        int x1 = margin + (int)((angles[i - 1] / 360.0) * plotWidth);
        int x2 = margin + (int)((angles[i] / 360.0) * plotWidth);

        // X coordinate curve (red)
        int y1_x = screenHeight - margin - (int)(((xCoords[i - 1] - minVal) / (maxVal - minVal)) * plotHeight);
        int y2_x = screenHeight - margin - (int)(((xCoords[i] - minVal) / (maxVal - minVal)) * plotHeight);
        drawLine(x1, y1_x, x2, y2_x, 0xFFFF0000);

        // Y coordinate curve (green)
        int y1_y = screenHeight - margin - (int)(((yCoords[i - 1] - minVal) / (maxVal - minVal)) * plotHeight);
        int y2_y = screenHeight - margin - (int)(((yCoords[i] - minVal) / (maxVal - minVal)) * plotHeight);
        drawLine(x1, y1_y, x2, y2_y, 0xFF00FF00);

        // Z coordinate curve (blue)
        int y1_z = screenHeight - margin - (int)(((zCoords[i - 1] - minVal) / (maxVal - minVal)) * plotHeight);
        int y2_z = screenHeight - margin - (int)(((zCoords[i] - minVal) / (maxVal - minVal)) * plotHeight);
        drawLine(x1, y1_z, x2, y2_z, 0xFF0000FF);
    }

    // Save as TIFF
    fb->SaveTiff("rotation_plot.tiff");
}