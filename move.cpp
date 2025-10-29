#include <vector>
#include "move.h"
#include "board.h"

std::vector<Move> generateLegalMoves(const Board& board) {
    std::vector<Move> legalMoves;
    const auto& b = board.getBoard();
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece pezzo = b[r][c];

            switch(pezzo) {
                case Piece::PAWN:
                    int pawnMove;
                    if (board.whiteTurn) {
                        pawnMove = 8;
                    } else {
                        pawnMove = -8;
                    }
                case Piece::KNIGHT:
                case Piece::BISHOP:
                case Piece::ROOK:
                case Piece::QUEEN:
                case Piece::KING:
              }

        }
    }



}