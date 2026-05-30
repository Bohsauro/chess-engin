#include "board.h"
#include <iostream>

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

    whiteTurn = true;
    moves = 1;
    halfMoves = 0;
    fiftyMoves = 0;
    enPassant = -1;
    whiteLong = whiteShort = blackLong = blackShort = true;
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

void Board::makeMove(const Move& m) {
    int fr = m.from / 8, fc = m.from % 8;
    int tr = m.to / 8, tc = m.to % 8;

    Piece p = board[fr][fc].piece;
    Color c = board[fr][fc].color;

    // Reset enPassant square
    int oldEnPassant = enPassant;
    enPassant = -1;

    // Handle En Passant capture
    if (m.isEnPassant) {
        int capR = fr;
        int capC = tc;
        board[capR][capC] = { Piece::EMPTY, Color::NONE };
    }

    // Handle Castling
    if (m.isCastling) {
        if (tc == 6) { // Kingside
            int rookF = 7, rookT = 5;
            board[tr][rookT] = board[tr][rookF];
            board[tr][rookF] = { Piece::EMPTY, Color::NONE };
        } else if (tc == 2) { // Queenside
            int rookF = 0, rookT = 3;
            board[tr][rookT] = board[tr][rookF];
            board[tr][rookF] = { Piece::EMPTY, Color::NONE };
        }
    }

    // Move the piece
    board[tr][tc] = board[fr][fc];
    board[fr][fc] = { Piece::EMPTY, Color::NONE };

    // Handle Promotion
    if (m.promotionPiece != Piece::EMPTY) {
        board[tr][tc].piece = m.promotionPiece;
    }

    // Set new En Passant square
    if (p == Piece::PAWN && std::abs(tr - fr) == 2) {
        enPassant = (fr + tr) / 2 * 8 + fc;
    }

    // Update castling rights
    if (p == Piece::KING) {
        if (c == Color::WHITE) whiteShort = whiteLong = false;
        else blackShort = blackLong = false;
    }
    if (p == Piece::ROOK) {
        if (c == Color::WHITE) {
            if (fr == 0 && fc == 0) whiteLong = false;
            if (fr == 0 && fc == 7) whiteShort = false;
        } else {
            if (fr == 7 && fc == 0) blackLong = false;
            if (fr == 7 && fc == 7) blackShort = false;
        }
    }
    // Also if a rook is captured
    if (tr == 0 && tc == 0) whiteLong = false;
    if (tr == 0 && tc == 7) whiteShort = false;
    if (tr == 7 && tc == 0) blackLong = false;
    if (tr == 7 && tc == 7) blackShort = false;

    if (c == Color::BLACK) moves++;
    if (p == Piece::PAWN || m.capturedPiece != Piece::EMPTY) fiftyMoves = 0;
    else fiftyMoves++;

    whiteTurn = !whiteTurn;
}

bool Board::isSquareAttacked(int r, int c, Color attackerColor) const {
    int pawnDir = (attackerColor == Color::WHITE) ? -1 : 1;

    // Pawn
    for (int dc : {-1, 1}) {
        int nr = r + pawnDir, nc = c + dc;
        if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
            if (board[nr][nc].piece == Piece::PAWN && board[nr][nc].color == attackerColor) return true;
        }
    }

    // Knight
    int knr[] = {1, 2, 2, 1, -1, -2, -2, -1}, knc[] = {2, 1, -1, -2, -2, -1, 1, 2};
    for (int i = 0; i < 8; i++) {
        int nr = r + knr[i], nc = c + knc[i];
        if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
            if (board[nr][nc].piece == Piece::KNIGHT && board[nr][nc].color == attackerColor) return true;
        }
    }

    // Sliding pieces
    int dr[] = {1, -1, 0, 0, 1, 1, -1, -1}, dc[] = {0, 0, 1, -1, 1, -1, 1, -1};
    for (int i = 0; i < 8; i++) {
        for (int d = 1; d < 8; d++) {
            int nr = r + dr[i] * d, nc = c + dc[i] * d;
            if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8) break;
            if (board[nr][nc].piece != Piece::EMPTY) {
                if (board[nr][nc].color == attackerColor) {
                    Piece p = board[nr][nc].piece;
                    if (i < 4 && (p == Piece::ROOK || p == Piece::QUEEN)) return true;
                    if (i >= 4 && (p == Piece::BISHOP || p == Piece::QUEEN)) return true;
                }
                break;
            }
        }
    }

    // King
    for (int drr = -1; drr <= 1; drr++) {
        for (int dcc = -1; dcc <= 1; dcc++) {
            if (drr == 0 && dcc == 0) continue;
            int nr = r + drr, nc = c + dcc;
            if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                if (board[nr][nc].piece == Piece::KING && board[nr][nc].color == attackerColor) return true;
            }
        }
    }

    return false;
}
