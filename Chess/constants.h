#pragma once

#include <string>
#include <fstream>

using namespace std;

enum class PIECE {
	PAWN,
	BISHOP,
	KNIGHT,
	KING,
	QUEEN,
	ROOK
};

enum class COLOR {
	WHITE,
	BLACK
};

enum class TURN {
	WHITE,
	BLACK
};
