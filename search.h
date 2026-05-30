#pragma once
#include "board.h"
#include "move.h"
#include <cstdint>

struct SearchResult {
    Move bestMove;
    int score;
    uint64_t nodes;
};

SearchResult findBestMove(Board& board, int depth);
int evaluate(const Board& board);
