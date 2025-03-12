#include <imgui.h>
#include <iostream>
#include "board.hpp"
#include "chess_renderer.hpp"
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    Board chessBoard;

    float value{0.f};

    // quick_imgui::loop(
    //     "Chess",
    //     /* init: */ [&]() {},
    //     /* loop: */
    //     [&]() {
    //         ImGui::ShowDemoWindow();      // Fenêtre de démonstration ImGui
    //         renderChessBoard(chessBoard); // Appelle la fonction qui affiche l'échiquier
    //     }
    // );

    quick_imgui::loop(
        "Quick ImGui",
        {
            .init                     = []() { std::cout << "Init\n"; },
            .loop                     = [&]() { ImGui::ShowDemoWindow(); renderChessBoard(chessBoard); },
            .key_callback             = [](int key, int scancode, int action, int mods) { std::cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " Mods: " << mods << '\n'; },
            .mouse_button_callback    = [](int button, int action, int mods) { std::cout << "Button: " << button << " Action: " << action << " Mods: " << mods << '\n'; },
            .cursor_position_callback = [](double xpos, double ypos) { std::cout << "Position: " << xpos << ' ' << ypos << '\n'; },
            .scroll_callback          = [](double xoffset, double yoffset) { std::cout << "Scroll: " << xoffset << ' ' << yoffset << '\n'; },
            .window_size_callback     = [](int width, int height) { std::cout << "Resized: " << width << ' ' << height << '\n'; },
        }
    );

    return 0;
}
