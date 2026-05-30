#pragma once
#include <array>
#include <string>
#include <vector>

enum class Piece {
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

enum class Color {
    NONE,
    WHITE,
    BLACK
};

struct Square {
    Piece piece;
    Color color;
};

struct Move {
    int from;
    int to;
    Piece movedPiece;
    Piece capturedPiece;
    Piece promotionPiece;
    bool isEnPassant;
    bool isCastling;
};

class Board {
public:
    bool whiteTurn;
    int moves;
    int halfMoves;
    int fiftyMoves;
    int enPassant;
    bool whiteLong, whiteShort, blackLong, blackShort;

    Board();
    void reset();  // Inizializza la board alla posizione standard
    void print() const;

    void makeMove(const Move& m);
    // For simple undo, we could store state or just take it back manually.
    // Given the small state, copying might be easier for legal move checking,
    // but let's implement a simple make/unmake or just copy.

    bool isSquareAttacked(int r, int c, Color attackerColor) const;

    std::array<std::array<Square, 8>, 8> board;
};
