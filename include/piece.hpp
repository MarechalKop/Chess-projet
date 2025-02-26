#ifndef PIECE_HPP
#define PIECE_HPP

#include <string>

struct Piece {
    std::string type;    // Exemple : "P" pour pion, "R" pour tour, "K" pour roi, etc.
    bool        isWhite; // true si la pièce est blanche, false si noire
};

#endif // PIECE_HPP
