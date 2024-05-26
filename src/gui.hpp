#pragma once

#include <config.h>
#include "chess-engine.hpp"

#include "raylib.h"

#include <string>
#include <iostream>

using std::string;

class GuiPieceRenderer;
class GuiChessboard;
class GuiInputManager;

class GuiChessboard{

    Board* targetBoard;
    GuiPieceRenderer* pieceRenderer;

    Color whiteCell;
    Color blackCell;
    Color selectionCell;
    int cellSize;

    bool isSelected = false;
    bool isCalculated = false;

    Coordinate selectedCoordinate = {0, 0};
    vector<Coordinate> possibleMoves;

    public:
        GuiChessboard(Board& board, GuiPieceRenderer& renderer, Color whiteCell, Color blackCell, Color selectionCell, int screenHeight);
        void RenderBoard();
        void RenderPieces();
        void RenderSelection();
        void HandleClick(uint8_t x, uint8_t y);
};

class GuiPieceRenderer{

    Texture2D chessPiecesTexture;
    array<array<Vector2,6>,2> imageOffsetVectors; 

    public:
        GuiPieceRenderer(array<array<Vector2,6>,2> imageOffsetVectors, std::string chessPieceSvgPath, int chessPieceImageWidth, int chessPieceImageHeight);
        void RenderPiece(Piece piece, Rectangle targetArea);
        Vector2 GetPieceOffset(Piece& piece);
};

class GuiInputManager{
    GuiChessboard* guiChessboard;
    int boardSize;
    public:
        GuiInputManager(GuiChessboard& board, int boardSize);
        void Update();
};