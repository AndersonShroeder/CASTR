//
// Created by Anderson on 11/8/2023.
//

#include "Geometry.h"

Geometry::Geometry(vVertex vertices, vIndex indices, GLuint type, GLfloat size) {
    this->vertices = vertices;
    this->indices = indices;
    this->type = type;
    this->size = size;
}

void Geometry::renderGeometry(GLuint VAO, GLuint shaderProgram) {
    GLuint gridVBO, gridIBO;
    glGenBuffers(1, &gridVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(GLfloat), this->vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &gridIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

    glDrawElements(this->type, this->indices.size(), GL_UNSIGNED_INT, 0);

    glDeleteBuffers(1, &gridVBO);
    glDeleteBuffers(1, &gridIBO);

    glBindVertexArray(0);
}

Triangles::Triangles(vVertex vertices, vIndex indices, GLfloat size) :
                    Geometry(vertices, indices, GL_TRIANGLES, size) {}

Points::Points(vVertex vertices, vIndex indices, GLfloat size) :
                    Geometry(vertices, indices, GL_POINTS, size) {}

Lines::Lines(vVertex vertices, vIndex indices, GLfloat size) :
        Geometry(vertices, indices, GL_LINES, size) {}
