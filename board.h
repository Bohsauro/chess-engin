#pragma once
#include <array>
#include <string>

int NotToInt(char str[3]);

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

private:
    std::array<std::array<Square, 8>, 8> board;
};
