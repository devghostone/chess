#include "chess-engine.hpp"

uint8_t GetArrayCoordinate(uint8_t X, uint8_t Y){
    return X + (8 * Y);
}

uint8_t GetArrayCoordinate(Coordinate coord){
    return GetArrayCoordinate(coord.X, coord.Y);
}

uint8_t cast_to_uint8_t(Piece piece){
    return static_cast<uint8_t>(piece);
}

uint8_t SetPiece(Piece faction, Piece type){
    return cast_to_uint8_t(faction) | cast_to_uint8_t(type);
}

Piece GetPieceFaction(uint8_t piece){

    Piece value = static_cast<Piece>(piece & 0b00011000);
    return value;
}

bool IsCellEmpty(uint8_t cell){
    return cell == 0;
}

Piece GetPieceType(uint8_t piece){
    return static_cast<Piece>(piece & 0b00000111);
}

void Board::UpdateAttackBoard(){
    for(uint8_t i = 0; i < 8; i++){
        for(uint8_t j = 0; j < 8; j++){
            if(IsCellOccupied(Coordinate{i, j})){
                Board::GeneratePseudoLegalMoves(Coordinate{i, j});
            }
        }
    }
}

void Board::CalculateLegalMovesCount(){
    legalMovesCount = 0;
    for(uint8_t i = 0; i < 8; i++){
        for(uint8_t j = 0; j < 8; j++){
            if(IsCellOccupied(Coordinate{i, j})){
                legalMovesCount += Board::GeneratePseudoLegalMoves(Coordinate{i, j}).size();
            }
        }
    }
}

void Board::clearAttackBoard(){
    attackBoard[0].reset();
    attackBoard[1].reset();
} 

void Board::clearXrayBoard(){
    xrayBoard[0].reset();
    xrayBoard[1].reset();
}

void Board::CheckGameState(){
    // task : get which side this person is on
    if(legalMovesCount == 0){
        // if checked -> checkmate
        // if not checked -> draw
    }else{
        // if checked -> mention check
        // if not -> normal
    }
}

