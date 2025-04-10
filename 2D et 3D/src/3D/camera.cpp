#include "../include/3D/camera.hpp"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

namespace chess3D {

glm::vec3 eye    = glm::vec3(4.0f, 16.0f, 10.0f); // Position initiale de la caméra
glm::vec3 center = glm::vec3(4.0f, 0.0f, 4.0f);   // Point visé (centre du plateau)
glm::vec3 up     = glm::vec3(0.0f, 1.0f, 0.0f);   // Direction vers le "haut"

const float MIN_HEIGHT = 1.0f; // Hauteur minimale au-dessus de l'échiquier

glm::mat4 getViewProjectionMatrix()
{
    // Matrice de vue : caméra placée en hauteur qui regarde le centre du plateau
    glm::mat4 view = glm::lookAt(eye, center, up);

    // Matrice de projection : perspective avec FOV 45°, ratio largeur/hauteur = 1, plan proche 0.1, plan lointain 100
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    return projection * view;
}

// Méthodes pour mettre à jour la caméra (à appeler dans main.cpp via les touches)
void moveCamera(glm::vec3 delta)
{
    // Calculer la nouvelle position de la caméra
    glm::vec3 newEye = eye + delta;

    // Limiter la position de la caméra sur l'axe Y pour ne pas descendre sous l'échiquier
    if (newEye.y < MIN_HEIGHT)
    {
        newEye.y = MIN_HEIGHT;
    }

    // Appliquer la nouvelle position à la caméra
    eye = newEye;
}

void rotateCamera(float angle, glm::vec3 axis)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    eye                = glm::vec3(rotation * glm::vec4(eye, 1.0f)); // Appliquer la rotation sur la caméra
}
} // namespace chess3D
