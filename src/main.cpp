#define SUPPORT_FILEFORMAT_SVG

#include "config.h"
#include "chess-engine.hpp"
#include "gui.hpp"

#include "raylib.h"

#include <iostream>
#include <array>

using std::cout, std::endl, std::array;

array<Vector2, 6> GetWhitePieces(){
    array<Vector2, 6> whitePieces;
    for(int i = 0; i < 6; i++){
        whitePieces[i].x = SVG_RES_X/6 * i;
        whitePieces[i].y = 0;
    }
    return whitePieces;
}

array<Vector2, 6> GetBlackPieces(){
    array<Vector2, 6> blackPieces;
    for(int i = 0; i < 6; i++){
        blackPieces[i].x = SVG_RES_X/6 * i;
        blackPieces[i].y = SVG_RES_Y/2;
    }
    return blackPieces;
}

int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME);
    Board board = Board();
    GuiPieceRenderer renderer = GuiPieceRenderer({GetWhitePieces(), GetBlackPieces()}, CHESS_PIECE_SVG, SVG_RES_X, SVG_RES_Y);
    GuiChessboard boardGui = GuiChessboard(board, renderer, WHITE_SPACE_COLOR, BLACK_SPACE_COLOR, SELECTION_SPACE_COLOR, SCREEN_HEIGHT);
    GuiInputManager inputManager = GuiInputManager(boardGui, SCREEN_HEIGHT);

    while(!WindowShouldClose()){
        inputManager.Update();
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        boardGui.RenderBoard();
        boardGui.RenderPieces();
        boardGui.RenderSelection();

        EndDrawing();
    }
    CloseWindow();
}