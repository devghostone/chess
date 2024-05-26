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
    for (uint8_t file = 0; file < 8; file++)
    {
        for (uint8_t rank = 0; rank < 8; rank++)
        {
            DrawRectangle(file * cellSize, (7 - rank) * cellSize, cellSize, cellSize, (file + rank) % 2 == 0 ? whiteCell : blackCell);
        }
    }
    if (isSelected)
    {
        DrawRectangle(selectedCoordinate.X * cellSize, (7 - selectedCoordinate.Y) * cellSize, cellSize, cellSize, selectionCell);
        if (isCalculated)
        {
            // we will calculate the move set only once
            return;
        }
        // Get all possible moves and draw hints
        if (targetBoard->IsCellOccupied(selectedCoordinate))
        {
            //Cell *cell = targetBoard->GetCell(selectedCoordinate);
            //Piece *piece = nullptr;
            //cell->GetPieceOnCell(piece);
            possibleMoves = targetBoard->GenerateLegalMoves(selectedCoordinate);
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
            pieceRenderer->RenderPiece(static_cast<Piece>(targetBoard->Cells[GetArrayCoordinate(i, j)]), (Rectangle){static_cast<float>(i * cellSize), static_cast<float>((7 - j) * cellSize), static_cast<float>(cellSize), static_cast<float>(cellSize)});
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
            DrawCircle((coords.X * cellSize) + cellSize / 2, ((7 - coords.Y) * cellSize) + cellSize / 2, cellSize / 10, YELLOW);
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
            targetBoard->MakeMove(selectedCoordinate, Coordinate{x, y});
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

void GuiPieceRenderer::RenderPiece(Piece piece, Rectangle targetArea)
{
    if(piece != Piece::NO_PIECE){
        Vector2 pieceOffset = GetPieceOffset(piece);
        DrawTexturePro(chessPiecesTexture, (Rectangle){pieceOffset.x, pieceOffset.y, (float)chessPiecesTexture.width / 6, (float)chessPiecesTexture.height / 2}, targetArea, (Vector2){0, 0}, 0, WHITE);
    }
}

Vector2 GuiPieceRenderer::GetPieceOffset(Piece& piece)
{   
    int X = GetPieceFaction(static_cast<uint8_t>(piece)) == Piece::FWHITE ? 0 : 1;
    int Y = static_cast<uint8_t>(GetPieceType(static_cast<uint8_t>(piece))) - 1;
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
        guiChessboard->HandleClick(mouseX / (boardSize / 8), 7 - mouseY / (boardSize / 8));
    }
}