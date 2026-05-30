#include <iostream>
#include <vector>
#include <cstdint>
#include "board.h"
#include "move.h"

uint64_t perft(const Board& board, int depth) {
    if (depth == 0) return 1;

    auto moves = generateLegalMoves(board);
    if (depth == 1) return moves.size();

    uint64_t nodes = 0;
    for (const auto& m : moves) {
        Board temp = board;
        temp.makeMove(m);
        nodes += perft(temp, depth - 1);
    }
    return nodes;
}

int main() {
    Board board;
    board.print();

    auto moves = generateLegalMoves(board);
    std::cout << "Legal moves in starting position: " << moves.size() << " (Expected: 20)\n";

    for (int d = 1; d <= 3; d++) {
        std::cout << "Perft(" << d << "): " << perft(board, d) << std::endl;
    }

    return 0;
}
