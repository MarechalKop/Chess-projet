#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "piece.hpp"

class Board {
public:
    Board(); // Constructeur pour initialiser le plateau

    std::vector<std::vector<Piece>>& getBoard(); // Accès au plateau

private:
    std::vector<std::vector<Piece>> board; // Stocke l'état des pièces
};

#endif
