#include "search.h"
#include <limits>
#include <algorithm>

int evaluate(const Board& board) {
    int score = 0;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            const Square& sq = board.board[r][c];
            int val = 0;
            switch (sq.piece) {
                case Piece::PAWN:   val = 100; break;
                case Piece::KNIGHT: val = 320; break;
                case Piece::BISHOP: val = 330; break;
                case Piece::ROOK:   val = 500; break;
                case Piece::QUEEN:  val = 900; break;
                case Piece::KING:   val = 20000; break;
                default: break;
            }
            if (sq.color == Color::BLACK) score -= val;
            else if (sq.color == Color::WHITE) score += val;
        }
    }
    return score;
}

uint64_t nodesSearched = 0;

int minimax(Board& board, int depth, bool maximizing) {
    nodesSearched++;
    auto moves = generateLegalMoves(board);

    if (moves.empty()) {
        Color us = board.whiteTurn ? Color::WHITE : Color::BLACK;
        Color them = board.whiteTurn ? Color::BLACK : Color::WHITE;
        // Find king
        int kr = -1, kc = -1;
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (board.board[r][c].piece == Piece::KING && board.board[r][c].color == us) {
                    kr = r; kc = c; break;
                }
            }
            if (kr != -1) break;
        }
        if (board.isSquareAttacked(kr, kc, them)) {
            // Checkmate
            return maximizing ? -100000 - depth : 100000 + depth;
        } else {
            // Stalemate
            return 0;
        }
    }

    if (depth == 0) return evaluate(board);

    if (maximizing) {
        int maxEval = std::numeric_limits<int>::min();
        for (const auto& m : moves) {
            Board next = board;
            next.makeMove(m);
            int eval = minimax(next, depth - 1, false);
            maxEval = std::max(maxEval, eval);
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const auto& m : moves) {
            Board next = board;
            next.makeMove(m);
            int eval = minimax(next, depth - 1, true);
            minEval = std::min(minEval, eval);
        }
        return minEval;
    }
}

SearchResult findBestMove(Board& board, int depth) {
    nodesSearched = 0;
    auto moves = generateLegalMoves(board);
    if (moves.empty()) return {{}, 0, 0};

    Move bestMove = moves[0];
    int bestScore = board.whiteTurn ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

    for (const auto& m : moves) {
        Board next = board;
        next.makeMove(m);
        int score = minimax(next, depth - 1, !board.whiteTurn);
        if (board.whiteTurn) {
            if (score > bestScore) {
                bestScore = score;
                bestMove = m;
            }
        } else {
            if (score < bestScore) {
                bestScore = score;
                bestMove = m;
            }
        }
    }

    return {bestMove, bestScore, nodesSearched};
}
