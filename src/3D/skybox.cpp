#include "../include/3D/skybox.hpp"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "../include/3D/camera.hpp"
#include "../include/3D/shaders.hpp"
#include "glad/glad.h"

void Skybox::init()
{
    // Coordonnées pour dessiner un cube (une skybox)
    float cubeVertices[] = {
        // Positions des 8 coins du cube
        -50.0f, -50.0f, 50.0f,
        50.0f, -50.0f, 50.0f,
        50.0f, 50.0f, 50.0f,
        -50.0f, 50.0f, 50.0f,
        -50.0f, -50.0f, -50.0f,
        50.0f, -50.0f, -50.0f,
        50.0f, 50.0f, -50.0f,
        -50.0f, 50.0f, -50.0f
    };

    // Indices pour former un cube
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, // Face avant
        4, 5, 6, 6, 7, 4, // Face arrière
        4, 5, 1, 1, 0, 4, // Face inférieure
        7, 6, 2, 2, 3, 7, // Face supérieure
        4, 0, 3, 3, 7, 4, // Face gauche
        5, 1, 2, 2, 6, 5  // Face droite
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Shader pour la skybox
    const char* vertexShaderSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        uniform mat4 view;
        uniform mat4 projection;

        out vec3 TexCoords;

        void main() {
            TexCoords = aPos;  
            gl_Position = projection * view * vec4(aPos, 1.0);
        })";

    const char* fragmentShaderSrc = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 TexCoords;

        uniform samplerCube skybox;

        void main()
        {
            FragColor = texture(skybox, TexCoords);
        })";

    shaderProgram = createShaderProgram(vertexShaderSrc, fragmentShaderSrc);
    glUseProgram(shaderProgram);

    // Initialisation de la texture cubemap pour la skybox
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    // Les faces de la skybox avec des couleurs différentes
    glm::vec3 colors[6] = {
        glm::vec3(0.9f, 0.6f, 0.6f), // Rose clair (face droite)
        glm::vec3(0.6f, 0.9f, 0.6f), // Vert clair (face gauche)
        glm::vec3(0.6f, 0.6f, 0.9f), // Bleu clair (face haut)
        glm::vec3(0.9f, 0.9f, 0.6f), // Jaune clair (face bas)
        glm::vec3(0.6f, 0.9f, 0.9f), // Cyan clair (face devant)
        glm::vec3(0.9f, 0.6f, 0.9f)  // Violet clair (face derrière)
    };

    // Remplir la texture cubemap avec les couleurs des faces
    for (int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, &colors[i]);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glEnable(GL_DEPTH_TEST);
}

void Skybox::renderSkybox()
{
    // glDepthMask(GL_FALSE);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Récupérer la matrice de vue et projection
    glm::mat4 view       = glm::mat4(glm::mat3(glm::lookAt(chess3D::eye, chess3D::center, chess3D::up)));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    // Uniforms pour la matrice de vue et projection
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // Dessiner la skybox
    // glDepthMask(GL_TRUE);
}

void Skybox::shutdown()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(1, &cubemapTexture);
}
