#include "board.hpp"

Board::Board()
{
    // Initialisation du plateau avec les pièces en position de départ
    board = {
        {{"R", true}, {"Kn", true}, {"B", true}, {"Q", true}, {"K", true}, {"B", true}, {"Kn", true}, {"R", true}},
        {{"P", true}, {"P", true}, {"P", true}, {"P", true}, {"P", true}, {"P", true}, {"P", true}, {"P", true}},
        {{"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}},
        {{"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}},
        {{"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}},
        {{"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}},
        {{"P", false}, {"P", false}, {"P", false}, {"P", false}, {"P", false}, {"P", false}, {"P", false}, {"P", false}},
        {{"R", false}, {"Kn", false}, {"B", false}, {"Q", false}, {"K", false}, {"B", false}, {"Kn", false}, {"R", false}}
    };
}

std::vector<std::vector<Piece>>& Board::getBoard()
{
    return board;
}
