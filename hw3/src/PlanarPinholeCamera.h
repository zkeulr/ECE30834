#include "Vector.h"
#include "Point.h"
#include "Direction.h"

#pragma once

class FrameBuffer; // Forward declaration

/**
 * @brief Planar pinhole camera class for 3D projection and camera operations
 *
 * This class represents a pinhole camera model with the following capabilities:
 * - 3D to 2D projection
 * - Camera transformations (rotation, translation, zoom)
 * - Camera interpolation
 * - File I/O operations
 * - Visualization
 */
class PlanarPinholeCamera
{
public:
    Vector a, b, c, C; ///< Camera coordinate system vectors and center
    int w, h;          ///< Image width and height in pixels

    /**
     * @brief Default constructor
     */
    PlanarPinholeCamera() {};

    /**
     * @brief Constructor with horizontal field of view and image dimensions
     * @param hfov Horizontal field of view in degrees
     * @param _w Image width in pixels
     * @param _h Image height in pixels
     */
    PlanarPinholeCamera(float hfov, int _w, int _h);

    /**
     * @brief Projects a 3D point to 2D screen coordinates
     * @param P 3D point to project
     * @param PP Resulting 2D projected point
     * @return 1 if point is in front of camera, 0 otherwise
     */
    int Project(Vector P, Vector &PP);

    /**
     * @brief Translates the camera by a given vector
     * @param tv Translation vector (left-right, up-down, front-back)
     */
    void Translate(Vector tv);

    /**
     * @brief Pans the camera (rotation about vertical axis)
     * @param theta Pan angle in degrees (positive = right, negative = left)
     */
    void Pan(float theta);

    /**
     * @brief Tilts the camera (rotation about horizontal axis)
     * @param theta Tilt angle in degrees (positive = up, negative = down)
     */
    void Tilt(float theta);

    /**
     * @brief Rolls the camera (rotation about forward axis)
     * @param theta Roll angle in degrees (positive = clockwise)
     */
    void Roll(float theta);

    /**
     * @brief Zooms the camera in or out (changes focal length)
     * @param zoom_scalar Zoom factor (>1.0 = zoom in, <1.0 = zoom out)
     */
    void Zoom(float zoom_scalar);

    /**
     * @brief Interpolates between this camera and another camera
     * @param other_camera The target camera to interpolate towards
     * @param t Interpolation parameter (0.0 = this camera, 1.0 = other camera)
     * @return Interpolated camera at parameter t
     */
    PlanarPinholeCamera Interpolate(const PlanarPinholeCamera &other_camera, float t);

    /**
     * @brief Saves camera parameters to a text file
     * @param filename Path to the output text file
     */
    void SaveTextFile(const char *filename);

    /**
     * @brief Loads camera parameters from a text file
     * @param filename Path to the input text file
     * @return true if successful, false otherwise
     */
    bool LoadTextFile(const char *filename);

    /**
     * @brief Visualizes this camera in wireframe using another camera's view
     * @param viewing_camera Camera used to view this camera
     * @param fb Framebuffer to draw the wireframe on
     * @param color Color of the wireframe lines
     */
    void Visualize(PlanarPinholeCamera *viewing_camera, FrameBuffer *fb, unsigned int color);
};