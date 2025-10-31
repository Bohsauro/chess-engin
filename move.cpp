#include <vector>
#include "move.h"

#include <algorithm>

#include "board.h"

// Converte un indice (0–63) in una stringa tipo "e2"
std::string squareToString(int index) {
    if (index < 0 || index > 63)
        return "??";
    char file = 'a' + (index % 8);
    char rank = '1' + (index / 8);
    return std::string{file, rank};
}

// Converte una stringa tipo "e2" in un indice (0–63)
int stringToSquare(const std::string& s) {
    if (s.size() != 2)
        return -1;
    char file = s[0];
    char rank = s[1];
    if (file < 'a' || file > 'h' || rank < '1' || rank > '8')
        return -1;
    int col = file - 'a';
    int row = rank - '1';
    return row * 8 + col;
}

std::vector<Move> generateLegalMoves(const Board& board) {
    std::vector<Move> legalMoves;
    auto& b = board.board;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece pezzo = b[r][c].piece;
            int position = r * 8 + c;
            switch(pezzo) {
                case Piece::PAWN:
                    int pawnMove;
                    if (board.whiteTurn) {
                        pawnMove = 8;
                        if (b[r][c+1].pezzo != NULL) {

                        }
                    } else {
                        pawnMove = -8;
                    }
                    break;
                case Piece::KNIGHT: break;
                case Piece::BISHOP: break;
                case Piece::ROOK: break;
                case Piece::QUEEN: break;
                case Piece::KING: break;
                default: break;
            }
        }
    }
    return legalMoves;
}