Board::Board(){
    for(int i = 0; i < 64; i++){
        Cells[i] = cast_to_uint8_t(Piece::NO_PIECE);
    }
    FenLoader::Load(Cells, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    attackBoard = array<bitset<64>, 2>();
    attackBoard[0] = bitset<64>(0);
    attackBoard[1] = bitset<64>(0);
    UpdateAttackBoard();
    CalculateLegalMovesCount();
}

void Board::FormulaRecursion(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord, int movementFormulaX, int movementFormulaY){
    Coordinate nextCoordinate = coord;
    nextCoordinate.X += movementFormulaX;
    nextCoordinate.Y += movementFormulaY;

    if(nextCoordinate.X < 0 || nextCoordinate.X > 7 || nextCoordinate.Y < 0 || nextCoordinate.Y > 7){
        return;
    }

    if(IsCellOccupied(nextCoordinate)){
        if(
            GetPieceFaction(Cells[GetArrayCoordinate(nextCoordinate)]) != 
            GetPieceFaction(Cells[GetArrayCoordinate(hostCoord)])
        ){
            pseudoLegalMoveList.push_back(nextCoordinate);
        }
    }else{
        pseudoLegalMoveList.push_back(nextCoordinate);
    }

    if(!IsCellOccupied(nextCoordinate)){
        FormulaRecursion(pseudoLegalMoveList, hostCoord, nextCoordinate, movementFormulaX, movementFormulaY);
    }

    attackBoard[GetPieceFaction(Cells[GetArrayCoordinate(hostCoord)]) != Piece::FWHITE].set(GetArrayCoordinate(nextCoordinate));
}

void Board::DirectMoveFormula(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord, int movementFormulaX, int movementFormulaY){
    Coordinate targetCoord = coord;
    targetCoord.X += movementFormulaX;
    targetCoord.Y += movementFormulaY;

    if(targetCoord.X < 0 || targetCoord.X > 7 || targetCoord.Y < 0 || targetCoord.Y > 7){
        return;
    }
    attackBoard[GetPieceFaction(Cells[GetArrayCoordinate(hostCoord)]) != Piece::FWHITE].set(GetArrayCoordinate(targetCoord));
    if(IsCellOccupied(targetCoord)){
        if(
            GetPieceFaction(Cells[GetArrayCoordinate(targetCoord)]) != 
            GetPieceFaction(Cells[GetArrayCoordinate(hostCoord)])
        ){
            pseudoLegalMoveList.push_back(targetCoord);
        }
    }else{
        pseudoLegalMoveList.push_back(targetCoord);
    }
}
void Board::HorizontalRecursion(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord){
    FormulaRecursion(pseudoLegalMoveList, hostCoord, coord, -1, 0);
    FormulaRecursion(pseudoLegalMoveList, hostCoord, coord, 1, 0);
}
void Board::VerticalRecursion(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord){
    FormulaRecursion(pseudoLegalMoveList, hostCoord, coord, 0, 1);
    FormulaRecursion(pseudoLegalMoveList, hostCoord, coord, 0, -1);
}
void Board::DiagonalRecursion(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord){
    FormulaRecursion(pseudoLegalMoveList, hostCoord, coord, 1, 1);
    FormulaRecursion(pseudoLegalMoveList, hostCoord, coord, -1, -1);
}
void Board::AntiDiagonalRecursion(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord){
    FormulaRecursion(pseudoLegalMoveList, hostCoord, coord, 1, -1);
    FormulaRecursion(pseudoLegalMoveList, hostCoord, coord, -1, 1);
}

void Board::SurroundingMove(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord){
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, 1, -1);
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, 1, 0);
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, 1, 1);
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, 0, 1);
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, -1, 1);
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, -1, 0);
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, -1, -1);
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, 0, -1);
}

void Board::KnightMove(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord){
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, 1, 2);
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, 2, 1);
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, 2, -1);
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, 1, -2);
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, -1, -2);
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, -2, -1);
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, -2, 1);
    DirectMoveFormula(pseudoLegalMoveList, hostCoord, coord, -1, 2);
}

uint8_t Board::FindAPiece(Piece type, Piece faction){
    for(uint8_t x = 0; x < 8; x++){
        for(uint8_t y = 0; y < 8; y++){
            uint8_t current_index = GetArrayCoordinate(Coordinate{x, y});
            if(GetPieceType(Cells[current_index]) == type && GetPieceFaction(Cells[current_index]) == faction)
                return current_index;
        }
    }
    return 64;
}

