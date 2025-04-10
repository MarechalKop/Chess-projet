#ifndef PIECES_MESHES_HPP
#define PIECES_MESHES_HPP

#include <glad/glad.h>

struct Mesh {
    GLuint  vao        = 0;
    GLuint  vbo        = 0;
    GLuint  ebo        = 0;
    GLsizei indexCount = 0;
};

namespace chess3D {

// Fonctions de création de mesh pour chaque pièce
Mesh createPawnMesh();
Mesh createRookMesh();
Mesh createKnightMesh();
Mesh createBishopMesh();
Mesh createQueenMesh();
Mesh createKingMesh();

} // namespace chess3D

#endif // PIECES_MESHES_HPP
