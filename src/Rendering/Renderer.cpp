// Created by Anderson on 11/8/2023.

#include "../imports.h"
#include "../Geometry/Geometry.h"
#include "Renderer.h"

void BasicShapeRenderer::render(Geometry &object, Shader &shader, GLuint VAO) {
    // Activate the shader for rendering
    shader.useShader();

    // Bind the Vertex Array Object
    glBindVertexArray(VAO);

    // Generate and bind Vertex Buffer Object (VBO) for vertices
    GLuint gridVBO, gridIBO;
    glGenBuffers(1, &gridVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, object.vertices.size() * sizeof(GLfloat), object.vertices.data(), GL_STATIC_DRAW);

    // Generate and bind Index Buffer Object (IBO) for indices
    glGenBuffers(1, &gridIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.indices.size() * sizeof(GLuint), object.indices.data(),
                 GL_STATIC_DRAW);

    // Specify the layout of the vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

    // Draw the elements using the specified rendering mode and indices
    glDrawElements(object.type, object.indices.size(), GL_UNSIGNED_INT, 0);

    // Cleanup: delete the allocated VBO and IBO
    glDeleteBuffers(1, &gridVBO);
    glDeleteBuffers(1, &gridIBO);

    // Unbind the Vertex Array Object
    glBindVertexArray(0);
}

//template <size_t rows, size_t cols>
//Lines RayCastRenderer::DDA(int screenWidth, int screenHeight, PositionInfo2D positionInfo, int (&worldMap)[rows][cols]) {
//    vVertex vertices;
//    vIndex indices;
//
//    for (int x = 0; x < screenWidth; x++) {
//        double screenXVal = 2 * x / double(screenWidth) - 1;
//
//        vDouble2d vRayDir = (positionInfo.dir + positionInfo.plane * screenXVal);
//        vDouble2d vRayUnitStepSize{(vRayDir[0] == 0) ? 1e30 : std::abs(1 / vRayDir[0]),
//                                   (vRayDir[1] == 0) ? 1e30 : std::abs(1 / vRayDir[1])};
//        vDouble2d vRayDist;
//
//        vInt2d vMapPos{(int) positionInfo.pos[0], (int) positionInfo.pos[1]};
//        vInt2d step;
//
//        // Iterates through x and y and setup initial condition
//        for (int i = 0; i < 2; i++) {
//            step[i] = vRayDir[i] < 0 ? -1 : 1;
//            vRayDist[i] = vRayDir[i] < 0 ? (positionInfo.pos[i] - vMapPos[i]) : (vMapPos[i] + 1 - positionInfo.pos[i]);
//            vRayDist[i] *= vRayUnitStepSize[i];
//        }
//
//        int hit = 0;
//        int side = 0;
//        int max = 50;
//        int count = 0;
//
//        while (hit == 0 && count < max) {
//            // we move down the path with the shortest distance -> this is some tom foolery with booleans:
//            // if the x distance is less than y distance the bool evaluates to 0 which corresponds to x values.
//            int idx = vRayDist[0] >= vRayDist[1];
//
//            vRayDist[idx] += vRayUnitStepSize[idx];
//            vMapPos[idx] += step[idx];
//            side = idx;
//
//            if (worldMap[vMapPos[0]][vMapPos[1]] > 0) hit = 1;
//            count++;
//        }
//
//        double perpWallDist;
//        if (side == 0) perpWallDist = (vRayDist[0] - vRayUnitStepSize[0]);
//        else perpWallDist = (vRayDist[1] - vRayUnitStepSize[1]);
//
//        double lineHeight = 1 / (2 * perpWallDist);
//        float r = 0, g = 0, b = 0;
//        switch (worldMap[vMapPos[0]][vMapPos[1]]) {
//            case 1:
//                r = 1;
//                break;
//            case 2:
//                g = 1;
//                break;
//            case 3:
//                b = 1;
//                break;
//            case 4:
//                r = 1;
//                b = 1;
//                g = 1;
//                break;
//            default:
//                break;
//        }
//
//        if (side == 1) {
//            r /= 2;
//            g /= 2;
//            b /= 2;
//        }
//
//        vertices.push_back(screenXVal);
//        vertices.push_back(lineHeight);
//        vertices.push_back(0);
//        vertices.push_back(r);
//        vertices.push_back(g);
//        vertices.push_back(b);
//
//        vertices.push_back(screenXVal);
//        vertices.push_back(-lineHeight);
//        vertices.push_back(0);
//        vertices.push_back(r);
//        vertices.push_back(g);
//        vertices.push_back(b);
//
//        indices.push_back(indices.size());
//        indices.push_back(indices.size());
//    }
//    return Lines{vertices, indices, 10};
//}