#include "chess_renderer.hpp"
#include <vector>
#include "board.hpp"
#include "chess_controller.hpp"
#include "game_state.hpp"
#include "imgui.h"

static SelectedPiece selectedPiece;

void renderChessBoard(Board& chessBoard)
{
    ImGui::Begin("Chess Board");

    // Taille des cases
    constexpr float tileSize = 60.0f;

    ImVec2 boardTopLeft = ImGui::GetCursorScreenPos();

    // Couleurs des cases du damier
    ImVec4 darkTileColor  = ImVec4{0.3f, 0.3f, 0.3f, 1.f};
    ImVec4 lightTileColor = ImVec4{0.8f, 0.8f, 0.8f, 1.f};
    ImVec4 highlightColor = ImVec4{1.0f, 1.0f, 0.0f, 0.5f};

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

            // Si cette case est sélectionnée, la surligner
            if (selectedPiece.isSelected && selectedPiece.row == row && selectedPiece.col == col)
            {
                tileColor = highlightColor;
            }

            // Dessiner la case
            ImGui::GetWindowDrawList()->AddRectFilled(tilePos, ImVec2(tilePos.x + tileSize, tilePos.y + tileSize), ImGui::ColorConvertFloat4ToU32(tileColor));

            // Vérifier si la case est cliquée
            ImVec2 mousePos = ImGui::GetMousePos();
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                if (mousePos.x > tilePos.x && mousePos.x < tilePos.x + tileSize && mousePos.y > tilePos.y && mousePos.y < tilePos.y + tileSize)
                {
                    if (selectedPiece.isSelected)
                    {
                        movePiece(chessBoard, selectedPiece, row, col); // Déplacer la pièce
                    }
                    else
                    {
                        selectPiece(chessBoard, selectedPiece, row, col); // Sélectionner une pièce
                    }
                }
            }

            // Désélectionner avec un clic droit
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                deselectPiece(selectedPiece);
            }

            // Afficher la pièce si présente
            if (!board[row][col].type.empty())
            {
                ImVec4 pieceColor = board[row][col].isWhite ? whitePieceColor : blackPieceColor;
                ImVec4 textColor  = board[row][col].isWhite ? ImVec4{0.f, 0.f, 0.f, 1.f} : ImVec4{1.f, 1.f, 1.f, 1.f};

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
