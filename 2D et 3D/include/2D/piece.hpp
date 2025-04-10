// piece.hpp
#ifndef PIECE_HPP
#define PIECE_HPP

#include <string>

struct Piece {
    std::string type;
    bool        isWhite;
    bool        justMovedTwoSquares = false; // <-- AJOUTER CECI
};

#endif // PIECE_HPP
