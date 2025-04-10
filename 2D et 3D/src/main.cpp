#include <GLFW/glfw3.h>
#include <imgui.h>
#include <iostream>
#include "../include/2D/board.hpp"
#include "../include/2D/chess_renderer.hpp"
#include "../include/3D/camera.hpp"
#include "../include/3D/chess_renderer_3D.hpp"
#include "../include/3D/skybox.hpp"
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    Board           chessBoard;
    Skybox          skybox;
    ChessRenderer3D renderer3D;

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
        "Chess",
        {
            .init                     = [&]() {
                std::cout << "Init\n";
                skybox.init();
                renderer3D.init(); },
            .loop                     = [&]() { 
                skybox.renderSkybox();
                renderer3D.renderChessBoard3D(chessBoard); 
                renderChessBoard(chessBoard); },
            .key_callback             = [&](int key, int scancode, int action, int mods) {
                // Vitesse de la caméra
                const float cameraSpeed = 0.1f;  
                const float rotationSpeed = 2.0f; 

                if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                    // Déplacement de la caméra
                    if (key == GLFW_KEY_W) 
                        chess3D::moveCamera(glm::vec3(0.0f, 0.0f, -cameraSpeed)); // Avancer
                    if (key == GLFW_KEY_S) 
                        chess3D::moveCamera(glm::vec3(0.0f, 0.0f, cameraSpeed)); // Reculer
                    if (key == GLFW_KEY_A) 
                        chess3D::moveCamera(glm::vec3(-cameraSpeed, 0.0f, 0.0f)); // Gauche
                    if (key == GLFW_KEY_D) 
                        chess3D::moveCamera(glm::vec3(cameraSpeed, 0.0f, 0.0f)); // Droite

                    // Rotation de la caméra
                    if (key == GLFW_KEY_UP) 
                        chess3D::rotateCamera(rotationSpeed, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotation vers le haut
                    if (key == GLFW_KEY_DOWN) 
                        chess3D::rotateCamera(-rotationSpeed, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotation vers le bas
                    if (key == GLFW_KEY_LEFT) 
                        chess3D::rotateCamera(rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotation vers la gauche
                    if (key == GLFW_KEY_RIGHT) 
                        chess3D::rotateCamera(-rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotation vers la droite
                } },
            .mouse_button_callback    = [](int button, int action, int mods) { std::cout << "Button: " << button << " Action: " << action << " Mods: " << mods << '\n'; },
            .cursor_position_callback = [](double xpos, double ypos) { /*std::cout << "Position: " << xpos << ' ' << ypos << '\n';*/ },
            .scroll_callback          = [](double xoffset, double yoffset) { /*std::cout << "Scroll: " << xoffset << ' ' << yoffset << '\n';*/ },
            .window_size_callback     = [](int width, int height) { std::cout << "Resized: " << width << ' ' << height << '\n'; },
        }
    );

    skybox.shutdown();

    return 0;
}
