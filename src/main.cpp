#include <imgui.h>
#include <iostream>
#include "quick_imgui/quick_imgui.hpp"
#include "chess_ui.hpp" // Inclusion de ton fichier

int main() {
    float value{0.f};

    quick_imgui::loop(
        "Chess",
        /* init: */ [&]() {},
        /* loop: */
        [&]() {
            ImGui::ShowDemoWindow(); // Fenêtre de démonstration ImGui
            renderChessBoard(); // Appelle la fonction qui affiche l'échiquier
        }
    );

    return 0;
}