vector<Coordinate> Board::GeneratePseudoLegalMoves(Coordinate coord){
    
    vector<Coordinate> pseudoLegalMovesList;
    Piece pieceType = GetPieceType(Cells[GetArrayCoordinate(coord)]);
    Piece pieceFaction = GetPieceFaction(Cells[GetArrayCoordinate(coord)]);
    switch(pieceType){
        //depending on the type of cell on the particular coordinate do different action
        case Piece::KING:
            SurroundingMove(pseudoLegalMovesList, coord, coord);
            break;
        case Piece::QUEEN:
            DiagonalRecursion(pseudoLegalMovesList, coord, coord);
            AntiDiagonalRecursion(pseudoLegalMovesList ,coord, coord);
            HorizontalRecursion(pseudoLegalMovesList, coord, coord);
            VerticalRecursion(pseudoLegalMovesList, coord, coord);
            break;
        case Piece::BISHOP:
            DiagonalRecursion(pseudoLegalMovesList, coord, coord);
            AntiDiagonalRecursion(pseudoLegalMovesList, coord, coord);
            break;
        case Piece::KNIGHT:
            KnightMove(pseudoLegalMovesList, coord, coord);
            break;
        case Piece::ROOK:
            HorizontalRecursion(pseudoLegalMovesList, coord, coord);
            VerticalRecursion(pseudoLegalMovesList, coord, coord);
            break;
        case Piece::PAWN:
            if(pieceFaction == Piece::FWHITE){
                bool isFirstMove = coord.Y == 1;
                //check if the coord in front of it is occupied or not
                // since cell is set as a uint8_t anything other than 0 becomes a "piece"
                // first we will check whether it can capture or not
                Coordinate leftCoordinate = Coordinate{static_cast<uint8_t>(coord.X - 1), static_cast<uint8_t>(coord.Y + 1)};
                if(Cells[GetArrayCoordinate(leftCoordinate)] && !(leftCoordinate.X < 0)){
                    Piece p = GetPieceFaction(Cells[GetArrayCoordinate(leftCoordinate)]);
                    if(p != pieceFaction){
                        pseudoLegalMovesList.push_back(leftCoordinate);
                    }
                }

                if(!(leftCoordinate.X < 0)){
                    attackBoard[0].set(GetArrayCoordinate(leftCoordinate));
                }

                Coordinate rightCoordinate = Coordinate{static_cast<uint8_t>(coord.X + 1), static_cast<uint8_t>(coord.Y + 1)};
                if(Cells[GetArrayCoordinate(rightCoordinate)] && !(rightCoordinate.X > 7)){
                    Piece p = GetPieceFaction(Cells[GetArrayCoordinate(rightCoordinate)]);
                    if(p != pieceFaction){
                        pseudoLegalMovesList.push_back(rightCoordinate);
                    }
                }

                if(!(rightCoordinate.X > 7)){
                    attackBoard[0].set(GetArrayCoordinate(rightCoordinate));
                }

                if(Cells[GetArrayCoordinate(coord.X, static_cast<uint8_t>(coord.Y + 1))] != 0){
                    break;
                }
                pseudoLegalMovesList.push_back(Coordinate{coord.X, static_cast<uint8_t>(coord.Y + 1)});

                if(Cells[GetArrayCoordinate(coord.X, coord.Y + 2)] || !isFirstMove){
                    break;
                }
                pseudoLegalMovesList.push_back(Coordinate{coord.X, static_cast<uint8_t>(coord.Y + 2)});

                break;
            }
            if(pieceFaction == Piece::FBLACK){
                bool isFirstMove = coord.Y == 6;
                //check if the coord in front of it is occupied or not
                // since cell is set as a uint8_t anything other than 0 becomes a "piece"
                // first we will check whether it can capture or not
                Coordinate leftCoordinate = Coordinate{static_cast<uint8_t>(coord.X - 1), static_cast<uint8_t>(coord.Y - 1)};
                if(Cells[GetArrayCoordinate(leftCoordinate)] && !(leftCoordinate.X < 0)){
                    Piece p = GetPieceFaction(Cells[GetArrayCoordinate(leftCoordinate)]);
                    if(p != pieceFaction){
                        pseudoLegalMovesList.push_back(leftCoordinate);
                    }
                    //attackBoard[1].set(GetArrayCoordinate(leftCoordinate));
                }
                if(!(leftCoordinate.X < 0)){
                    attackBoard[1].set(GetArrayCoordinate(leftCoordinate));
                }

                Coordinate rightCoordinate = Coordinate{static_cast<uint8_t>(coord.X + 1), static_cast<uint8_t>(coord.Y - 1)};
                if(Cells[GetArrayCoordinate(rightCoordinate)] && !(rightCoordinate.X > 7)){
                    Piece p = GetPieceFaction(Cells[GetArrayCoordinate(rightCoordinate)]);
                    if(p != pieceFaction){
                        pseudoLegalMovesList.push_back(rightCoordinate);
                    }
                }
                if(!(rightCoordinate.X > 7)){
                    attackBoard[1].set(GetArrayCoordinate(rightCoordinate));
                }

                if(Cells[GetArrayCoordinate(coord.X, static_cast<uint8_t>(coord.Y - 1))] != 0){
                    break;
                }
                pseudoLegalMovesList.push_back(Coordinate{coord.X, static_cast<uint8_t>(coord.Y - 1)});

                if(Cells[GetArrayCoordinate(coord.X, coord.Y - 2)] || !isFirstMove){
                    break;
                }
                pseudoLegalMovesList.push_back(Coordinate{coord.X, static_cast<uint8_t>(coord.Y - 2)});

                break;
            }
    }
    return pseudoLegalMovesList;
}

