#ifndef CHESS_CONTROLLER_HPP
#define CHESS_CONTROLLER_HPP

#include "board.hpp"

struct SelectedPiece {
    int  row = -1, col = -1;
    bool isSelected = false;
};

void                             selectPiece(Board& board, SelectedPiece& selected, int row, int col);
void                             movePiece(Board& board, SelectedPiece& selected, int newRow, int newCol);
void                             deselectPiece(SelectedPiece& selected);
std::vector<std::pair<int, int>> getValidMovesForSelected(const Board& board, const SelectedPiece& selectedPiece);

#endif // CHESS_CONTROLLER_HPP
