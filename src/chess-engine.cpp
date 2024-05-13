#include "chess-engine.hpp"

uint8_t GetCoordinate(uint8_t x, uint8_t y){
    return x + (8 * y);
}

uint8_t GetCoordinate(Coordinate coordinate){
    return GetCoordinate(coordinate.X, coordinate.Y);
}

Piece::Piece(PieceType type){
    this->type = type;
}

bool Piece::IsWhite(){
    return (static_cast<uint8_t>(this->type) & 0b00001000) == 0;
}

vector<Piece::PieceMovementFormula> Piece::GetPieceMovementFormula(){
    vector<Piece::PieceMovementFormula> result;
    switch(type){
        case PieceType::W_KING:
        case PieceType::B_KING:
            result.push_back({-1,1,false});
            result.push_back({0,1,false});
            result.push_back({1,1,false});
            result.push_back({1,0,false});
            result.push_back({1,-1,false});
            result.push_back({0,-1,false});
            result.push_back({-1,-1,false});
            result.push_back({-1, 0, false});
            return result;
        case PieceType::W_BISHOP:
        case PieceType::B_BISHOP:
            result.push_back({-1,1,true});
            result.push_back({1,1,true});
            result.push_back({1,-1,true});
            result.push_back({-1,-1,true});
            return result;
        case PieceType::W_QUEEN:
        case PieceType::B_QUEEN:
            result.push_back({-1,1,true});
            result.push_back({0,1,true});
            result.push_back({1,1,true});
            result.push_back({1,0,true});
            result.push_back({1,-1,true});
            result.push_back({0,-1,true});
            result.push_back({-1,-1,true});
            result.push_back({-1, 0, true});
            return result;
        case PieceType::W_ROOK:
        case PieceType::B_ROOK:
            result.push_back({0,1,true});
            result.push_back({1,0,true});
            result.push_back({0,-1,true});
            result.push_back({-1, 0, true});
            return result;
        case PieceType::W_PAWN:
            result.push_back({0,-1,true,2});
            return result;
        case PieceType::B_PAWN:
            result.push_back({0,1,true,2}); 
            return result;
        case PieceType::W_KNIGHT:
        case PieceType::B_KNIGHT:
            result.push_back({1, 2, false});
            result.push_back({-1, 2, false});
            result.push_back({1, -2, false});
            result.push_back({-1, -2, false});
            result.push_back({2, 1, false});
            result.push_back({2, -1, false});
            result.push_back({-2, 1, false});
            result.push_back({-2, -1, false});
            return result;
        default:
            result.push_back({-1,1,false});
            result.push_back({0,1,false});
            result.push_back({1,1,false});
            result.push_back({1,0,false});
            result.push_back({1,-1,false});
            result.push_back({0,-1,false});
            result.push_back({-1,-1,false});
            result.push_back({-1, 0, false});
            return result;
    }
}

Cell::Cell(){
    this->pieceOnCell = nullptr;
}

void Cell::GetPieceOnCell(Piece*& piecePtr){
    piecePtr = this->pieceOnCell.get();
}

void Cell::GetPieceOnCell(unique_ptr<Piece>& piecePtr){
    piecePtr = std::move(this->pieceOnCell);
}

void Cell::SetPieceOnCell(Piece newPieceOnCell){
    this->pieceOnCell = make_unique<Piece>(newPieceOnCell);
}

void Cell::SetPieceOnCell(unique_ptr<Piece> newPieceOnCell){
    this->pieceOnCell = std::move(newPieceOnCell);
}

