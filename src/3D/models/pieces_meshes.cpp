#include "../include/3D/models/pieces_meshes.hpp"
#include <cmath>
#include <vector>
#include "glad/glad.h"

namespace chess3D {

// Helper pour créer un VAO/VBO/EBO à partir de vertices/indices
Mesh createMeshFromData(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
    Mesh mesh;
    mesh.indexCount = static_cast<int>(indices.size());

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return mesh;
}

// PION
Mesh createPawnMesh()
{
    std::vector<float> vertices = {
        // base plate
        -0.2f, 0.0f, -0.2f,
        0.2f, 0.0f, -0.2f,
        0.2f, 0.0f, 0.2f,
        -0.2f, 0.0f, 0.2f,
        // head (top)
        0.0f, 0.5f, 0.0f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0, // base
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    return createMeshFromData(vertices, indices);
}

// === TOUR (forme simplifiée : base + "chapeau" cube) ===
Mesh createRookMesh()
{
    std::vector<float> vertices = {
        // base (cube aplati)
        -0.2f, 0.0f, -0.2f,
        0.2f, 0.0f, -0.2f,
        0.2f, 0.4f, -0.2f,
        -0.2f, 0.4f, -0.2f,
        -0.2f, 0.0f, 0.2f,
        0.2f, 0.0f, 0.2f,
        0.2f, 0.4f, 0.2f,
        -0.2f, 0.4f, 0.2f,

        // chapeau (plus petit cube)
        -0.1f, 0.4f, -0.1f,
        0.1f, 0.4f, -0.1f,
        0.1f, 0.6f, -0.1f,
        -0.1f, 0.6f, -0.1f,
        -0.1f, 0.4f, 0.1f,
        0.1f, 0.4f, 0.1f,
        0.1f, 0.6f, 0.1f,
        -0.1f, 0.6f, 0.1f
    };

    std::vector<unsigned int> indices;

    // cube de base
    unsigned int base[] = {
        0, 1, 2, 2, 3, 0, // front
        1, 5, 6, 6, 2, 1, // right
        5, 4, 7, 7, 6, 5, // back
        4, 0, 3, 3, 7, 4, // left
        3, 2, 6, 6, 7, 3, // top
        4, 5, 1, 1, 0, 4  // bottom
    };
    indices.insert(indices.end(), base, base + sizeof(base) / sizeof(unsigned int));

    // cube du haut
    unsigned int top[] = {
        8, 9, 10, 10, 11, 8,    // front
        9, 13, 14, 14, 10, 9,   // right
        13, 12, 15, 15, 14, 13, // back
        12, 8, 11, 11, 15, 12,  // left
        11, 10, 14, 14, 15, 11, // top
        12, 13, 9, 9, 8, 12     // bottom
    };
    indices.insert(indices.end(), top, top + sizeof(top) / sizeof(unsigned int));

    return createMeshFromData(vertices, indices);
}

Mesh createKnightMesh()
{
    std::vector<float> vertices = {
        // Base en forme de "L" pour le cheval
        -0.2f, 0.0f, -0.2f, // coin inférieur gauche
        0.2f, 0.0f, -0.2f,  // coin inférieur droit
        0.2f, 0.0f, 0.2f,   // coin supérieur droit
        -0.2f, 0.0f, 0.2f,  // coin supérieur gauche

        -0.2f, 0.4f, -0.2f, // coin bas gauche (partie haute du "L")
        0.2f, 0.4f, -0.2f,  // coin bas droit (partie haute du "L")
        0.2f, 0.4f, 0.2f,   // coin haut droit
        -0.2f, 0.4f, 0.2f   // coin haut gauche
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0, // base du "L" (plan inférieur)
        4, 5, 6, 6, 7, 4, // haut du "L"
        0, 1, 5, 5, 4, 0, // côté gauche
        1, 2, 6, 6, 5, 1, // côté droit
        2, 3, 7, 7, 6, 2, // côté droit supérieur
        3, 0, 4, 4, 7, 3  // côté gauche supérieur
    };

    return createMeshFromData(vertices, indices);
}

Mesh createBishopMesh()
{
    std::vector<float> vertices = {
        // Base pour le fou (cylindre ou forme arrondie)
        -0.5f, 0.0f, -0.5f,
        0.5f, 0.0f, -0.5f,
        0.5f, 0.0f, 0.5f,
        -0.5f, 0.0f, 0.5f,

        -0.5f, 1.2f, -0.5f, // haut de la base
        0.5f, 1.2f, -0.5f,
        0.5f, 1.2f, 0.5f,
        -0.5f, 1.2f, 0.5f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0, // base
        4, 5, 6, 6, 7, 4, // haut
        0, 1, 5, 5, 4, 0, // côté bas
        1, 2, 6, 6, 5, 1, // côté droit
        2, 3, 7, 7, 6, 2, // côté haut
        3, 0, 4, 4, 7, 3  // côté gauche
    };

    return createMeshFromData(vertices, indices);
}

Mesh createQueenMesh()
{
    std::vector<float> vertices = {
        // Base plus arrondi pour la reine
        -0.6f, 0.0f, -0.6f,
        0.6f, 0.0f, -0.6f,
        0.6f, 0.0f, 0.6f,
        -0.6f, 0.0f, 0.6f,

        -0.6f, 1.5f, -0.6f,
        0.6f, 1.5f, -0.6f,
        0.6f, 1.5f, 0.6f,
        -0.6f, 1.5f, 0.6f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 5, 5, 4, 0,
        1, 2, 6, 6, 5, 1,
        2, 3, 7, 7, 6, 2,
        3, 0, 4, 4, 7, 3
    };

    return createMeshFromData(vertices, indices);
}

Mesh createKingMesh()
{
    std::vector<float> vertices = {
        // Base similaire au roi
        -0.4f, 0.0f, -0.4f,
        0.4f, 0.0f, -0.4f,
        0.4f, 0.0f, 0.4f,
        -0.4f, 0.0f, 0.4f,

        -0.4f, 1.8f, -0.4f,
        0.4f, 1.8f, -0.4f,
        0.4f, 1.8f, 0.4f,
        -0.4f, 1.8f, 0.4f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 5, 5, 4, 0,
        1, 2, 6, 6, 5, 1,
        2, 3, 7, 7, 6, 2,
        3, 0, 4, 4, 7, 3
    };

    return createMeshFromData(vertices, indices);
}

} // namespace chess3D
