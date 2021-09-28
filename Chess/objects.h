#pragma once
#include "constants.h"


class Piece
{
private:
	PIECE pc;
	COLOR col;
public:
	Piece();
	Piece(PIECE, COLOR cl = COLOR::WHITE);
	PIECE get_piece() { return pc; }
	COLOR get_color() { return col; }
	void set_piece(const PIECE& p) { pc = p; }
	void set_color(const COLOR& col) { this->col = col; }
};

class Tile
{
private:
	COLOR col;
	bool has_pc;
	Piece pc;
public:
	Tile();
	Tile(COLOR, bool);
	Tile(COLOR, bool, Piece);
	bool get_hasPc() { return has_pc; }
	void set_hasPc(bool bl) { has_pc = bl; }
	void set_pc(Piece p) { pc = p; }
	COLOR get_color() { return col; }
	Piece get_piece() { return pc; }
};

//For storing the last move
struct Move
{
	bool is_enpass = false;
	int sx, sy, ex, ey;
	bool ate_piece;
	Piece pc;
	Move() {}
	Move(int stx, int sty, int enx, int eny, bool enpass, bool ate_pc, Piece piece = Piece(PIECE::PAWN)) :sx(stx), sy(sty), ex(enx), ey(eny), ate_piece(ate_pc), pc(piece)
	{
	}
};
