#include "chess_ui.hpp"
#include <vector>

void renderChessBoard() {
    ImGui::Begin("Chess Board");

    // Couleurs pour les pièces noires et blanches
    ImVec4 blackPieceColor = ImVec4{0.f, 0.f, 0.f, 1.f};
    ImVec4 whitePieceColor = ImVec4{1.f, 1.f, 1.f, 1.f};

    ImVec4 blackTextColor = ImVec4{1.f, 1.f, 1.f, 1.f};
    ImVec4 whiteTextColor = ImVec4{0.f, 0.f, 0.f, 1.f};

    // Fonction lambda pour afficher une ligne de pièces
    auto renderRow = [&](const std::vector<std::string>& pieces, ImVec4 pieceColor, ImVec4 textColor) {
        ImGui::PushStyleColor(ImGuiCol_Button, pieceColor);
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);
        
        for (size_t i = 0; i < pieces.size(); ++i) {
            ImGui::PushID(i); // Un ID unique pour chaque bouton
            if (ImGui::Button(pieces[i].c_str(), ImVec2{50.f, 50.f})) {
                std::cout << "Clicked button: " << pieces[i] << "\n";
            }
            ImGui::PopID();
            ImGui::SameLine(); // Place le bouton suivant sur la même ligne
        }
        
        ImGui::PopStyleColor(2);
        ImGui::NewLine(); // Passe à la ligne suivante
    };

    // Pièces noires
    renderRow({"R", "Kn", "B", "Q", "K", "B", "Kn", "R"}, blackPieceColor, blackTextColor);
    renderRow({"P", "P", "P", "P", "P", "P", "P", "P"}, blackPieceColor, blackTextColor);

    // Espacement entre les pièces noires et blanches
    for (int i = 0; i < 4; ++i) {
        ImGui::Dummy(ImVec2(0.0f, 50.0f));
    }

    // Pièces blanches
    renderRow({"P", "P", "P", "P", "P", "P", "P", "P"}, whitePieceColor, whiteTextColor);
    renderRow({"R", "Kn", "B", "Q", "K", "B", "Kn", "R"}, whitePieceColor, whiteTextColor);

    ImGui::End();
}
