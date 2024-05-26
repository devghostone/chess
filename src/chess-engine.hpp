#pragma once

#include <string>
#include <memory>
#include <array>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <map>
#include <cctype>
#include <bitset>

using std::shared_ptr, std::make_shared, std::unique_ptr, std::make_unique, std::array, std::vector, std::uint8_t, std::string, std::bitset;

/// @brief all the different possible piece type
enum class Piece: uint8_t{
    NO_PIECE = 0,
    KING = 1,
    QUEEN = 2,
    BISHOP = 3,
    KNIGHT = 4,
    ROOK = 5,
    PAWN = 6,
    FWHITE = 8,
    FBLACK = 16
};

Piece GetPieceFaction(uint8_t piece);
Piece GetPieceType(uint8_t piece);

struct Coordinate{
    uint8_t X: 4;
    uint8_t Y: 4;
    bool operator==(const Coordinate& other){
        return (X == other.X) && (Y == other.Y);
    }
    Coordinate operator+(const Coordinate& other){
        return Coordinate{static_cast<uint8_t>(X + other.X), static_cast<uint8_t>(Y + other.Y)};
    }

    Coordinate operator-(const Coordinate& other){
        return Coordinate{static_cast<uint8_t>(X - other.X), static_cast<uint8_t>(Y - other.Y)};
    }
};

uint8_t GetArrayCoordinate(Coordinate coord);
uint8_t GetArrayCoordinate(uint8_t X, uint8_t Y);

void GenerateMoves(Piece piece, Coordinate coord);
void GenerateLegalMoves(Piece piece, Coordinate coord);

class Board{

    int legalMovesCount = 0;

    void CalculateLegalMovesCount();
    void UpdateAttackBoard();
    void CheckGameState();

    void clearAttackBoard();
    void clearXrayBoard();
    
    void FormulaRecursion(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord, int movementFormulaX, int movementFormulaY);
     // move directly to a cell
    void DirectMoveFormula(vector<Coordinate>& pseudoLegalMovelist, Coordinate hostCoord, Coordinate coord, int movementFormulaX, int movementFormulaY);
    
    void HorizontalRecursion(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord);
    void VerticalRecursion(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord);
    void DiagonalRecursion(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord);
    void AntiDiagonalRecursion(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord);
    void SurroundingMove(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord);
    void KnightMove(vector<Coordinate>& pseudoLegalMoveList, Coordinate hostCoord, Coordinate coord);

    uint8_t FindAPiece(Piece type, Piece faction);

    public:
        array<uint8_t, 64> Cells;
        array<bitset<64>, 2> attackBoard; // records which cells are being attacked or in sight of a piece
        array<bitset<64>, 2> xrayBoard; // records which cells are being x-rayed in relation to the king
        
        Board();
        vector<Coordinate> GeneratePseudoLegalMoves(Coordinate coord);
        vector<Coordinate> GenerateLegalMoves(Coordinate coord);

        /// @brief check whether if a cell is occupied or not
        /// @param coord 
        /// @return 
        bool IsCellOccupied(Coordinate coord);
        
        void MovePiece(Coordinate fromCoord, Coordinate toCoord);
        void MakeMove(Coordinate fromCoord, Coordinate toCoord);
};

class FenLoader{
    public:
        static void Load(array<uint8_t, 64>& cells, string config);
};
