#include <iostream>
#include <vector>
#include <string>
#include "board.h"
#include "move.h"
#include "search.h"

int main() {
    Board board;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "uci") {
            std::cout << "id name Sauro" << std::endl;
            std::cout << "id author Jules" << std::endl;
            std::cout << "uciok" << std::endl;
        } else if (line == "isready") {
            std::cout << "readyok" << std::endl;
        } else if (line.substr(0, 8) == "position") {
            if (line.find("startpos") != std::string::npos) {
                board.reset();
                size_t movesIdx = line.find("moves");
                if (movesIdx != std::string::npos) {
                    std::string movesPart = line.substr(movesIdx + 6);
                    size_t start = 0, end;
                    while ((end = movesPart.find(' ', start)) != std::string::npos) {
                        std::string moveStr = movesPart.substr(start, end - start);
                        auto legals = generateLegalMoves(board);
                        for (auto& m : legals) {
                            std::string mStr = squareToString(m.from) + squareToString(m.to);
                            if (m.promotionPiece != Piece::EMPTY) {
                                switch(m.promotionPiece) {
                                    case Piece::QUEEN: mStr += "q"; break;
                                    case Piece::ROOK: mStr += "r"; break;
                                    case Piece::BISHOP: mStr += "b"; break;
                                    case Piece::KNIGHT: mStr += "n"; break;
                                    default: break;
                                }
                            }
                            if (mStr == moveStr) {
                                board.makeMove(m);
                                break;
                            }
                        }
                        start = end + 1;
                    }
                    std::string lastMove = movesPart.substr(start);
                    if (!lastMove.empty()) {
                        auto legals = generateLegalMoves(board);
                        for (auto& m : legals) {
                            std::string mStr = squareToString(m.from) + squareToString(m.to);
                            if (m.promotionPiece != Piece::EMPTY) {
                                switch(m.promotionPiece) {
                                    case Piece::QUEEN: mStr += "q"; break;
                                    case Piece::ROOK: mStr += "r"; break;
                                    case Piece::BISHOP: mStr += "b"; break;
                                    case Piece::KNIGHT: mStr += "n"; break;
                                    default: break;
                                }
                            }
                            if (mStr == lastMove) {
                                board.makeMove(m);
                                break;
                            }
                        }
                    }
                }
            }
        } else if (line.substr(0, 2) == "go") {
            SearchResult res = findBestMove(board, 3);
            std::cout << "info depth 3 score cp " << res.score << " nodes " << res.nodes << std::endl;
            std::string moveStr = squareToString(res.bestMove.from) + squareToString(res.bestMove.to);
            if (res.bestMove.promotionPiece != Piece::EMPTY) {
                switch(res.bestMove.promotionPiece) {
                    case Piece::QUEEN: moveStr += "q"; break;
                    case Piece::ROOK: moveStr += "r"; break;
                    case Piece::BISHOP: moveStr += "b"; break;
                    case Piece::KNIGHT: moveStr += "n"; break;
                    default: break;
                }
            }
            std::cout << "bestmove " << moveStr << std::endl;
        } else if (line == "quit") {
            break;
        }
    }
    return 0;
}
