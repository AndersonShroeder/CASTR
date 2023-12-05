// Created by Anderson on 11/8/2023.

#include "../imports.h"
#include "Renderer.h"

TextureRenderer::TextureRenderer() {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void TextureRenderer::render(TextureQuad &object, Shader &shader, GLuint VAO) {
    // Vertex Array Object (VAO), Vertex Buffer Object (VBO), and Element Buffer Object (EBO)
    GLuint VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, object.vertices.size() * sizeof(GLfloat), object.vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Upload pixel data to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, object.width, object.height, 0, GL_RGB, GL_UNSIGNED_BYTE, object.textureData);

    // Set the texture uniform in the shader
    glUniform1i(glGetUniformLocation(shader.getShaderProgram(), "ourTexture"), 0);
// Bind the texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Render the quad
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

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

template <std::size_t rows, std::size_t cols>
void RayCastRenderer::DDA(PositionInfo2D positionInfo, int (&worldMap)[rows][cols]) {
    int screenWidth = quad.width;
    int screenHeight = quad.height;
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

        int lineHeight = (int) screenHeight/(perpWallDist);
        int r = 0, g = 0, b =0;
        switch (worldMap[vMapPos[0]][vMapPos[1]]) {
            case 1: r = 255; break;
            case 2: g = 255; break;
            case 3: b = 255; break;
            case 4: r = 255; b = 255; g = 255; break;
            default: break;
        }

        if (side == 1) {r/=2; g/=2; b/=2;}

        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if(drawStart < 0)drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if(drawEnd >= screenHeight)drawEnd = screenHeight - 1;

        for (int y = drawStart; y < drawEnd; y++) {
            quad.textureData[(screenWidth*y + x)*3 + 0] = r;
            quad.textureData[(screenWidth*y + x)*3 + 1] = g;
            quad.textureData[(screenWidth*y + x)*3 + 2] = b;
        }
    }
}

template void RayCastRenderer::DDA<24ull, 24ull>(PositionInfo2D, int (&) [24ull][24ull]);

RayCastRenderer::RayCastRenderer(TextureQuad &quad) : quad(quad){}

template <std::size_t rows, std::size_t cols>
void RayCastRenderer::render(Shader shader, GLuint VAO, PositionInfo2D pos, int (&worldMap)[rows][cols]) {
    DDA(pos, worldMap);
    TextureRenderer::render(quad, shader, VAO);
    quad.clearTextureData();
}

template void RayCastRenderer::render<24ull, 24ull>(Shader, GLuint, PositionInfo2D, int (&) [24ull][24ull]);