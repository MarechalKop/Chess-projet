#include "../include/3D/chess_renderer_3D.hpp"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "../include/3D/camera.hpp"
#include "../include/3D/models/pieces_meshes.hpp"
#include "../include/3D/shaders.hpp"
#include "glad/glad.h"

Mesh pawnMesh;
Mesh rookMesh;
Mesh bishopMesh;
Mesh knightMesh;
Mesh queenMesh;
Mesh kingMesh;

void ChessRenderer3D::init()
{
    pawnMesh   = chess3D::createPawnMesh();
    rookMesh   = chess3D::createRookMesh();
    bishopMesh = chess3D::createBishopMesh();
    knightMesh = chess3D::createKnightMesh();
    queenMesh  = chess3D::createQueenMesh();
    kingMesh   = chess3D::createKingMesh();

    // Coordonnées pour dessiner un cube (en 3D)
    float cubeVertices[] = {
        // Face avant
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // bas-gauche avant
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // bas-droit avant
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,   // haut-droit avant
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,  // haut-gauche avant

        // Face arrière
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // bas-gauche arrière
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bas-droit arrière
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,   // haut-droit arrière
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  // haut-gauche arrière
    };

    // Indices pour dessiner les triangles du cube (deux triangles par face)
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, // face avant
        4, 5, 6, 6, 7, 4, // face arrière
        4, 5, 1, 1, 0, 4, // face inférieure
        7, 6, 2, 2, 3, 7, // face supérieure
        4, 0, 3, 3, 7, 4, // face gauche
        5, 1, 2, 2, 6, 5  // face droite
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    // Initialisation shader ici (à compléter)
    const char* vertexShaderSrc = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    uniform mat4 model;
    uniform mat4 vp;

    out vec3 vertexColor;

    void main() {
        vertexColor = aColor;
        gl_Position = vp * model * vec4(aPos, 1.0);
    }
    )";

    const char* fragmentShaderSrc = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec3 color;  // Uniform pour la couleur du cube

    void main()
    {
        FragColor = vec4(color, 1.0);
    }
    )";

    shaderProgram = createShaderProgram(vertexShaderSrc, fragmentShaderSrc);
    glUseProgram(shaderProgram);
}

