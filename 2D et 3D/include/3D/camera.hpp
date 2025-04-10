#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm/glm.hpp>

namespace chess3D {
extern glm::vec3 eye;
extern glm::vec3 center;
extern glm::vec3 up;

glm::mat4 getViewProjectionMatrix();
void      moveCamera(glm::vec3 delta);
void      rotateCamera(float angle, glm::vec3 axis);
} // namespace chess3D

#endif
