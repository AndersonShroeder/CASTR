//
// Created by Anderson on 11/8/2023.
//

#ifndef CASTR_RENDERER_H
#define CASTR_RENDERER_H

template <class T>
class Renderer {

public:
    virtual void render(T object, GLuint VAO) = 0;
};

class BasicShapeRenderer : public Renderer<Geometry>{
public:
    void render(Geometry object, GLuint VAO) override;
};

#endif //CASTR_RENDERER_H
