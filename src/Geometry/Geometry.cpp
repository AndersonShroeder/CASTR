// Created by Anderson on 11/8/2023.

#include "Geometry.h"

Geometry::Geometry(vVertex &vertices, vIndex &indices, GLuint type, GLfloat size) {
    this->vertices = vertices;
    this->indices = indices;
    this->type = type;
    this->size = size;
}

void Geometry::setSize(GLfloat size) {
    this->size = size;
}

Triangles::Triangles(vVertex &vertices, vIndex &indices, GLfloat size) :
        Geometry(vertices, indices, GL_TRIANGLES, size) {}

Points::Points(vVertex &vertices, vIndex &indices, GLfloat size) :
        Geometry(vertices, indices, GL_POINTS, size) {}

Lines::Lines(vVertex &vertices, vIndex &indices, GLfloat size) :
        Geometry(vertices, indices, GL_LINES, size) {}
