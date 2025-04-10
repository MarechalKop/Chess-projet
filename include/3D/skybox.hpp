#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <glm/glm/glm.hpp>

class Skybox {
public:
    void init();         // Initialiser les ressources OpenGL pour la skybox
    void renderSkybox(); // Rendre la skybox dans la scène
    void shutdown();     // Libérer les ressources

private:
    unsigned int vao, vbo, ebo;  // Ressources OpenGL
    unsigned int shaderProgram;  // Shader pour la skybox
    unsigned int cubemapTexture; // Texture de la skybox
};

#endif
