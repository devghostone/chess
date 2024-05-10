#include "chess-engine.hpp"

Piece::Piece(PieceType type){
    this->type = type;
}

bool Piece::IsWhite(){
    return (static_cast<uint8_t>(this->type) & 0b00001000) == 0;
}

Cell::Cell(){
    this->pieceOnCell = nullptr;
}

void Cell::GetPieceOnCell(Piece*& piecePtr){
    piecePtr = this->pieceOnCell.get();
}

void Cell::GetPieceOnCell(unique_ptr<Piece>*& piecePtr){
    piecePtr = &(this->pieceOnCell);
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

uint8_t Board::GetCoordinate(uint8_t x, uint8_t y){
    return x + (8 * y);
}

Cell* Board::GetCell(uint8_t x, uint8_t y){
    return cells[GetCoordinate(x, y)].get();
}