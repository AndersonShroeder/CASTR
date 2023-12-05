// Created by Anderson on 11/8/2023.

/**
 * @file Renderer.h
 * @brief Declaration of the Renderer class template and BasicShapeRenderer class.
 */

#ifndef CASTR_RENDERER_H
#define CASTR_RENDERER_H

#include "Shader.h"
#include "../Entities/Entity.h"
#include "../Geometry/Geometry.h"

/**
 * @brief Generic template class for rendering objects using a shader and VAO.
 * @tparam T The type of object to be rendered.
 */
template <class T>
class Renderer {

public:
    /**
     * @brief Renders an object using the specified shader and VAO.
     * @param object The object to be rendered.
     * @param shader The shader to be used for rendering.
     * @param VAO The Vertex Array Object representing the geometry of the object.
     */
    virtual void render(T &object, Shader &shader, GLuint VAO) = 0;
};

class TextureRenderer : public Renderer<TextureQuad> {
public:
    TextureRenderer();
    void render(TextureQuad &object, Shader &shader, GLuint VAO) override;
private:
    GLuint textureID{};
};

/**
 * @brief Specialized renderer class for rendering Basic Shapes.
 */
class BasicShapeRenderer : public Renderer<Geometry>{
public:
    /**
     * @brief Renders a Geometry object using the specified shader and VAO.
     * @param object The Geometry object to be rendered.
     * @param shader The shader to be used for rendering.
     * @param VAO The Vertex Array Object representing the geometry of the object.
     */
    void render(Geometry &object, Shader &shader, GLuint VAO) override;
};

class RayCastRenderer : public TextureRenderer {
public:
    explicit RayCastRenderer(TextureQuad &quad);
    /**
     * @brief Performs raycasting and generates Lines for rendering.
     * @param screenWidth Width of the screen.
     * @param screenHeight Height of the screen.
     * @param positionInfo The position information of the camera.
     * @param worldMap 2D array representing the world map.
     * @return Lines object containing vertices and indices for rendering.
     */
    template <std::size_t rows, std::size_t cols>
    void DDA(PositionInfo2D positionInfo, int (&worldMap)[rows][cols]);

    template <std::size_t rows, std::size_t cols>
    void render(Shader shader, GLuint VAO, PositionInfo2D pos, int (&worldMap)[rows][cols]);

private:
    TextureQuad &quad;
};

#endif //CASTR_RENDERER_H
