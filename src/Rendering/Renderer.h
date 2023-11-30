// Created by Anderson on 11/8/2023.

/**
 * @file Renderer.h
 * @brief Declaration of the Renderer class template and BasicShapeRenderer class.
 */

#ifndef CASTR_RENDERER_H
#define CASTR_RENDERER_H

#include "Shader.h"
#include "../Entities/Entity.h"

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

class RayCastRenderer : public BasicShapeRenderer {
public:
    /**
     * @brief Performs raycasting and generates Lines for rendering.
     * @param screenWidth Width of the screen.
     * @param screenHeight Height of the screen.
     * @param positionInfo The position information of the camera.
     * @param worldMap 2D array representing the world map.
     * @return Lines object containing vertices and indices for rendering.
     */
    template <size_t rows, size_t cols>
    Lines DDA(int screenWidth, int screenHeight, PositionInfo2D positionInfo, int (&worldMap)[rows][cols]);
    void RayCastRender();
};

template <size_t rows, size_t cols>
Lines RayCastRenderer::DDA(int screenWidth, int screenHeight, PositionInfo2D positionInfo, int (&worldMap)[rows][cols]) {
    vVertex vertices;
    vIndex indices;

    for(int x = 0; x < screenWidth; x++) {
        double screenXVal = 2 * x / double(screenWidth) - 1;

        vDouble2d vRayDir = (positionInfo.dir + positionInfo.plane * screenXVal);
        vDouble2d vRayUnitStepSize{(vRayDir[0] == 0) ? 1e30 : std::abs(1 / vRayDir[0]), (vRayDir[1] == 0) ? 1e30 : std::abs(1 / vRayDir[1])};
        vDouble2d vRayDist;

        vInt2d vMapPos{(int) positionInfo.pos[0], (int) positionInfo.pos[1]};
        vInt2d step;

        // Iterates through x and y and setup initial condition
        for (int i = 0; i < 2; i++) {
            step[i] = vRayDir[i] < 0 ? -1 : 1;
            vRayDist[i] = vRayDir[i] < 0 ? (positionInfo.pos[i] - vMapPos[i]) : (vMapPos[i] + 1 - positionInfo.pos[i]);
            vRayDist[i] *= vRayUnitStepSize[i];
        }

        int hit = 0;
        int side = 0;
        int max = 50;
        int  count = 0;

        while (hit == 0 && count < max) {
            // we move down the path with the shortest distance -> this is some tom foolery with booleans:
            // if the x distance is less than y distance the bool evaluates to 0 which corresponds to x values.
            int idx = vRayDist[0] >= vRayDist[1];

            vRayDist[idx] += vRayUnitStepSize[idx];
            vMapPos[idx] += step[idx];
            side = idx;

            if (worldMap[vMapPos[0]][vMapPos[1]] > 0) hit = 1;
            count++;
        }

        double perpWallDist;
        if(side == 0) perpWallDist = (vRayDist[0] - vRayUnitStepSize[0]);
        else          perpWallDist = (vRayDist[1] - vRayUnitStepSize[1]);

        double lineHeight = 1/(2*perpWallDist);
        float r = 0, g = 0, b =0;
        switch (worldMap[vMapPos[0]][vMapPos[1]]) {
            case 1: r = 1; break;
            case 2: g = 1; break;
            case 3: b = 1; break;
            case 4: r = 1; b = 1; g = 1; break;
            default: break;
        }

        if (side == 1) {r/=2; g/=2; b/=2;}

        vertices.push_back(screenXVal);
        vertices.push_back(lineHeight);
        vertices.push_back(0);
        vertices.push_back(r);
        vertices.push_back(g);
        vertices.push_back(b);

        vertices.push_back(screenXVal);
        vertices.push_back(-lineHeight);
        vertices.push_back(0);
        vertices.push_back(r);
        vertices.push_back(g);
        vertices.push_back(b);

        indices.push_back(indices.size());
        indices.push_back(indices.size());
    }
    return Lines{vertices, indices, 10};
}

#endif //CASTR_RENDERER_H
