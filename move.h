#pragma once
#include <vector>

#include "board.h"

// Converte un indice 0–63 in una stringa tipo "e2"
std::string squareToString(int index);

// Converte una stringa tipo "e2" in un indice 0–63
int stringToSquare(const std::string& s);

std::vector<Move> generateLegalMoves(const Board& board);