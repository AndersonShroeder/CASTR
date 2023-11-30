// Created by Anderson on 11/8/2023.

/**
 * @file Geometry.h
 * @brief Declaration of structures representing geometric objects (Geometry, Triangles, Points, Lines).
 */

#include "../imports.h"

#ifndef CASTR_GEOMETRY_H
#define CASTR_GEOMETRY_H

/**
 * @brief Alias for a vector of GLfloat, representing vertex data.
 */
typedef std::vector<GLfloat> vVertex;

/**
 * @brief Alias for a vector of GLuint, representing index data.
 */
typedef std::vector<GLuint> vIndex;

/**
 * @brief Structure representing a generic geometric object.
 */
struct Geometry {
public:
    vVertex vertices; /**< Vector of vertex data. */
    vIndex indices; /**< Vector of index data. */
    GLuint type; /**< Type of geometry (GL_TRIANGLES, GL_POINTS, GL_LINES, etc.). */
    GLfloat size = 0.0f; /**< Size or scale of the geometry. */

    /**
     * @brief Constructor for initializing geometry with vertex and index data, type, and size.
     * @param vertices Vector of vertex data.
     * @param indices Vector of index data.
     * @param type Type of geometry (GL_TRIANGLES, GL_POINTS, GL_LINES, etc.).
     * @param size Size or scale of the geometry.
     */
    Geometry(vVertex &vertices, vIndex &indices, GLuint type, GLfloat size);

    /**
     * @brief Setter for the size of the geometry.
     * @param size Size or scale of the geometry.
     */
    void setSize(GLfloat size);
};

/**
 * @brief Structure representing a collection of triangles.
 */
struct Triangles : public Geometry {
    /**
     * @brief Constructor for initializing triangles with vertex and index data and size.
     * @param vertices Vector of vertex data.
     * @param indices Vector of index data.
     * @param size Size or scale of the triangles.
     */
    Triangles(vVertex &vertices, vIndex &indices, GLfloat size);
};

/**
 * @brief Structure representing a collection of points.
 */
struct Points : public Geometry {
    /**
     * @brief Constructor for initializing points with vertex and index data and size.
     * @param vertices Vector of vertex data.
     * @param indices Vector of index data.
     * @param size Size or scale of the points.
     */
    Points(vVertex &vertices, vIndex &indices, GLfloat size);
};

/**
 * @brief Structure representing a collection of lines.
 */
struct Lines : public Geometry {
    /**
     * @brief Constructor for initializing lines with vertex and index data and size.
     * @param vertices Vector of vertex data.
     * @param indices Vector of index data.
     * @param size Size or scale of the lines.
     */
    Lines(vVertex &vertices, vIndex &indices, GLfloat size);
};

#endif //CASTR_GEOMETRY_H
