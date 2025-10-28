#include <iostream>

int main() {
    auto lang = "C++";
    enum Piece { EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
    enum Color { WHITE, BLACK, NONE };

    struct Square {
        Piece piece;
        Color color;
    };

    Square board[8][8];

    return 0;
}