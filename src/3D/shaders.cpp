#include "../include/3D/shaders.hpp"
#include <glad/glad.h>
#include <iostream>

unsigned int createShaderProgram(const char* vertexSrc, const char* fragmentSrc)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // nettoyage
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}
