#include "moves.hpp"
#include <cmath>

std::vector<std::pair<int, int>> getValidMoves(const Piece& piece, int row, int col, const std::vector<std::vector<Piece>>& board)
{
    std::vector<std::pair<int, int>> moves;

    // Vérifier toutes les cases où la pièce peut aller
    for (int r = 0; r < 8; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            if (isValidMove(piece, row, col, r, c, board))
            {
                moves.emplace_back(r, c);
            }
        }
    }
    return moves;
}

bool isValidMove(const Piece& piece, int startRow, int startCol, int endRow, int endCol, const std::vector<std::vector<Piece>>& board)
{
    // Vérifier que la case de destination est bien dans les limites du plateau
    if (endRow < 0 || endRow >= 8 || endCol < 0 || endCol >= 8)
        return false;

    // Gestion des mouvements des pions
    if (piece.type == "P")
    {
        int direction = piece.isWhite ? -1 : 1; // Blancs avancent vers le haut (-1), noirs vers le bas (+1)

        // Déplacement simple d'une case vers l'avant
        if (startCol == endCol && startRow + direction == endRow && board[endRow][endCol].type.empty())
            return true;

        // Premier déplacement : possibilité d'avancer de deux cases
        if (startCol == endCol && startRow + 2 * direction == endRow && board[endRow][endCol].type.empty() && board[startRow + direction][endCol].type.empty())
        {
            if ((piece.isWhite && startRow == 6) || (!piece.isWhite && startRow == 1))
                return true;
        }

        // Capture diagonale
        if (std::abs(startCol - endCol) == 1 && startRow + direction == endRow && !board[endRow][endCol].type.empty() && board[endRow][endCol].isWhite != piece.isWhite)
            return true;
    }

    // Gestion des mouvements des tours
    if (piece.type == "R") // "R" pour Rook (Tour)
    {
        // Déplacement en ligne droite (horizontal ou vertical)
        if (startRow == endRow || startCol == endCol)
        {
            // Vérification qu'il n'y a pas d'obstacles sur le chemin
            int rowStep = (endRow - startRow) == 0 ? 0 : (endRow - startRow) / std::abs(endRow - startRow);
            int colStep = (endCol - startCol) == 0 ? 0 : (endCol - startCol) / std::abs(endCol - startCol);

            int row = startRow + rowStep;
            int col = startCol + colStep;

            while (row != endRow || col != endCol)
            {
                if (!board[row][col].type.empty()) // Une pièce bloque le passage
                    return false;

                row += rowStep;
                col += colStep;
            }

            // Vérifie si la case d'arrivée est vide ou contient une pièce adverse
            if (board[endRow][endCol].type.empty() || board[endRow][endCol].isWhite != piece.isWhite)
                return true;
        }
    }

    return false; // Mouvement invalide par défaut
}
