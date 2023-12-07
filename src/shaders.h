// Created by Anderson on 11/14/2023.

/**
 * @file Shaders.h
 * @brief Declaration of vertex and fragment shader source code.
 */

#ifndef CASTR_SHADERS_H
#define CASTR_SHADERS_H

// Vertex Shader source code

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    void main() {
        gl_Position = vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)";


// Fragment Shader source code
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoord;

    uniform sampler2D ourTexture;

    void main() {
        FragColor = texture(ourTexture, TexCoord);
    }
)";

#endif //CASTR_SHADERS_H
