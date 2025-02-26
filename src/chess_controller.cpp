#include "chess_controller.hpp"
#include "moves.hpp"

void selectPiece(Board& board, SelectedPiece& selected, int row, int col)
{
    // Si une pièce est cliquée, on la sélectionne
    if (!board.getBoard()[row][col].type.empty())
    {
        selected.row        = row;
        selected.col        = col;
        selected.isSelected = true;
    }
}

void movePiece(Board& board, SelectedPiece& selected, int newRow, int newCol)
{
    if (selected.isSelected)
    {
        Piece& piece = board.getBoard()[selected.row][selected.col];

        if (isValidMove(piece, selected.row, selected.col, newRow, newCol, board.getBoard()))
        {
            // Déplacement de la pièce
            board.getBoard()[newRow][newCol]             = piece;
            board.getBoard()[selected.row][selected.col] = {"", false}; // Case vidée

            // Désélectionner la pièce après le déplacement
            selected.isSelected = false;
        }
    }
}

void deselectPiece(SelectedPiece& selected)
{
    selected.isSelected = false;
    selected.row        = -1;
    selected.col        = -1;
}
