#include "gui.hpp"

GuiChessboard::GuiChessboard(Board &board, GuiPieceRenderer &renderer, Color whiteCell, Color blackCell, Color selectionCell, int screenHeight)
    : whiteCell(whiteCell),
      blackCell(blackCell),
      selectionCell(selectionCell),
      cellSize(screenHeight / 8),
      targetBoard(&board),
      pieceRenderer(&renderer)
{
}

void GuiChessboard::RenderBoard()
{
    bool isWhite = true;
    for (int i = 0; i < 8; i++)
    {
        isWhite = !isWhite;
        for (int j = 0; j < 8; j++)
        {
            DrawRectangle(i * cellSize, j * cellSize, cellSize, cellSize, isWhite ? whiteCell : blackCell);
            isWhite = !isWhite;
        }
    }
    if (isSelected)
    {
        DrawRectangle(selectedCoordinate.X * cellSize, selectedCoordinate.Y * cellSize, cellSize, cellSize, selectionCell);
        if (isCalculated)
        {
            // we will calculate the move set only once
            return;
        }
        // Get all possible moves and draw hints
        if (targetBoard->IsCellOccupied(selectedCoordinate))
        {
            Cell *cell = targetBoard->GetCell(selectedCoordinate);
            Piece *piece = nullptr;
            cell->GetPieceOnCell(piece);
            possibleMoves = targetBoard->GetPossibleMoves(cell, selectedCoordinate);
        }
        else
        {
            possibleMoves = vector<Coordinate>();
        }
        isCalculated = true;
    }
}

void GuiChessboard::RenderPieces()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            Piece *currentPiece;
            targetBoard->cells[GetCoordinate(i, j)]->GetPieceOnCell(*&currentPiece);
            if (currentPiece != nullptr)
            {
                pieceRenderer->RenderPiece(currentPiece, (Rectangle){static_cast<float>(i * cellSize), static_cast<float>(j * cellSize), static_cast<float>(cellSize), static_cast<float>(cellSize)});
            }
        }
    }
}

void GuiChessboard::RenderSelection()
{
    if (isSelected)
    {
        for (Coordinate coords : possibleMoves)
        {
            // DrawRectangle(coords.X * cellSize, coords.Y * cellSize, cellSize, cellSize, YELLOW);
            DrawCircle((coords.X * cellSize) + cellSize / 2, (coords.Y * cellSize) + cellSize / 2, cellSize / 10, YELLOW);
        }
    }
}

void GuiChessboard::HandleClick(uint8_t x, uint8_t y)
{
    if (x > 7 || y > 7 || x < 0 || y < 0)
    {
        // invalid condition
        isSelected = false;
        return;
    }

    if (isSelected && targetBoard->IsCellOccupied(selectedCoordinate))
    {
        auto it = std::find(possibleMoves.begin(), possibleMoves.end(), Coordinate{x, y});
        if (it != possibleMoves.end())
        {
            Cell *fromCell = targetBoard->cells[GetCoordinate(selectedCoordinate)].get();
            Cell *toCell = targetBoard->cells[GetCoordinate(x, y)].get();
            targetBoard->MovePiece(fromCell, toCell);
        }
        isSelected = false;
        return;
    }

    isSelected = true;
    isCalculated = false;
    selectedCoordinate = {x, y};
}

GuiPieceRenderer::GuiPieceRenderer(array<array<Vector2, 6>, 2> imageOffsetVectors, std::string chessPieceSvgPath, int chessPieceImageWidth, int chessPieceImageHeight) : chessPiecesTexture(LoadTextureFromImage(LoadImageSvg(chessPieceSvgPath.c_str(), chessPieceImageWidth, chessPieceImageHeight)))
{
    this->imageOffsetVectors = imageOffsetVectors;
}

void GuiPieceRenderer::RenderPiece(Piece *&piece, Rectangle targetArea)
{
    Vector2 pieceOffset = GetPieceOffset(piece);
    DrawTexturePro(chessPiecesTexture, (Rectangle){pieceOffset.x, pieceOffset.y, (float)chessPiecesTexture.width / 6, (float)chessPiecesTexture.height / 2}, targetArea, (Vector2){0, 0}, 0, WHITE);
}

Vector2 GuiPieceRenderer::GetPieceOffset(Piece *&piece)
{
    int X = piece->IsWhite() ? 0 : 1;
    int Y = static_cast<int>(piece->type) % 8 - 1;
    return imageOffsetVectors[X][Y];
}

GuiInputManager::GuiInputManager(GuiChessboard &board, int boardSize)
{
    guiChessboard = &board;
    this->boardSize = boardSize;
}

void GuiInputManager::Update()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePosition = GetMousePosition();
        int mouseX = mousePosition.x;
        int mouseY = mousePosition.y;
        guiChessboard->HandleClick(mouseX / (boardSize / 8), mouseY / (boardSize / 8));
    }
}