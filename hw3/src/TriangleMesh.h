#pragma once
#include "Vector.h"
#include "framebuffer.h"
#include "PlanarPinholeCamera.h"

/**
 * @brief Triangle mesh class that stores vertex data, colors, and triangle connectivity
 *
 * This class represents a triangle mesh with the following capabilities:
 * - Stores an array of triangle vertices, vertex colors, and triangle connectivity data
 * - Loading mesh data from binary files
 * - Basic transformations (translation, rotation, scaling)
 * - Rendering operations
 */
class TriangleMesh
{
public:
    Vector *verts;      ///< Array of vertex positions
    int vertsN;         ///< Number of vertices
    Vector *colors;     ///< Vertex colors in Vector format (one float in [0.0f, 1.0f] per R, G, and B channel)
    unsigned int *tris; ///< Array of triangle indices (triples of vertex indices)
    int trisN;          ///< Number of triangles
    Vector *normals;    ///< Array of vertex normals

    /**
     * @brief Default constructor - initializes all pointers to nullptr and counts to 0
     */
    TriangleMesh() : verts(nullptr), vertsN(0), colors(nullptr), tris(nullptr), trisN(0), normals(nullptr) {};

    /**
     * @brief Destructor - cleans up dynamically allocated memory
     */
    ~TriangleMesh();

    /**
     * @brief Loads triangle mesh data from a binary file
     * @param fname Path to the binary file containing mesh data
     */
    void LoadBin(char *fname);

    /**
     * @brief Sets the mesh as an axis-aligned box (cube)
     * @param center Center position of the box
     * @param size Size of the box (edge length)
     */
    void SetAsAxisAlignedBox(Vector center, float size);

    /**
     * @brief Computes the center of mass (centroid) of the mesh
     * @return Vector representing the centroid of all vertices
     */
    Vector GetCenter();

    /**
     * @brief Translates the mesh so its center of mass is at the specified position
     * @param newCenter The new position for the mesh's center of mass
     */
    void Position(Vector newCenter);
    /**
     * @brief Scales the mesh about its center of mass
     * @param scaleFactor Uniform scaling factor (1.0 = no change, >1.0 = larger, <1.0 = smaller)
     */
    void Scale(float scaleFactor);

    /**
     * @brief Rotates the mesh about an arbitrary axis by theta degrees
     * @param origin Point on the rotation axis (axis origin)
     * @param direction Direction of the rotation axis
     * @param theta Rotation angle in degrees
     */
    void RotateAboutArbitraryAxis(Point origin, Direction direction, float theta);

    /**
     * @brief Renders the triangle mesh in wireframe mode with vertex color interpolation
     * @param color Color of the points (32-bit RGBA)
     * @param psize Thickness of line in pixels
     * @param ppc Pointer to the camera for projection
     * @param fb Pointer to the framebuffer to draw on
     */
    void DrawWireFrame(unsigned int color, int psize, PlanarPinholeCamera *ppc, FrameBuffer *fb);

    /**
     * @brief Draws all vertices of the mesh as points
     * @param color Color of the points (32-bit RGBA)
     * @param psize Size of each point in pixels
     * @param ppc Pointer to the camera for projection
     * @param fb Pointer to the framebuffer to draw on
     */
    void DrawPoints(unsigned int color, int psize, PlanarPinholeCamera *ppc, FrameBuffer *fb);

    /**
     * @brief Translates all vertices of the mesh by the given translation vector
     * @param tv Translation vector to apply to all vertices
     */
    void Translate(Vector tv);
};