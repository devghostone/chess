#pragma once

#include <memory>
#include <array>
#include <vector>
#include <cstdint>
#include <algorithm>

using std::shared_ptr, std::make_shared, std::unique_ptr, std::make_unique, std::array, std::vector, std::uint8_t;

/// @brief all the different possible piece type
enum class PieceType: uint8_t{
    NO_PIECE,
    W_KING = 1, W_QUEEN, W_BISHOP, W_KNIGHT, W_ROOK, W_PAWN,
    B_KING = 9, B_QUEEN, B_BISHOP, B_KNIGHT, B_ROOK, B_PAWN
};

class Piece;
class Cell;
class Board;

struct Coordinate{
    uint8_t X: 4;
    uint8_t Y: 4;

    bool operator==(const Coordinate& other) const {
        return (X == other.X) && (Y == other.Y);
    }
};

uint8_t GetCoordinate(uint8_t x, uint8_t y);
uint8_t GetCoordinate(Coordinate coordinate);

/// @brief the piece class emcompassing all the methods and functionalities
class Piece{
    public:
        struct PieceMovementFormula{
            int8_t x_movement: 3;
            int8_t y_movement: 3;
            bool continuous;
            uint8_t max_continuous: 3;
        };

        PieceType type;
        array<vector<Piece::PieceMovementFormula>,7> result;

        Piece(PieceType type);

        ///@brief check whether if a piece is white or black
        ///@return true -> White Piece, false -> Black Piece
        bool IsWhite();

        vector<PieceMovementFormula> GetPieceMovementFormula();

};

/// @brief the cell class that is going to house piece and be inside a board
class Cell{
    unique_ptr<Piece> pieceOnCell;

    public:
        Cell();

        /// @brief get the current piece on the cell
        /// @return pointer to the current piece
        void GetPieceOnCell(Piece*& piecePtr);
        
        /// @brief get the unique ptr to be moved
        /// @return 
        void GetPieceOnCell(unique_ptr<Piece>& piecePtr);

        /// @brief new piece
        /// @param newPieceOnCell 
        void SetPieceOnCell(Piece newPieceOnCell);

        /// @brief release the previous reference to the piece on this cell and replace it with a new one
        /// @param pieceOnCell current piece on this cell
        void SetPieceOnCell(unique_ptr<Piece> newPieceOnCell);
};

class Board{
    public:

        array<shared_ptr<Cell>, 64> cells;

        Board();

        Cell* GetCell(uint8_t x, uint8_t y);
        Cell* GetCell(Coordinate coordinate);

        bool IsCellOccupied(Coordinate coordinate);

        vector<Coordinate> GetPossibleMoves(Cell*& cell, Coordinate coordinate);

        void MovePiece(Cell*& fromCell, Cell*& toCell);
};
