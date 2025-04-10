#include "chess_controller.hpp"
#include "game_state.hpp"
#include "imgui.h"
#include "moves.hpp"
#include <cstdlib>
#include "chess_renderer.hpp"
#include <iostream>


int turnCount = 0;


void tryExpandBoard(Board& board) {
    float chance = std::min(0.05f + 0.005f * turnCount, 0.5f);
    if (RAND() < chance) {
        auto& grid = board.getBoard();

        bool addLeft = RAND() < 0.5f;

        for (auto& row : grid) {
            if (addLeft)
                row.insert(row.begin(), Piece()); // Ajoute à gauche
            else
                row.push_back(Piece()); // Ajoute à droite
        }

        std::cout << "Le plateau s'est élargi vers la " 
                  << (addLeft ? "gauche" : "droite") << " !" << std::endl;
    }
}




struct PendingPromotion {
    bool active = false;
    int  row;
    int  col;
    bool isWhite;
} pendingPromotion;

bool isWhiteTurn = true;

int drawFatigueDuration()
{
    float u1 = RAND();
    float u2 = RAND();
    float fatigue = 1 + (u1 + u2) * 1.5f; // approx loi gamma entre 1 et 4
    return static_cast<int>(std::round(fatigue));
}

struct LastMovedPiece {
    int row = -1;
    int col = -1;
    bool isWhite = true;
    bool active = false;
    int uniqueID = -1;
};
LastMovedPiece lastMovedPiece;







void selectPiece(Board& board, SelectedPiece& selected, int row, int col)
{
    const Piece& piece = board.getBoard()[row][col];

    // Vérifie si la pièce appartient au bon joueur
    if (!piece.type.empty() && ((piece.isWhite && isWhiteTurn) || (!piece.isWhite && !isWhiteTurn)))
    {
        // Empêche de sélectionner une pièce fatiguée 
        if (piece.fatigueRemaining > 0)
        {
            std::cout << "🚫 Cette pièce est fatiguée et ne peut pas être sélectionnée !" << std::endl;
            selected.isSelected = false;
            return;
        }

        selected.isSelected = true;
        selected.row        = row;
        selected.col        = col;
    }
    else
    {
        selected.isSelected = false;
    }
}


void checkTwinTowers(Board& board)
{
    auto& grid = board.getBoard();
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece& p = grid[r][c];
            if (p.type == "R") {
                // Check 4 directions (haut, bas, gauche, droite)
                const std::vector<std::pair<int,int>> dirs = {{0,1},{1,0},{0,-1},{-1,0}};
                for (auto [dr, dc] : dirs) {
                    int nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                        Piece& neighbor = grid[nr][nc];
                        if (neighbor.type == "R" && neighbor.isWhite == p.isWhite) {
                            if (RAND() < 0.05f) { // loi géométrique : p = 0.05
                                std::cout << "🗼 Deux tours adjacentes se sont effondrées !" << std::endl;
                                p = Piece();
                                neighbor = Piece();
                            }
                        }
                    }
                }
            }
        }
    }
}








// Fonction pour vérifier si un pion atteint la dernière rangée
void checkForPawnPromotion(Board& board, int row, int col)
{
    Piece& piece = board.getBoard()[row][col];

    if (piece.type == "P" && (row == 0 || row == 7)) // Si un pion atteint la dernière rangée
    {
        // Stocker les informations pour la promotion
        pendingPromotion.active  = true;
        pendingPromotion.row     = row;
        pendingPromotion.col     = col;
        pendingPromotion.isWhite = piece.isWhite;

        // Ouvrir la fenêtre modale
        ImGui::OpenPopup("Promotion");
    }
}

