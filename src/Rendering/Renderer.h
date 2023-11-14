//
// Created by Anderson on 11/8/2023.
//

#ifndef CASTR_RENDERER_H
#define CASTR_RENDERER_H

#include "Shader.h"

template <class T>
class Renderer {

public:
    virtual void render(T &object, Shader &shader, GLuint VAO) = 0;
};

class BasicShapeRenderer : public Renderer<Geometry>{
public:
    void render(Geometry &object, Shader &shader, GLuint VAO) override;
};

#endif //CASTR_RENDERER_H
