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

bool isInside(int r, int c) {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

void addPawnMove(std::vector<Move>& moves, int from, int to, Piece moved, Piece captured, Piece promotion = Piece::EMPTY, bool isEP = false) {
    Move m;
    m.from = from;
    m.to = to;
    m.movedPiece = moved;
    m.capturedPiece = captured;
    m.promotionPiece = promotion;
    m.isEnPassant = isEP;
    m.isCastling = false;

    if (promotion != Piece::EMPTY) {
        moves.push_back(m);
    } else if ((moved == Piece::PAWN && to / 8 == 7) || (moved == Piece::PAWN && to / 8 == 0)) {
        // Promotion
        Piece promos[] = {Piece::QUEEN, Piece::ROOK, Piece::BISHOP, Piece::KNIGHT};
        for (auto p : promos) {
            m.promotionPiece = p;
            moves.push_back(m);
        }
    } else {
        moves.push_back(m);
    }
}

std::vector<Move> generatePseudoLegalMoves(const Board& board) {
    std::vector<Move> pseudoMoves;
    Color us = board.whiteTurn ? Color::WHITE : Color::BLACK;
    Color them = board.whiteTurn ? Color::BLACK : Color::WHITE;
    int pawnDir = board.whiteTurn ? 1 : -1;
    int startRank = board.whiteTurn ? 1 : 6;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board.board[r][c].color != us) continue;

            int from = r * 8 + c;
            Piece piece = board.board[r][c].piece;

            if (piece == Piece::PAWN) {
                // Forward
                int nr = r + pawnDir;
                if (isInside(nr, c) && board.board[nr][c].piece == Piece::EMPTY) {
                    addPawnMove(pseudoMoves, from, nr * 8 + c, piece, Piece::EMPTY);
                    // Double push
                    if (r == startRank) {
                        int nnr = nr + pawnDir;
                        if (isInside(nnr, c) && board.board[nnr][c].piece == Piece::EMPTY) {
                            addPawnMove(pseudoMoves, from, nnr * 8 + c, piece, Piece::EMPTY);
                        }
                    }
                }
                // Captures
                for (int dc : {-1, 1}) {
                    int nc = c + dc;
                    if (isInside(nr, nc)) {
                        if (board.board[nr][nc].color == them) {
                            addPawnMove(pseudoMoves, from, nr * 8 + nc, piece, board.board[nr][nc].piece);
                        } else if (nr * 8 + nc == board.enPassant) {
                            addPawnMove(pseudoMoves, from, nr * 8 + nc, piece, Piece::PAWN, Piece::EMPTY, true);
                        }
                    }
                }
            } else if (piece == Piece::KNIGHT) {
                int dr[] = {1, 2, 2, 1, -1, -2, -2, -1};
                int dc[] = {2, 1, -1, -2, -2, -1, 1, 2};
                for (int i = 0; i < 8; i++) {
                    int nr = r + dr[i], nc = c + dc[i];
                    if (isInside(nr, nc) && board.board[nr][nc].color != us) {
                        Move m = {from, nr * 8 + nc, piece, board.board[nr][nc].piece, Piece::EMPTY, false, false};
                        pseudoMoves.push_back(m);
                    }
                }
            } else if (piece == Piece::BISHOP || piece == Piece::ROOK || piece == Piece::QUEEN) {
                int dr[] = {1, -1, 0, 0, 1, 1, -1, -1};
                int dc[] = {0, 0, 1, -1, 1, -1, 1, -1};
                int start = (piece == Piece::BISHOP) ? 4 : 0;
                int end = (piece == Piece::ROOK) ? 4 : 8;
                for (int i = start; i < end; i++) {
                    for (int dist = 1; dist < 8; dist++) {
                        int nr = r + dr[i] * dist, nc = c + dc[i] * dist;
                        if (!isInside(nr, nc)) break;
                        if (board.board[nr][nc].color == us) break;
                        Move m = {from, nr * 8 + nc, piece, board.board[nr][nc].piece, Piece::EMPTY, false, false};
                        pseudoMoves.push_back(m);
                        if (board.board[nr][nc].color == them) break;
                    }
                }
            } else if (piece == Piece::KING) {
                for (int dr = -1; dr <= 1; dr++) {
                    for (int dc = -1; dc <= 1; dc++) {
                        if (dr == 0 && dc == 0) continue;
                        int nr = r + dr, nc = c + dc;
                        if (isInside(nr, nc) && board.board[nr][nc].color != us) {
                            Move m = {from, nr * 8 + nc, piece, board.board[nr][nc].piece, Piece::EMPTY, false, false};
                            pseudoMoves.push_back(m);
                        }
                    }
                }
                // Castling
                if (board.whiteTurn) {
                    if (board.whiteShort && board.board[0][5].piece == Piece::EMPTY && board.board[0][6].piece == Piece::EMPTY) {
                        if (!board.isSquareAttacked(0, 4, Color::BLACK) && !board.isSquareAttacked(0, 5, Color::BLACK)) {
                             pseudoMoves.push_back({from, 0 * 8 + 6, piece, Piece::EMPTY, Piece::EMPTY, false, true});
                        }
                    }
                    if (board.whiteLong && board.board[0][3].piece == Piece::EMPTY && board.board[0][2].piece == Piece::EMPTY && board.board[0][1].piece == Piece::EMPTY) {
                        if (!board.isSquareAttacked(0, 4, Color::BLACK) && !board.isSquareAttacked(0, 3, Color::BLACK)) {
                             pseudoMoves.push_back({from, 0 * 8 + 2, piece, Piece::EMPTY, Piece::EMPTY, false, true});
                        }
                    }
                } else {
                    if (board.blackShort && board.board[7][5].piece == Piece::EMPTY && board.board[7][6].piece == Piece::EMPTY) {
                        if (!board.isSquareAttacked(7, 4, Color::WHITE) && !board.isSquareAttacked(7, 5, Color::WHITE)) {
                             pseudoMoves.push_back({from, 7 * 8 + 6, piece, Piece::EMPTY, Piece::EMPTY, false, true});
                        }
                    }
                    if (board.blackLong && board.board[7][3].piece == Piece::EMPTY && board.board[7][2].piece == Piece::EMPTY && board.board[7][1].piece == Piece::EMPTY) {
                        if (!board.isSquareAttacked(7, 4, Color::WHITE) && !board.isSquareAttacked(7, 3, Color::WHITE)) {
                             pseudoMoves.push_back({from, 7 * 8 + 2, piece, Piece::EMPTY, Piece::EMPTY, false, true});
                        }
                    }
                }
            }
        }
    }
    return pseudoMoves;
}

std::vector<Move> generateLegalMoves(const Board& board) {
    std::vector<Move> pseudoMoves = generatePseudoLegalMoves(board);
    std::vector<Move> legalMoves;
    Color us = board.whiteTurn ? Color::WHITE : Color::BLACK;
    Color them = board.whiteTurn ? Color::BLACK : Color::WHITE;

    for (const auto& m : pseudoMoves) {
        Board temp = board;
        temp.makeMove(m);

        // Find king
        int kr = -1, kc = -1;
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (temp.board[r][c].piece == Piece::KING && temp.board[r][c].color == us) {
                    kr = r; kc = c;
                    break;
                }
            }
            if (kr != -1) break;
        }

        if (kr != -1 && !temp.isSquareAttacked(kr, kc, them)) {
            legalMoves.push_back(m);
        }
    }
    return legalMoves;
}