// foreach pseudo move we will check whether if king would be checked after the move is made
// if yes, the move is not allowed
// if no, the move is allowed
vector<Coordinate> Board::GenerateLegalMoves(Coordinate coord){
    array<uint8_t, 64> originalCells = Cells;
    array<bitset<64>, 2> originalAttackBoard;
    originalAttackBoard[0] = attackBoard[0];
    originalAttackBoard[1] = attackBoard[1]; 
    Piece pieceFaction = GetPieceFaction(Cells[GetArrayCoordinate(coord)]);
    std::vector<Coordinate> PseudoLegalCoordinates = GeneratePseudoLegalMoves(coord);
    std::vector<Coordinate> LegalCoordinates;
    for(int i = 0; i < PseudoLegalCoordinates.size(); i++){
        
        MovePiece(coord, PseudoLegalCoordinates[i]);
        uint8_t kingIndex = FindAPiece(Piece::KING, pieceFaction);
        if(!attackBoard[pieceFaction == Piece::FWHITE? 1: 0].test(kingIndex)){
            LegalCoordinates.push_back(PseudoLegalCoordinates[i]);
        }
        attackBoard = originalAttackBoard;
        Cells = originalCells;
    }
    return LegalCoordinates;
}

void FenLoader::Load(array<uint8_t, 64>& cells, string config){
    std::map<char, uint8_t> pieceTypeFromSymbol = {
        {'k', cast_to_uint8_t(Piece::KING)},
        {'q', cast_to_uint8_t(Piece::QUEEN)},
        {'b', cast_to_uint8_t(Piece::BISHOP)},
        {'n', cast_to_uint8_t(Piece::KNIGHT)},
        {'r', cast_to_uint8_t(Piece::ROOK)},
        {'p', cast_to_uint8_t(Piece::PAWN)}
    };

    int file = 0, rank = 7;
    for(char symbol: config){
        if(symbol == '/'){
            file = 0;
            rank --;
        }else{
            if(isdigit(symbol)){
                file += symbol - '0';
            }else{
                uint8_t pieceColour = static_cast<uint8_t>(isupper(symbol)? Piece::FWHITE : Piece::FBLACK);
                uint8_t pieceType = pieceTypeFromSymbol[tolower(symbol)];
                cells[rank * 8 + file] = pieceType | pieceColour;
                file++;
            }
        }
    }
}

bool Board::IsCellOccupied(Coordinate coord){
    return Cells[GetArrayCoordinate(coord)] != 0; //something is there
}

void Board::MakeMove(Coordinate fromCoord, Coordinate toCoord){
    Board::MovePiece(fromCoord, toCoord);
    Board::CalculateLegalMovesCount();
    CheckGameState();
}

void Board::MovePiece(Coordinate fromCoord, Coordinate toCoord){
    //vector<Coordinate> allLegalMoves = GenerateLegalMoves(fromCoord);
    //auto it = std::find(allLegalMoves.begin(), allLegalMoves.end(), toCoord);
    //if(it != allLegalMoves.end()){
        // valid move
        clearAttackBoard();
        Cells[GetArrayCoordinate(toCoord)] = Cells[GetArrayCoordinate(fromCoord)];
        Cells[GetArrayCoordinate(fromCoord)] = 0;
        UpdateAttackBoard();
    //}
}