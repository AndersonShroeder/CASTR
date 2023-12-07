// Created by Anderson on 11/8/2023.

#include "Geometry.h"

#include <utility>
namespace Rendering {
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

    TextureQuad::TextureQuad(int height, int width, int colorChannels) :
                    height(height), width(width), colorChannels(colorChannels){
        this->size = height * width * colorChannels;
        this->textureData = new GLubyte[size];
        this->vertices = {
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 0.0f, 1.0f, 1.0f
        };
    }

    TextureQuad::~TextureQuad() {
        delete this->textureData;
    }

    void TextureQuad::clearTextureData() const {
        std::fill(this->textureData, this->textureData + this->size, 0);
    }

    void TextureQuad::updateVertices(vVertex vertices) {
        this->vertices = std::move(vertices);
    }
}
