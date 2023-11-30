//
// Created by Anderson on 11/14/2023.
//

#include "../imports.h"

/**
 * @file Shader.h
 * @brief Declaration of the Shader class for managing OpenGL shaders.
 */

#ifndef CASTR_SHADER_H
#define CASTR_SHADER_H

/**
 * @class Shader
 * @brief Encapsulates functionality for managing OpenGL shaders.
 */
class Shader {
    /**
     * @brief ID of the shader program.
     */
    unsigned int shaderProgram;

public:
    /**
     * @brief Constructs a Shader object using vertex and fragment shader source code.
     * @param vertexShaderSource Source code for the vertex shader.
     * @param fragmentShaderSource Source code for the fragment shader.
     */
    Shader(const char *vertexShaderSource, const char *fragmentShaderSource);

    /**
     * @brief Activates (uses) the shader program.
     */
    void useShader() const;

    /**
     * @brief Retrieves the ID of the shader program.
     * @return The ID of the shader program.
     */
    [[nodiscard]] unsigned int getShaderProgram() const;
};

#endif //CASTR_SHADER_H
