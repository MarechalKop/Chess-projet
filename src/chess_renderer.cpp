#include "chess_renderer.hpp"
#include <vector>
#include "board.hpp"
#include "game_state.hpp"
#include "imgui.h"

void renderChessBoard(Board& chessBoard)
{
    ImGui::Begin("Chess Board");

    // Taille des cases
    constexpr float tileSize = 60.0f;

    ImVec2 boardTopLeft = ImGui::GetCursorScreenPos();

    // Couleurs des cases du damier
    ImVec4 darkTileColor  = ImVec4{0.3f, 0.3f, 0.3f, 1.f};
    ImVec4 lightTileColor = ImVec4{0.8f, 0.8f, 0.8f, 1.f};

    // Couleurs des pièces
    ImVec4 blackPieceColor = ImVec4{0.0f, 0.0f, 0.0f, 1.f};
    ImVec4 whitePieceColor = ImVec4{1.0f, 1.0f, 1.0f, 1.f};

    // Récupération du plateau
    const auto& board = chessBoard.getBoard();

    // Dessin du damier
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            ImVec2 tilePos   = ImVec2(boardTopLeft.x + col * tileSize, boardTopLeft.y + row * tileSize);
            ImVec4 tileColor = ((row + col) % 2 == 0) ? lightTileColor : darkTileColor;

            // Dessiner la case
            ImGui::GetWindowDrawList()->AddRectFilled(tilePos, ImVec2(tilePos.x + tileSize, tilePos.y + tileSize), ImGui::ColorConvertFloat4ToU32(tileColor));

            // Afficher la pièce si présente
            if (!board[row][col].type.empty())
            {
                ImVec4 pieceColor = board[row][col].isWhite ? blackPieceColor : whitePieceColor;
                ImVec4 textColor  = board[row][col].isWhite ? ImVec4{1.f, 1.f, 1.f, 1.f} : ImVec4{0.f, 0.f, 0.f, 1.f};

                // Dessiner un carré coloré pour les pions
                ImVec2 piecePos = ImVec2(tilePos.x + 10, tilePos.y + 10);
                ImGui::GetWindowDrawList()->AddRectFilled(piecePos, ImVec2(piecePos.x + 40, piecePos.y + 40), ImGui::ColorConvertFloat4ToU32(pieceColor));

                // Dessiner la lettre au centre
                ImGui::SetCursorScreenPos(ImVec2(tilePos.x + 20, tilePos.y + 15));
                ImGui::TextColored(textColor, board[row][col].type.c_str());
            }
        }
    }

    ImGui::End();
}
