#include "chess_renderer.hpp"
#include <vector>
#include "board.hpp"
#include "chess_controller.hpp"
#include "game_state.hpp"
#include "imgui.h"
#include <array>
#include <cstdlib>  
#include <ctime>     
#include <cmath>
#include <algorithm> 
#include "TimeOfDay.hpp"


extern int turnCount;

static SelectedPiece selectedPiece;

float drawNormal(float mu, float sigma) {
    float u1 = RAND();
    float u2 = RAND();
    float z = sqrt(-2.0f * log(u1)) * cos(2.0f * 3.1415926f * u2);
    return mu + sigma * z;
}

float RAND()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

ImVec4 generateRandomColor(float r_min, float r_max, float g_min, float g_max, float b_min, float b_max)
{
    float r = r_min + RAND() * (r_max - r_min);
    float g = g_min + RAND() * (g_max - g_min);
    float b = b_min + RAND() * (b_max - b_min);
    return ImVec4(r, g, b, 1.0f);
}

void renderChessBoard(Board& chessBoard)
{
    ImGui::Begin("Chess Board");


// Met à jour le moment de la journée si on a changé de tour
if (currentTimeTurn != turnCount) {
    currentTime = getNextTimeOfDay(currentTime);
    currentTimeTurn = turnCount;
}

//  Appliquer la couleur du fond (avant tout le dessin)
ImVec4 bgColor = getBackgroundColor(currentTime);
ImVec2 winMin = ImGui::GetWindowPos();
ImVec2 winMax = ImVec2(winMin.x + ImGui::GetWindowWidth(), winMin.y + ImGui::GetWindowHeight());
ImGui::GetWindowDrawList()->AddRectFilled(winMin, winMax, ImGui::ColorConvertFloat4ToU32(bgColor));





    const auto& board = chessBoard.getBoard();
    int numRows = board.size();
    int numCols = board[0].size();

    static std::vector<std::vector<ImVec4>> tileColors;
    static int knownCols = 8;





    

    if (numCols != knownCols || tileColors.size() != numRows)
    {
        tileColors.resize(numRows);
        for (int row = 0; row < numRows; ++row)
        {
            tileColors[row].resize(numCols);
            for (int col = 0; col < numCols; ++col)
            {
                if ((row + col) % 2 == 0)
                    tileColors[row][col] = generateRandomColor(0.9f, 1.0f, 0.7f, 0.9f, 0.7f, 0.9f);
                else
                    tileColors[row][col] = generateRandomColor(0.2f, 0.4f, 0.4f, 0.6f, 0.8f, 1.0f);
            }
        }
        knownCols = numCols;
    }

    constexpr float tileSize = 60.0f;
    ImVec2 boardTopLeft = ImGui::GetCursorScreenPos();

    ImVec4 highlightColor = ImVec4{1.0f, 1.0f, 0.0f, 0.5f};
    ImVec4 validTileColor = ImVec4{0.0f, 1.0f, 0.0f, 0.5f};
    ImVec4 grayOverlay     = ImVec4{0.5f, 0.5f, 0.5f, 0.5f};

    for (int row = 0; row < numRows; ++row)
    {
        for (int col = 0; col < numCols; ++col)
        {
            ImVec2 tilePos   = ImVec2(boardTopLeft.x + col * tileSize, boardTopLeft.y + row * tileSize);
            ImVec4 tileColor = tileColors[row][col];

            if (selectedPiece.isSelected && selectedPiece.row == row && selectedPiece.col == col)
                tileColor = highlightColor;

            ImGui::GetWindowDrawList()->AddRectFilled(tilePos, ImVec2(tilePos.x + tileSize, tilePos.y + tileSize), ImGui::ColorConvertFloat4ToU32(tileColor));

            ImVec2 mousePos = ImGui::GetMousePos();
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                if (mousePos.x > tilePos.x && mousePos.x < tilePos.x + tileSize && mousePos.y > tilePos.y && mousePos.y < tilePos.y + tileSize)
                {
                    if (selectedPiece.isSelected)
                        movePiece(chessBoard, selectedPiece, row, col);
                    else
                        selectPiece(chessBoard, selectedPiece, row, col);
                }
            }

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                deselectPiece(selectedPiece);

            if (selectedPiece.isSelected)
            {
                const Piece& selected = chessBoard.getBoard()[selectedPiece.row][selectedPiece.col];
                if (selected.fatigueRemaining <= 0)
                {
                    std::vector<std::pair<int, int>> validMoves = getValidMovesForSelected(chessBoard, selectedPiece);
                    for (const auto& move : validMoves)
                    {
                        int r = move.first, c = move.second;
                        ImVec2 t = {boardTopLeft.x + c * tileSize, boardTopLeft.y + r * tileSize};
                        ImVec2 center = {t.x + tileSize / 2, t.y + tileSize / 2};
                        ImGui::GetWindowDrawList()->AddCircle(center, tileSize / 4, ImGui::ColorConvertFloat4ToU32(validTileColor), 12, 3.0f);
                    }
                }
            }

            if (!board[row][col].type.empty())
            {
                Piece& p = const_cast<Piece&>(board[row][col]);
                if (!p.colorInitialized)
                {
                    float sigma = 0.07f;
    float baseColor = std::clamp(drawNormal(p.isWhite ? 1.0f : 0.0f, sigma), 0.0f, 1.0f);
    p.r = baseColor;
    p.g = baseColor;
    p.b = baseColor;
    p.colorInitialized = true;
                }

                ImVec4 pieceColor = ImVec4(p.r, p.g, p.b, 1.0f);
                ImVec4 textColor = p.isWhite ? ImVec4{0.f, 0.f, 0.f, 1.f} : ImVec4{1.f, 1.f, 1.f, 1.f};
                ImVec2 piecePos = ImVec2(tilePos.x + 10, tilePos.y + 10);

                if (p.type == "B" && p.explosionCountdown > 0)
                {
                    int count = p.explosionCountdown;
                    if (count == 3) pieceColor = ImVec4(1.f, 1.f, 0.f, 1.f);
                    else if (count == 2) pieceColor = ImVec4(1.f, 0.5f, 0.f, 1.f);
                    else if (count == 1) pieceColor = ImVec4(1.f, 0.f, 0.f, 1.f);
                }

                ImGui::GetWindowDrawList()->AddRectFilled(piecePos, ImVec2(piecePos.x + 40, piecePos.y + 40), ImGui::ColorConvertFloat4ToU32(pieceColor));

                if (p.fatigueRemaining > 0)
                    ImGui::GetWindowDrawList()->AddRectFilled(piecePos, ImVec2(piecePos.x + 40, piecePos.y + 40), ImGui::ColorConvertFloat4ToU32(grayOverlay));

                ImGui::SetCursorScreenPos(ImVec2(tilePos.x + 20, tilePos.y + 15));
                ImGui::TextColored(textColor, p.type.c_str());
            }
        }
    }

    handlePawnPromotion(chessBoard);
    showVictoryPopup(chessBoard);

    ImGui::End();
}
