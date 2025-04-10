#ifndef CHESS_RENDERER_3D_HPP
#define CHESS_RENDERER_3D_HPP

#include "../include/2D/board.hpp"

class ChessRenderer3D {
public:
    void init();                           // Initialiser les ressources OpenGL
    void renderChessBoard3D(const Board&); // Afficher l’échiquier en OpenGL
private:
    unsigned int vao, vbo, ebo; // Ressources OpenGL
    unsigned int shaderProgram; // Shader
};

#endif
