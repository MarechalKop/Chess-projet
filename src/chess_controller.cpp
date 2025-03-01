#include "chess_controller.hpp"
#include "moves.hpp"
#include "imgui.h" // Pour afficher une fenêtre modale

struct PendingPromotion {
    bool active = false;
    int row;
    int col;
    bool isWhite;
} pendingPromotion;

bool isWhiteTurn = true;

void selectPiece(Board& board, SelectedPiece& selected, int row, int col)
{
    const Piece& piece = board.getBoard()[row][col];

    // Vérifier si la pièce appartient au joueur dont c'est le tour
    if (!piece.type.empty() && ((piece.isWhite && isWhiteTurn) || (!piece.isWhite && !isWhiteTurn)))
    {
        selected.isSelected = true;
        selected.row        = row;
        selected.col        = col;
    }
    else
    {
        selected.isSelected = false; // Désélectionner si la pièce ne correspond pas au tour
    }
}

// Fonction pour vérifier si un pion atteint la dernière rangée
void checkForPawnPromotion(Board& board, int row, int col)
{
    Piece& piece = board.getBoard()[row][col];

    if (piece.type == "P" && (row == 0 || row == 7)) // Si un pion atteint la dernière rangée
    {
        // Stocker les informations pour la promotion
        pendingPromotion.active = true;
        pendingPromotion.row = row;
        pendingPromotion.col = col;
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
            piece.type = "Q";
            pendingPromotion.active = false;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Tour"))
        {
            piece.type = "R";
            pendingPromotion.active = false;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Fou"))
        {
            piece.type = "B";
            pendingPromotion.active = false;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Cavalier"))
        {
            piece.type = "Kn";
            pendingPromotion.active = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}


bool movePiece(Board& board, SelectedPiece& selected, int newRow, int newCol)
{
    if (!selected.isSelected)
        return false;

    Piece& piece = board.getBoard()[selected.row][selected.col];

    // Vérifier si c'est bien le tour du joueur
    if ((piece.isWhite && !isWhiteTurn) || (!piece.isWhite && isWhiteTurn))
    {
        return false;
    }

    if (isValidMove(piece, selected.row, selected.col, newRow, newCol, board.getBoard()))
    {
        // Déplacer la pièce
        board.getBoard()[newRow][newCol]             = piece;
        board.getBoard()[selected.row][selected.col] = {"", false}; // Case vidée

        // Désélectionner la pièce après le déplacement
        selected.isSelected = false;

        // Vérifier si un pion atteint la dernière rangée
        checkForPawnPromotion(board, newRow, newCol);

        // Changer de tour
        isWhiteTurn = !isWhiteTurn;

        return true; // Déplacement réussi
    }

    return false; // Déplacement invalide
}

void deselectPiece(SelectedPiece& selected)
{
    selected.isSelected = false;
    selected.row        = -1;
    selected.col        = -1;
}

std::vector<std::pair<int, int>> getValidMovesForSelected(const Board& board, const SelectedPiece& selectedPiece)
{
    if (!selectedPiece.isSelected)
    {
        return {};
    }

    const Piece& piece = board.getBoard()[selectedPiece.row][selectedPiece.col];
    return getValidMoves(piece, selectedPiece.row, selectedPiece.col, board.getBoard());
}