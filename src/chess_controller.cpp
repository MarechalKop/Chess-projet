#include "chess_controller.hpp"
#include "moves.hpp"

bool isWhiteTurn = true;

void selectPiece(Board& board, SelectedPiece& selected, int row, int col)
{
    const Piece& piece = board.getBoard()[row][col];

    // Vérifier si la pièce appartient au joueur dont c'est le tour
    if (!piece.type.empty() && ((piece.isWhite && isWhiteTurn) || (!piece.isWhite && !isWhiteTurn)))
    {
        selected.isSelected = true;
        selected.row        = row;
        selected.col        = col;
    }
    else
    {
        selected.isSelected = false; // Désélectionner si la pièce ne correspond pas au tour
    }
}

bool movePiece(Board& board, SelectedPiece& selected, int newRow, int newCol)
{
    if (!selected.isSelected)
        return false;

    Piece& piece = board.getBoard()[selected.row][selected.col];

    // Vérifier si c'est bien le tour du joueur
    if ((piece.isWhite && !isWhiteTurn) || (!piece.isWhite && isWhiteTurn))
    {
        return false;
    }

    if (isValidMove(piece, selected.row, selected.col, newRow, newCol, board.getBoard()))
    {
        // Déplacer la pièce
        board.getBoard()[newRow][newCol]             = piece;
        board.getBoard()[selected.row][selected.col] = {"", false}; // Case vidée

        // Désélectionner la pièce après le déplacement
        selected.isSelected = false;

        // Changer de tour
        isWhiteTurn = !isWhiteTurn;

        return true; // Déplacement réussi
    }

    return false; // Déplacement invalide
}

void deselectPiece(SelectedPiece& selected)
{
    selected.isSelected = false;
    selected.row        = -1;
    selected.col        = -1;
}

std::vector<std::pair<int, int>> getValidMovesForSelected(const Board& board, const SelectedPiece& selectedPiece)
{
    if (!selectedPiece.isSelected)
    {
        return {};
    }

    const Piece& piece = board.getBoard()[selectedPiece.row][selectedPiece.col];
    return getValidMoves(piece, selectedPiece.row, selectedPiece.col, board.getBoard());
}
