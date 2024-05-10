#pragma once

#include <memory>
#include <array>
#include <vector>
#include <cstdint>

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

/// @brief the piece class emcompassing all the methods and functionalities
class Piece{
    public:
        PieceType type;
        
        Piece(PieceType type);

        ///@brief check whether if a piece is white or black
        ///@return true -> White Piece, false -> Black Piece
        bool IsWhite();
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
        void GetPieceOnCell(unique_ptr<Piece>*& piecePtr);

        /// @brief new piece
        /// @param newPieceOnCell 
        void SetPieceOnCell(Piece newPieceOnCell);

        /// @brief release the previous reference to the piece on this cell and replace it with a new one
        /// @param pieceOnCell current piece on this cell
        void SetPieceOnCell(unique_ptr<Piece> newPieceOnCell);
};

class Board{
    public:
        struct Coordinate{
            uint8_t X: 4;
            uint8_t Y: 4;
        };

        array<shared_ptr<Cell>, 64> cells;

        Board();

        Cell* GetCell(uint8_t x, uint8_t y);

        static uint8_t GetCoordinate(uint8_t x, uint8_t y);
};
