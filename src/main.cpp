#include <imgui.h>
#include "board.hpp"
#include "chess_renderer.hpp"
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    Board chessBoard;

    float value{0.f};

    quick_imgui::loop(
        "Chess",
        /* init: */ [&]() {},
        /* loop: */
        [&]() {
            ImGui::ShowDemoWindow();      // Fenêtre de démonstration ImGui
            renderChessBoard(chessBoard); // Appelle la fonction qui affiche l'échiquier
        }
    );

    return 0;
}