Board::Board(){
    for(uint8_t x = 0; x < 8; x++){
        for(uint8_t y = 0; y < 8; y++){
            this->cells[GetCoordinate(x, y)] = make_shared<Cell>();
        }
    }

    this->cells[GetCoordinate(4, 7)]->SetPieceOnCell(make_unique<Piece>(PieceType::W_KING));
    this->cells[GetCoordinate(3, 7)]->SetPieceOnCell(make_unique<Piece>(PieceType::W_QUEEN));

    this->cells[GetCoordinate(4, 0)]->SetPieceOnCell(make_unique<Piece>(PieceType::B_KING));
    this->cells[GetCoordinate(3, 0)]->SetPieceOnCell(make_unique<Piece>(PieceType::B_QUEEN));

    this->cells[GetCoordinate(0, 7)]->SetPieceOnCell(make_unique<Piece>(PieceType::W_ROOK));
    this->cells[GetCoordinate(7, 7)]->SetPieceOnCell(make_unique<Piece>(PieceType::W_ROOK));

    this->cells[GetCoordinate(0, 0)]->SetPieceOnCell(make_unique<Piece>(PieceType::B_ROOK));
    this->cells[GetCoordinate(7, 0)]->SetPieceOnCell(make_unique<Piece>(PieceType::B_ROOK));

    this->cells[GetCoordinate(1, 7)]->SetPieceOnCell(make_unique<Piece>(PieceType::W_KNIGHT));
    this->cells[GetCoordinate(6, 7)]->SetPieceOnCell(make_unique<Piece>(PieceType::W_KNIGHT));

    this->cells[GetCoordinate(1, 0)]->SetPieceOnCell(make_unique<Piece>(PieceType::B_KNIGHT));
    this->cells[GetCoordinate(6, 0)]->SetPieceOnCell(make_unique<Piece>(PieceType::B_KNIGHT));

    this->cells[GetCoordinate(2, 7)]->SetPieceOnCell(make_unique<Piece>(PieceType::W_BISHOP));
    this->cells[GetCoordinate(5, 7)]->SetPieceOnCell(make_unique<Piece>(PieceType::W_BISHOP));

    this->cells[GetCoordinate(2, 0)]->SetPieceOnCell(make_unique<Piece>(PieceType::B_BISHOP));
    this->cells[GetCoordinate(5, 0)]->SetPieceOnCell(make_unique<Piece>(PieceType::B_BISHOP));
    /*
    white_pieces.emplace_back(cast_to_piece(make_shared<King>(King(cells[4][7], Faction::WHITE_FACTION))));
    white_pieces.emplace_back(cast_to_piece(make_shared<Queen>(Queen(cells[3][7], Faction::WHITE_FACTION))));
    white_pieces.emplace_back(cast_to_piece(make_shared<Rook>(Rook(cells[0][7], Faction::WHITE_FACTION))));
    white_pieces.emplace_back(cast_to_piece(make_shared<Rook>(Rook(cells[7][7], Faction::WHITE_FACTION))));
    white_pieces.emplace_back(cast_to_piece(make_shared<Knight>(Knight(cells[1][7], Faction::WHITE_FACTION))));
    white_pieces.emplace_back(cast_to_piece(make_shared<Knight>(Knight(cells[6][7], Faction::WHITE_FACTION))));
    white_pieces.emplace_back(cast_to_piece(make_shared<Bishop>(Bishop(cells[2][7], Faction::WHITE_FACTION))));
    white_pieces.emplace_back(cast_to_piece(make_shared<Bishop>(Bishop(cells[5][7], Faction::WHITE_FACTION))));

    black_pieces.emplace_back(cast_to_piece(make_shared<King>(King(cells[4][0], Faction::BLACK_FACTION))));
    black_pieces.emplace_back(cast_to_piece(make_shared<Queen>(Queen(cells[3][0], Faction::BLACK_FACTION))));
    black_pieces.emplace_back(cast_to_piece(make_shared<Rook>(Rook(cells[0][0], Faction::BLACK_FACTION))));
    black_pieces.emplace_back(cast_to_piece(make_shared<Rook>(Rook(cells[7][0], Faction::BLACK_FACTION))));
    black_pieces.emplace_back(cast_to_piece(make_shared<Knight>(Knight(cells[1][0], Faction::BLACK_FACTION))));
    black_pieces.emplace_back(cast_to_piece(make_shared<Knight>(Knight(cells[6][0], Faction::BLACK_FACTION))));
    black_pieces.emplace_back(cast_to_piece(make_shared<Bishop>(Bishop(cells[2][0], Faction::BLACK_FACTION))));
    black_pieces.emplace_back(cast_to_piece(make_shared<Bishop>(Bishop(cells[5][0], Faction::BLACK_FACTION))));
    */
    for(uint8_t x = 0; x < 8; x++){
        this->cells[GetCoordinate(x, 6)]->SetPieceOnCell(Piece(PieceType::W_PAWN));
        this->cells[GetCoordinate(x, 1)]->SetPieceOnCell(Piece(PieceType::B_PAWN));
    }
}

Cell* Board::GetCell(uint8_t x, uint8_t y){
    return cells[GetCoordinate(x, y)].get();
}

Cell* Board::GetCell(Coordinate coordinate){
    return GetCell(coordinate.X, coordinate.Y);
}

bool Board::IsCellOccupied(Coordinate coordinate){
    Piece* potentialPiece;
    GetCell(coordinate)->GetPieceOnCell(potentialPiece);
    return potentialPiece != nullptr;
}

void FormulaApplier(Board& board, vector<Coordinate>& allPossibleMoves, Coordinate& coordinate, Piece::PieceMovementFormula& formula, bool isWhite, int counter = 0){
    uint8_t X = static_cast<uint8_t>(coordinate.X + formula.x_movement);
    uint8_t Y = static_cast<uint8_t>(coordinate.Y + formula.y_movement);
    if(X < 0 || X > 7 || Y < 0 || Y > 7){
        return;
    }
    if(board.IsCellOccupied(Coordinate{X, Y})){
        Piece* piecePtr;
        board.GetCell(X, Y)->GetPieceOnCell(piecePtr);
        if(piecePtr->IsWhite() == isWhite){
            return;
        }
        Coordinate newCoords = {X, Y};
        allPossibleMoves.push_back(newCoords);
        return;
    }
    Coordinate newCoords = {X, Y};
    allPossibleMoves.push_back(newCoords);
    if(formula.continuous){
        if(formula.max_continuous != 0){
            if(!formula.max_continuous < counter){
                return;
            }
            counter++;
        }
        // recursively apply until we reach the end
        FormulaApplier(board, allPossibleMoves, newCoords, formula, isWhite, counter);
    }
}

vector<Coordinate> Board::GetPossibleMoves(Cell*& cell, Coordinate coordinate){
    vector<Coordinate> allPossibleMoves;
    Piece* piece;
    cell->GetPieceOnCell(piece);
    for(Piece::PieceMovementFormula formula: piece->GetPieceMovementFormula()){
        FormulaApplier(*this, allPossibleMoves, coordinate, formula, piece->IsWhite());
    }
    //allPossibleMoves.push_back({1, 2});
    return allPossibleMoves;
}

void Board::MovePiece(Cell*& fromCell, Cell*& toCell){
    unique_ptr<Piece> targetPiece;
    fromCell->GetPieceOnCell(targetPiece);
    toCell->SetPieceOnCell(std::move(targetPiece));
}