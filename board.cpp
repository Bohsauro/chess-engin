#include "board.h"
#include <iostream>

int NotToInt(char str[3]) {
    int b = int(str[0]) - 96;
    int a = (str[1] - '0') * 7;
    return a+b;
}

Board::Board() {
    reset();
}

void Board::reset() {
    // Pulisci la scacchiera
    for (auto& row : board)
        for (auto& square : row)
            square = { Piece::EMPTY, Color::NONE };

    // Pedoni
    for (int i = 0; i < 8; ++i) {
        board[1][i] = { Piece::PAWN, Color::WHITE };
        board[6][i] = { Piece::PAWN, Color::BLACK };
    }

    // Torri
    board[0][0] = board[0][7] = { Piece::ROOK, Color::WHITE };
    board[7][0] = board[7][7] = { Piece::ROOK, Color::BLACK };

    // Cavalli
    board[0][1] = board[0][6] = { Piece::KNIGHT, Color::WHITE };
    board[7][1] = board[7][6] = { Piece::KNIGHT, Color::BLACK };

    // Alfieri
    board[0][2] = board[0][5] = { Piece::BISHOP, Color::WHITE };
    board[7][2] = board[7][5] = { Piece::BISHOP, Color::BLACK };

    // Regine
    board[0][3] = { Piece::QUEEN, Color::WHITE };
    board[7][3] = { Piece::QUEEN, Color::BLACK };

    // Re
    board[0][4] = { Piece::KING, Color::WHITE };
    board[7][4] = { Piece::KING, Color::BLACK };
}

void Board::print() const {
    for (int r = 7; r >= 0; --r) {
        std::cout << r + 1 << " ";
        for (int f = 0; f < 8; ++f) {
            const Square& sq = board[r][f];
            char c = '.';
            if (sq.piece != Piece::EMPTY) {
                switch (sq.piece) {
                    case Piece::PAWN:   c = 'P'; break;
                    case Piece::KNIGHT: c = 'N'; break;
                    case Piece::BISHOP: c = 'B'; break;
                    case Piece::ROOK:   c = 'R'; break;
                    case Piece::QUEEN:  c = 'Q'; break;
                    case Piece::KING:   c = 'K'; break;
                    default: break;
                }
                if (sq.color == Color::BLACK)
                    c = tolower(c);
            }
            std::cout << c << " ";
        }
        std::cout << '\n';
    }
    std::cout << "  a b c d e f g h\n";
}

std::array<std::array<Square, 8>, 8>& Board::getBoard() {
    return board;
}
