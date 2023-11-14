//
// Created by Anderson on 11/14/2023.
//
#include "../imports.h"
#ifndef CASTR_SHADER_H
#define CASTR_SHADER_H


class Shader {
    unsigned int shaderProgram;
public:
    Shader(const char *vertexShaderSource, const char *fragmentShaderSource);

    void useShader();

    [[nodiscard]] unsigned int getShaderProgram() const;
};


#endif //CASTR_SHADER_H
