#ifndef MOVES_HPP
#define MOVES_HPP

#include <vector>
#include "piece.hpp"


// Renvoie la liste des mouvements valides pour une pièce donnée
std::vector<std::pair<int, int>> getValidMoves(const Piece& piece, int row, int col, const std::vector<std::vector<Piece>>& board);

// Vérifie si un mouvement est valide
bool isValidMove(const Piece& piece, int startRow, int startCol, int endRow, int endCol, const std::vector<std::vector<Piece>>& board);

#endif // MOVES_HPP