// Fonction pour gérer la promotion du pion
void handlePawnPromotion(Board& board)
{
    if (pendingPromotion.active && ImGui::BeginPopupModal("Promotion", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Choisissez une pièce pour la promotion :");

        Piece& piece = board.getBoard()[pendingPromotion.row][pendingPromotion.col];

        if (ImGui::Button("Dame"))
        {
            piece.type              = "Q";
            pendingPromotion.active = false;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Tour"))
        {
            piece.type              = "R";
            pendingPromotion.active = false;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Fou"))
        {
            piece.type              = "B";
            pendingPromotion.active = false;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Cavalier"))
        {
            piece.type              = "Kn";
            pendingPromotion.active = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}




void tryNervousKingMove(Board& board, bool isWhite)
{
    if (RAND() > 0.55f) return; // Loi de Bernoulli p=0.05

    auto& grid = board.getBoard();
    int kingRow = -1, kingCol = -1;

    // Recherche du roi de la bonne couleur
    for (int r = 0; r < 8; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            const Piece& p = grid[r][c];
            if (p.type == "K" && p.isWhite == isWhite)
            {
                kingRow = r;
                kingCol = c;
                break;
            }
        }
        if (kingRow != -1) break;
    }

    if (kingRow == -1) return;

    // 8 directions possibles autour du roi
    std::vector<std::pair<int, int>> directions = {
        {-1,-1}, {-1,0}, {-1,1},
        { 0,-1},         { 0,1},
        { 1,-1}, { 1,0}, { 1,1}
    };

    std::vector<std::pair<int, int>> validMoves;
    for (auto& [dr, dc] : directions)
    {
        int newRow = kingRow + dr;
        int newCol = kingCol + dc;

        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
        {
            const Piece& dest = grid[newRow][newCol];
            if (dest.type.empty() || dest.isWhite != isWhite)
            {
                validMoves.emplace_back(newRow, newCol);
            }
        }
    }

    if (validMoves.empty()) return;

    auto [r, c] = validMoves[rand() % validMoves.size()];
    grid[r][c] = grid[kingRow][kingCol];
    grid[kingRow][kingCol] = Piece();

    std::cout << "👑 Le roi " << (isWhite ? "blanc" : "noir") << " a paniqué et s'est déplacé tout seul !" << std::endl;
}







bool movePiece(Board& board, SelectedPiece& selected, int newRow, int newCol)
{
    // Réinitialise "prise en passant"
    for (auto& row : board.getBoard()) {
        for (auto& p : row) {
            if (p.type == "P") {
                p.justMovedTwoSquares = false;
            }
        }
    }

    if (!selected.isSelected)
        return false;

    // Référence vers la pièce sélectionnée
    Piece& selectedPieceRef = board.getBoard()[selected.row][selected.col];

    if (selectedPieceRef.fatigueRemaining > 0)
    {
        std::cout << "🛌 Cette pièce est trop fatiguée pour bouger (encore " 
                  << selectedPieceRef.fatigueRemaining << " tours) !" << std::endl;
        return false;
    }

    if (isValidMove(selectedPieceRef, selected.row, selected.col, newRow, newCol, board.getBoard(), lastMove))
    {
        // Gestion prise en passant
        if (selectedPieceRef.type == "P" && abs(newCol - selected.col) == 1 
            && board.getBoard()[newRow][newCol].type.empty())
        {
            board.getBoard()[selected.row][newCol] = Piece();
        }

        // 🔁 Effectuer le déplacement SANS perdre les infos de la pièce
        Piece moved = std::move(selectedPieceRef);
        board.getBoard()[selected.row][selected.col] = Piece(); // Vide la case source
        board.getBoard()[newRow][newCol] = std::move(moved);

        // 🔧 Sécurité : recopie l'ID si jamais il a été perdu par le move
board.getBoard()[newRow][newCol].uniqueID = selectedPieceRef.uniqueID;
std::cout << "🆔 ID assigné à la pièce déplacée : " 
          << board.getBoard()[newRow][newCol].uniqueID << std::endl;


        

        // 🔍 Référence sur la pièce déplacée
        Piece& movedPiece = board.getBoard()[newRow][newCol];

        std::cout << "Last piece ID: " << lastMovedPiece.uniqueID 
          << " | Moved piece ID: " << movedPiece.uniqueID << std::endl;




        // ✅ Vérifie si même pièce jouée deux fois
        bool wasSameAsLast = lastMovedPiece.active && lastMovedPiece.uniqueID == movedPiece.uniqueID;
        lastMovedPiece = {newRow, newCol, movedPiece.isWhite, true, movedPiece.uniqueID};

        // Applique la fatigue
        if (wasSameAsLast)
        {
            int fatigue = drawFatigueDuration();
            movedPiece.fatigueRemaining = fatigue;
            std::cout << "😫 La pièce a été utilisée deux fois de suite et est fatiguée pour " 
                      << fatigue << " tours !" << std::endl;
        }

        //  Décrémenter explosionCountdown
        if (movedPiece.type == "B" && movedPiece.explosionCountdown > 0) {
            movedPiece.explosionCountdown--;
        }

        //  Explosion du fou
        if (movedPiece.type == "B" && movedPiece.explosionCountdown == 0) {
            std::cout << "💥 Le fou a explosé !!! 💥\n";

            int centerRow = newRow;
            int centerCol = newCol;

            for (int dr = -2; dr <= 2; ++dr) {
                for (int dc = -2; dc <= 2; ++dc) {
                    if (abs(dr) + abs(dc) <= 2) {
                        int r = centerRow + dr;
                        int c = centerCol + dc;

                        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
                            board.getBoard()[r][c] = Piece();
                        }
                    }
                }
            }

            deselectPiece(selected);
            isWhiteTurn = !isWhiteTurn;
            return true;
        }

        // Mettre à jour le dernier coup
        lastMove = {selected.row, selected.col, newRow, newCol};

        // Gestion prise double du pion
        if (movedPiece.type == "P" && abs(newRow - selected.row) == 2)
            movedPiece.justMovedTwoSquares = true;

        // Fin de tour classique
        selected.isSelected = false;
        isWhiteTurn = !isWhiteTurn;

        // Réduire la fatigue des pièces de l'autre joueur
        for (auto& row : board.getBoard()) {
            for (auto& p : row) {
                if (!p.type.empty() && p.isWhite == isWhiteTurn && p.fatigueRemaining > 0) {
                    p.fatigueRemaining--;
                }
            }
        }

        checkVictory(board, isWhiteTurn);
        checkForPawnPromotion(board, newRow, newCol);
        tryNervousKingMove(board, isWhiteTurn);
        checkTwinTowers(board);
        tryExpandBoard(board);

        turnCount++;


        return true;
    }

    return false;
}


void deselectPiece(SelectedPiece& selected)
{
    selected.isSelected = false;
    selected.row        = -1;
    selected.col        = -1;
}

// chess_controller.cpp
std::vector<std::pair<int, int>> getValidMovesForSelected(const Board& board, const SelectedPiece& selectedPiece)
{
    if (!selectedPiece.isSelected)
        return {};

    const Piece& piece = board.getBoard()[selectedPiece.row][selectedPiece.col];
    return getValidMoves(piece, selectedPiece.row, selectedPiece.col, board.getBoard(), lastMove);
}


void resetGame(Board& board)
{
    board.resetBoard();
    gameState.gameOver = false;
    isWhiteTurn        = true; // Réinitialisation pour que ce soit toujours les Blancs qui commencent
}
