#include <imgui.h>
#include <iostream>
#include <vector>
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    float value{0.f};

    quick_imgui::loop(
        "Chess",
        /* init: */ [&]() {},
        /* loop: */
        [&]() {
            ImGui::ShowDemoWindow(); // This opens a window which shows tons of examples of what you can do with ImGui. You should check it out! Also, you can use the "Item Picker" in the top menu of that demo window: then click on any widget and it will show you the corresponding code directly in your IDE!

            ImGui::Begin("Example");

            ImGui::SliderFloat("My Value", &value, 0.f, 3.f);
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.f, 0.f, 0.f, 0.f}); // Changes the color of all buttons until we call ImGui::PopStyleColor(). There is also ImGuiCol_ButtonActive and ImGuiCol_ButtonHovered
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{1.f, 1.f, 1.f, 1.f});

            ImGui::PushID("BR1");
            if (ImGui::Button("R", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 1\n";
            ImGui::SameLine(); // Draw the next ImGui widget on the same line as the previous one. Otherwise it would be below it
            ImGui::PopID();

            ImGui::PushID("BKn1"); // When some ImGui items have the same label (for exemple the next two buttons are labeled "Yo") ImGui needs you to specify an ID so that it can distinguish them. It can be an int, a pointer, a string, etc.
                              // You will definitely run into this when you create a button for each of your chess pieces, so remember to give them an ID!
            if (ImGui::Button("Kn", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 2\n";
            ImGui::PopID(); // Then pop the id you pushed after you created the widget

            ImGui::SameLine();
            ImGui::PushID("BB1");
            if (ImGui::Button("B", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID("BQ1");
            if (ImGui::Button("Q", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

             ImGui::SameLine();
            ImGui::PushID("BK");
            if (ImGui::Button("K", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

              ImGui::SameLine();
            ImGui::PushID("BB");
            if (ImGui::Button("B", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

             ImGui::SameLine();
            ImGui::PushID("BKn2");
            if (ImGui::Button("Kn", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID("BR2");
            if (ImGui::Button("R", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

             ImGui::PushID("BP1");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();
            
            ImGui::SameLine();
             ImGui::PushID("BP2");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::SameLine();
             ImGui::PushID("BP3");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::SameLine();
              ImGui::PushID("BP4");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();
            
            ImGui::SameLine();
             ImGui::PushID("BP5");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::SameLine();
             ImGui::PushID("BP6");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::SameLine();
             ImGui::PushID("BP7");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::SameLine();
             ImGui::PushID("BP8");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::PopStyleColor(2);
            
            
            ImGui::Dummy(ImVec2(0.0f, 50.0f)); // Crée un espace vertical de 50 pixels (hauteur du bouton)
            ImGui::Dummy(ImVec2(0.0f, 50.0f)); // Crée un espace vertical de 50 pixels (hauteur du bouton)
            ImGui::Dummy(ImVec2(0.0f, 50.0f)); // Crée un espace vertical de 50 pixels (hauteur du bouton)
            ImGui::Dummy(ImVec2(0.0f, 50.0f)); // Crée un espace vertical de 50 pixels (hauteur du bouton)

          

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.f, 1.f, 1.f, 1.f}); // Changes the color of all buttons until we call ImGui::PopStyleColor(). There is also ImGuiCol_ButtonActive and ImGuiCol_ButtonHovered
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{0.f, 0.f, 0.f, 1.f});

            ImGui::PushID("WR1");
            if (ImGui::Button("R", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 1\n";
            ImGui::SameLine(); // Draw the next ImGui widget on the same line as the previous one. Otherwise it would be below it
            ImGui::PopID();

            ImGui::PushID("WKn1"); // When some ImGui items have the same label (for exemple the next two buttons are labeled "Yo") ImGui needs you to specify an ID so that it can distinguish them. It can be an int, a pointer, a string, etc.
                              // You will definitely run into this when you create a button for each of your chess pieces, so remember to give them an ID!
            if (ImGui::Button("Kn", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 2\n";
            ImGui::PopID(); // Then pop the id you pushed after you created the widget

            ImGui::SameLine();
            ImGui::PushID("WB1");
            if (ImGui::Button("B", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID("WQ1");
            if (ImGui::Button("Q", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

             ImGui::SameLine();
            ImGui::PushID("WK");
            if (ImGui::Button("K", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

              ImGui::SameLine();
            ImGui::PushID("WB");
            if (ImGui::Button("B", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

             ImGui::SameLine();
            ImGui::PushID("WKn2");
            if (ImGui::Button("Kn", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID("WR2");
            if (ImGui::Button("R", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

             ImGui::PushID("WP1");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();
            
            ImGui::SameLine();
             ImGui::PushID("WP2");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::SameLine();
             ImGui::PushID("WP3");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::SameLine();
              ImGui::PushID("WP4");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();
            
            ImGui::SameLine();
             ImGui::PushID("WP5");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::SameLine();
             ImGui::PushID("WP6");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::SameLine();
             ImGui::PushID("WP7");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::SameLine();
             ImGui::PushID("WP8");
            if (ImGui::Button("P", ImVec2{50.f, 50.f}))
                std::cout << "Clicked button 3\n";
            ImGui::PopID();

            ImGui::PopStyleColor(2);


            ImGui::End();
        }
    );
}