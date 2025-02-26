#include "chess_controller.hpp"

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
        // Déplacer la pièce sélectionnée vers la nouvelle case
        board.getBoard()[newRow][newCol]             = board.getBoard()[selected.row][selected.col];
        board.getBoard()[selected.row][selected.col] = {"", false}; // Vider l'ancienne case

        // Réinitialiser la sélection
        selected.isSelected = false;
        selected.row        = -1;
        selected.col        = -1;
    }
}

void deselectPiece(SelectedPiece& selected)
{
    selected.isSelected = false;
    selected.row        = -1;
    selected.col        = -1;
}