void ChessRenderer3D::renderChessBoard3D(const Board&)
{
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Récupère la matrice view-projection
    glm::mat4 vp = chess3D::getViewProjectionMatrix();

    // Envoie la matrice au shader (à condition d’avoir une uniform "vp" côté shader)
    GLint vpLoc = glGetUniformLocation(shaderProgram, "vp");
    glUniformMatrix4fv(vpLoc, 1, GL_FALSE, &vp[0][0]);

    glBindVertexArray(vao);

    // Couleur de la bordure marron
    glm::vec3 borderColor(0.5f, 0.25f, 0.0f); // Marron foncé

    // Dessiner la bordure (un grand rectangle autour de l'échiquier)
    for (int x = -1; x <= 8; ++x)
    {
        for (int z = -1; z <= 8; ++z)
        {
            // Dessiner uniquement les cases extérieures pour former un bord autour de l'échiquier
            if (x == -1 || x == 8 || z == -1 || z == 8)
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, z));
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
                glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &borderColor[0]);

                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
        }
    }

    // Boucle pour dessiner 8x8 cases avec un décalage
    for (int x = 0; x < 8; ++x)
    {
        for (int z = 0; z < 8; ++z)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, z));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

            // Alternance des couleurs : noir/blanc
            glm::vec3 color = (x + z) % 2 == 0 ? glm::vec3(1.0f) : glm::vec3(0.1f);
            glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &color[0]);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // Dessiner les 36 indices du cube
        }
    }

    for (int x = 0; x < 8; ++x)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(x, 0.0f, 1.0f)); // rangée 2 (z = 1)
        model           = glm::scale(model, glm::vec3(2.0f));              // taille

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(glm::vec3(0.9f))); // blanc

        glBindVertexArray(pawnMesh.vao);
        glDrawElements(GL_TRIANGLES, pawnMesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    for (int x = 0; x < 8; ++x)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(x, 0.0f, 6.0f)); // rangée 7 (z = 6)
        model           = glm::scale(model, glm::vec3(2.0f));

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(glm::vec3(0.2f))); // noir

        glBindVertexArray(pawnMesh.vao);
        glDrawElements(GL_TRIANGLES, pawnMesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    // DESSIN DES AUTRES PIÈCES

    // Tour (positionner sur les coins de l'échiquier)
    for (int i = 0; i < 2; ++i)
    {
        int       positions[2] = {0, 7};
        glm::mat4 model        = glm::mat4(1.0f);
        model                  = glm::translate(model, glm::vec3(positions[i], 0.0f, 0.0f)); // positions des tours sur la rangée 1 et 8
        model                  = glm::scale(model, glm::vec3(2.0f));                         // taille

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(glm::vec3(0.9f))); // blanc

        glBindVertexArray(rookMesh.vao);
        glDrawElements(GL_TRIANGLES, rookMesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    // Cavalier (positionner à côté des tours)
    for (int i = 0; i < 2; ++i)
    {
        int       positions[2] = {1, 6};
        glm::mat4 model        = glm::mat4(1.0f);
        model                  = glm::translate(model, glm::vec3(positions[i], 0.0f, 0.0f)); // positions des cavaliers
        model                  = glm::scale(model, glm::vec3(2.0f));                         // taille

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(glm::vec3(0.9f))); // blanc

        glBindVertexArray(knightMesh.vao);
        glDrawElements(GL_TRIANGLES, knightMesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    // Fou (positionner à côté des cavaliers)
    for (int i = 0; i < 2; ++i)
    {
        int       positions[2] = {2, 5};
        glm::mat4 model        = glm::mat4(1.0f);
        model                  = glm::translate(model, glm::vec3(positions[i], 0.0f, 0.0f)); // positions des fous
        model                  = glm::scale(model, glm::vec3(0.75f));                        // taille

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(glm::vec3(0.9f))); // blanc

        glBindVertexArray(bishopMesh.vao);
        glDrawElements(GL_TRIANGLES, bishopMesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    // Reine (positionner au centre de la rangée 1 et 8)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(3.f, 0.0f, 0.0f)); // position de la reine
        model           = glm::scale(model, glm::vec3(0.75f));               // taille

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(glm::vec3(0.9f))); // blanc

        glBindVertexArray(queenMesh.vao);
        glDrawElements(GL_TRIANGLES, queenMesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    // Roi (positionner au centre de la rangée 1 et 8)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(4.f, 0.0f, 0.0f)); // position du roi
        model           = glm::scale(model, glm::vec3(1.0f));                // taille

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(glm::vec3(0.9f))); // blanc

        glBindVertexArray(kingMesh.vao);
        glDrawElements(GL_TRIANGLES, kingMesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    // Répéter pour la couleur noire sur la rangée 7 et 8

    // DESSIN NOIR - RANGÉE 7 ET 8 (similaire à la partie blanche ci-dessus)
    for (int x = 0; x < 8; ++x)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(x, 0.0f, 6.0f)); // rangée 7 (noir)
        model           = glm::scale(model, glm::vec3(2.0f));

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(glm::vec3(0.2f))); // noir

        glBindVertexArray(pawnMesh.vao);
        glDrawElements(GL_TRIANGLES, pawnMesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    // Place les autres pièces noires sur la rangée 8 (tour, cavalier, fou, reine, roi)
    // Tour
    for (int i = 0; i < 2; ++i)
    {
        int       positions[2] = {0, 7};
        glm::mat4 model        = glm::mat4(1.0f);
        model                  = glm::translate(model, glm::vec3(positions[i], 0.0f, 7.0f)); // position des tours noires
        model                  = glm::scale(model, glm::vec3(2.0f));

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(glm::vec3(0.2f))); // noir

        glBindVertexArray(rookMesh.vao);
        glDrawElements(GL_TRIANGLES, rookMesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    // Cavalier
    for (int i = 0; i < 2; ++i)
    {
        int       positions[2] = {1, 6};
        glm::mat4 model        = glm::mat4(1.0f);
        model                  = glm::translate(model, glm::vec3(positions[i], 0.0f, 7.0f)); // positions des cavaliers
        model                  = glm::scale(model, glm::vec3(2.0f));                         // taille

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(glm::vec3(0.2f)));

        glBindVertexArray(knightMesh.vao);
        glDrawElements(GL_TRIANGLES, knightMesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    // Fou (positionner à côté des cavaliers)
    for (int i = 0; i < 2; ++i)
    {
        int       positions[2] = {2, 5};
        glm::mat4 model        = glm::mat4(1.0f);
        model                  = glm::translate(model, glm::vec3(positions[i], 0.0f, 7.0f)); // positions des fous
        model                  = glm::scale(model, glm::vec3(0.75f));                        // taille

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(glm::vec3(0.2f)));

        glBindVertexArray(bishopMesh.vao);
        glDrawElements(GL_TRIANGLES, bishopMesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    // Reine (positionner au centre de la rangée 1 et 8)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(3.f, 0.0f, 7.0f)); // position de la reine
        model           = glm::scale(model, glm::vec3(0.75f));               // taille

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(glm::vec3(0.2f)));

        glBindVertexArray(queenMesh.vao);
        glDrawElements(GL_TRIANGLES, queenMesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    // Roi (positionner au centre de la rangée 1 et 8)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(4.f, 0.0f, 7.0f)); // position du roi
        model           = glm::scale(model, glm::vec3(1.0f));                // taille

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(glm::vec3(0.2f)));

        glBindVertexArray(kingMesh.vao);
        glDrawElements(GL_TRIANGLES, kingMesh.indexCount, GL_UNSIGNED_INT, 0);
    }
}
