#pragma once

#include "objects.h"

class Board
{
private:
	Tile board[8][8];
	Move last_move;

public:
	//For castling
	bool blk_csl_queen = true;
	bool blk_csl_king = true;
	bool wht_csl_queen = true;
	bool wht_csl_king = true;

	Board();
	void display();
	void make_move(int, int, int, int);
	void undo_move();
	Move get_lst_mv() { return last_move; }
	void set_lst_mv(const Move &m) { last_move = m; }
	Tile get_tile(int, int);
};
