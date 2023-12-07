// Created by Anderson on 11/8/2023.

#include "../imports.h"
#include "Renderer.h"

namespace Rendering {
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) 0);
        glEnableVertexAttribArray(0);

        // Texture coordinates attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Upload pixel data to GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, object.width, object.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     object.textureData);

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
}