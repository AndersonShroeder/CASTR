// Created by Anderson on 11/14/2023.

#include "Shader.h"

Shader::Shader(const char *vertexShaderSource, const char *fragmentShaderSource) {
    unsigned int vertexShader, fragmentShader;

    // Create and compile vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Create and compile fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Create and link shader program
    this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, vertexShader);
    glAttachShader(this->shaderProgram, fragmentShader);
    glLinkProgram(this->shaderProgram);

    // Cleanup: delete individual shaders as they are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int Shader::getShaderProgram() const {
    return this->shaderProgram;
}

void Shader::useShader() const {
    glUseProgram(this->shaderProgram);
}
