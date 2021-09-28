#pragma once

//What is left
//Enpassant - finished
//check - finished
//Pinned pieces - finished
//Undo move - finished
//checkmate - finished
//Castling - generate all of X's possible moves check if the two tiles next to the king are in the moves for king side and do the same for queen side
//Piece promotion
//Draw (3 move repetition, 50 move rule)
//Read and write pgn
//Testing
#include <vector>
#include "board.h"

class Chess
{
private:
	TURN turn;
	Board board;

public:
	Chess() :turn(TURN::WHITE) {};
	void switch_turn();
	void game_loop();
	vector<pair<int, int>> generate_poss_mvs(int, int);
	bool white_check();
	bool black_check();
	bool white_check_mate();
	bool black_check_mate();
};
