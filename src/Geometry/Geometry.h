//
// Created by Anderson on 11/8/2023.
//

#include "../imports.h"

#ifndef CASTR_GEOMETRY_H
#define CASTR_GEOMETRY_H

typedef std::vector<GLfloat> vVertex;
typedef std::vector<GLuint> vIndex;

struct Geometry {
public:
    vVertex vertices;
    vIndex indices;
    GLuint type;
    GLfloat size;

    Geometry(vVertex vertices, vIndex indices, GLuint type, GLfloat size);

    // Should be overridden for more complicated render needs
    virtual void renderGeometry(GLuint VAO, GLuint shaderProgram);

};

struct Triangles: public Geometry {
    Triangles(vVertex vertices, vIndex indices, GLfloat size);
};

struct Points: public Geometry {
    Points(vVertex vertices, vIndex indices, GLfloat size);
};

struct Lines: public Geometry {
    Lines(vVertex vertices, vIndex indices, GLfloat size);
};

#endif //CASTR_GEOMETRY_H
