#pragma once

#include "chess-engine.hpp"

#include "raylib.h"

#include <string>

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

    public:
        GuiChessboard(Board& board, GuiPieceRenderer& renderer, Color whiteCell, Color blackCell, Color selectionCell, int screenHeight);
        void RenderBoard();
        void RenderPieces();
};

class GuiPieceRenderer{

    Texture2D chessPiecesTexture;
    array<array<Vector2,6>,2> imageOffsetVectors; 

    public:
        GuiPieceRenderer(array<array<Vector2,6>,2> imageOffsetVectors, std::string chessPieceSvgPath, int chessPieceImageWidth, int chessPieceImageHeight);
        void RenderPiece(Piece*& piece, Rectangle targetArea);
        Vector2 GetPieceOffset(Piece*& piece);
};

class GuiInputManager{
    public:
        GuiInputManager();
        void ClickOnCell();
};