#include "gui.hpp"

GuiChessboard::GuiChessboard(Board& board, GuiPieceRenderer& renderer, Color whiteCell, Color blackCell, Color selectionCell, int screenHeight)
    : whiteCell(whiteCell),
      blackCell(blackCell),
      selectionCell(selectionCell),
      cellSize(screenHeight / 8),
      targetBoard(&board),
      pieceRenderer(&renderer)
{
}

void GuiChessboard::RenderBoard(){
    bool isWhite = true;
    for(int i = 0; i < 8; i++){
        isWhite = !isWhite;
        for(int j = 0; j < 8; j++){
            DrawRectangle(i * cellSize, j * cellSize, cellSize, cellSize, isWhite? whiteCell: blackCell);
            isWhite = !isWhite;
        }
    }
}

void GuiChessboard::RenderPieces(){
    for(uint8_t i = 0; i < 8; i++){
        for(uint8_t j = 0; j < 8; j++){
            Piece* currentPiece;
            targetBoard->cells[Board::GetCoordinate(i, j)]->GetPieceOnCell(*&currentPiece);
            if(currentPiece != nullptr){
                pieceRenderer->RenderPiece(currentPiece, (Rectangle){static_cast<float>(i * cellSize), static_cast<float>(j * cellSize), static_cast<float>(cellSize), static_cast<float>(cellSize)});
            }
        }
    }
}

GuiPieceRenderer::GuiPieceRenderer(array<array<Vector2,6>,2> imageOffsetVectors, std::string chessPieceSvgPath, int chessPieceImageWidth, int chessPieceImageHeight): chessPiecesTexture(LoadTextureFromImage(LoadImageSvg(chessPieceSvgPath.c_str(), chessPieceImageWidth, chessPieceImageHeight))){
    this->imageOffsetVectors = imageOffsetVectors;
}

void GuiPieceRenderer::RenderPiece(Piece*& piece, Rectangle targetArea){
    Vector2 pieceOffset = GetPieceOffset(piece);
    DrawTexturePro(chessPiecesTexture, (Rectangle){pieceOffset.x, pieceOffset.y, (float)chessPiecesTexture.width/6, (float)chessPiecesTexture.height/2}, targetArea, (Vector2){0, 0}, 0, WHITE);
}

Vector2 GuiPieceRenderer::GetPieceOffset(Piece*& piece){
    int X = piece->IsWhite()? 0: 1;
    int Y = static_cast<int>(piece->type) % 8 - 1;
    return imageOffsetVectors[X][Y];
}