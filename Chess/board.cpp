#include "board.h"
#include <iostream>

using namespace std;

Board::Board()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (i % 2 == 0)
			{
				if (j % 2 == 0)
				{
					board[i][j] = Tile(COLOR::WHITE, false);
				}
				else
					board[i][j] = Tile(COLOR::BLACK, false);
			}
			else if (j % 2 == 0)
			{
				board[i][j] = Tile(COLOR::BLACK, false);
			}
			else
			{
				board[i][j] = Tile(COLOR::WHITE, false);
			}
		}
	}

	//Initialization
	Piece white_king(PIECE::KING, COLOR::WHITE);
	
	Piece black_king(PIECE::KING, COLOR::BLACK);
	Piece white_queen(PIECE::QUEEN, COLOR::WHITE);
	Piece black_queen(PIECE::QUEEN, COLOR::BLACK);
	Piece white_knight(PIECE::KNIGHT, COLOR::WHITE);
	Piece black_knight(PIECE::KNIGHT, COLOR::BLACK);
	Piece white_bishop(PIECE::BISHOP, COLOR::WHITE);
	Piece black_bishop(PIECE::BISHOP, COLOR::BLACK);
	Piece white_pawn(PIECE::PAWN, COLOR::WHITE);
	Piece black_pawn(PIECE::PAWN, COLOR::BLACK);
	Piece white_rook(PIECE::ROOK, COLOR::WHITE);
	Piece black_rook(PIECE::ROOK, COLOR::BLACK);

	//BLACK PIECES
	board[0][0].set_hasPc(true);
	board[0][0].set_pc(black_rook);

	board[0][7].set_hasPc(true);
	board[0][7].set_pc(black_rook);

	board[0][1].set_hasPc(true);
	board[0][1].set_pc(black_knight);

	board[0][6].set_hasPc(true);
	board[0][6].set_pc(black_knight);

	board[0][2].set_hasPc(true);
	board[0][2].set_pc(black_bishop);

	board[0][5].set_hasPc(true);
	board[0][5].set_pc(black_bishop);

	board[0][3].set_hasPc(true);
	board[0][3].set_pc(black_queen);

	board[0][4].set_hasPc(true);
	board[0][4].set_pc(black_king);

	//WHITE PIECES
	board[7][0].set_hasPc(true);
	board[7][0].set_pc(white_rook);

	board[7][7].set_hasPc(true);
	board[7][7].set_pc(white_rook);

	board[7][1].set_hasPc(true);
	board[7][1].set_pc(white_knight);

	board[7][6].set_hasPc(true);
	board[7][6].set_pc(white_knight);

	board[7][2].set_hasPc(true);
	board[7][2].set_pc(white_bishop);

	board[7][5].set_hasPc(true);
	board[7][5].set_pc(white_bishop);

	board[7][3].set_hasPc(true);
	board[7][3].set_pc(white_queen);

	board[7][4].set_hasPc(true);
	board[7][4].set_pc(white_king);

	for (int i = 0; i < 8; ++i)
	{
		board[1][i].set_hasPc(true);
		board[1][i].set_pc(black_pawn);
	}

	for (int i = 0; i < 8; ++i)
	{
		board[6][i].set_hasPc(true);
		board[6][i].set_pc(white_pawn);
	}
}

void Board::display()
{
	cout << "+---+---+---+---+---+---+---+---+" << endl;
	for (int i = 0; i < 8; ++i)
	{
		cout << "|";
		for (int j = 0; j < 8; ++j)
		{
			//If tile has a piece
			if (board[i][j].get_hasPc())
			{
				Piece temp = board[i][j].get_piece();
				PIECE temp1 = temp.get_piece();
				if (temp1 == PIECE::BISHOP)
				{
					if (temp.get_color() == COLOR::WHITE)
					{
						cout << " B ";
					}
					else
						cout << " b ";
				}
				else if (temp1 == PIECE::KING)
				{
					if (temp.get_color() == COLOR::WHITE)
					{
						cout << " K ";
					}
					else
						cout << " k ";	
				}
				else if (temp1 == PIECE::KNIGHT)
				{
					if (temp.get_color() == COLOR::WHITE)
					{
						cout << " N ";
					}
					else
						cout << " n ";
				}
				else if (temp1 == PIECE::PAWN)
				{
					if (temp.get_color() == COLOR::WHITE)
					{
						cout << " P ";
					}
					else
						cout << " p ";
				}
				else if (temp1 == PIECE::QUEEN)
				{
					if (temp.get_color() == COLOR::WHITE)
					{
						cout << " Q ";
					}
					else
						cout << " q ";
				}
				else if (temp1 == PIECE::ROOK)
				{
					if (temp.get_color() == COLOR::WHITE)
					{
						cout << " R ";
					}
					else
						cout << " r ";
				}
			}
			else if (board[i][j].get_color() == COLOR::WHITE)//Else if tile is white
			{
				cout << "   ";
			}
			else//Else if tile is black
			{
				cout << "   ";
			}
			cout << "|";
		}
		cout << " " << 8 - (i);
		cout << endl;
		cout << "+---+---+---+---+---+---+---+---+" << endl;
	}

	cout << "  a   b   c   d   e   f   g   h" << endl;
}

void Board::make_move(int x1, int y1, int x2, int y2)
{
	if (x1 >= 8 || x1 < 0 || y1 >= 8 || y1 < 0 || x2 >= 8 || y2 < 0)
	{
		throw(exception("Invalid coordinates"));
	}

	if (!board[x1][y1].get_hasPc())
	{
		throw(exception("Unable to make move"));
	}

	//Castling
	if (board[x1][y1].get_hasPc() && board[x1][y1].get_piece().get_piece() == PIECE::KING)
	{
		if (x1 == 7 && y1 == 4)
		{
			if (x2 == 7 && y2 == 6)
			{
				make_move(7, 7, 7, 5);
			}
			else if (x2 == 7 && y2 == 2)
			{
				make_move(7, 0, 7, 3);
			}
		}
		else if (x1 == 0 && y1 == 4)
		{
			if (x1 == 0 && y1 == 6)
				make_move(0, 7, 0, 5);
			else if (x2 == 0 && y2 == 2)
				make_move(0, 0, 0, 3);
		}
		if (board[x1][y1].get_piece().get_color() == COLOR::WHITE)
		{
			wht_csl_king = false;
			wht_csl_queen = false;
		}
		else
		{
			blk_csl_king = false;
			blk_csl_queen = false;
		}
	}
	if (board[x1][y1].get_hasPc() && board[x1][y1].get_piece().get_piece() == PIECE::ROOK)
	{
		if (board[x1][y1].get_piece().get_color() == COLOR::WHITE)
		{
			if (x1 == 7 && y1 == 0)
				wht_csl_queen = false;
			else if (x1 == 7 && y1 == 7)
				wht_csl_king = false;
		}
		else if (x1 == 0 && y1 == 0)
			blk_csl_queen = false;
		else if (x1 == 0 && y1 == 7)
			blk_csl_king = false;
	}

	//Enpassant move
	if (board[x1][y1].get_piece().get_piece() == PIECE::PAWN)
	{
		if (board[x1][y1].get_piece().get_color() == COLOR::WHITE)
		{
			if (x2 == x1 - 1 && y2 == y1 - 1 && board[x2][y2].get_hasPc() == false)
			{
				Move mv(x1, y1, x2, y2, true, true);
				last_move = mv;
				board[x2][y2].set_hasPc(true);
				board[x2][y2].set_pc(board[x1][y1].get_piece());
				board[x1][y1].set_hasPc(false);
				board[x1][y1 - 1].set_hasPc(false);
				return;
			}
			else if (x2 == x1 - 1 && y2 == y1 + 1 && board[x2][y2].get_hasPc() == false)
			{
				Move mv(x1, y1, x2, y2, true, true);
				last_move = mv;
				board[x2][y2].set_hasPc(true);
				board[x2][y2].set_pc(board[x1][y1].get_piece());
				board[x1][y1].set_hasPc(false);
				board[x1][y1 + 1].set_hasPc(false);
				return;
			}
		}
		else if (x2 == x1 + 1 && y2 == y1 - 1 && board[x2][y2].get_hasPc() == false)
		{
			Move mv(x1, y1, x2, y2, true, true);
			last_move = mv;
			board[x2][y2].set_hasPc(true);
			board[x2][y2].set_pc(board[x1][y1].get_piece());
			board[x1][y1].set_hasPc(false);
			board[x1][y1 - 1].set_hasPc(false);
			return;
		}
		else if (x2 == x1 + 1 && y2 == y1 + 1 && board[x2][y2].get_hasPc() == false)
		{
			Move mv(x1, y1, x2, y2, true, true);
			last_move = mv;
			board[x2][y2].set_hasPc(true);
			board[x2][y2].set_pc(board[x1][y1].get_piece());
			board[x1][y1].set_hasPc(false);
			board[x1][y1 + 1].set_hasPc(false);
			return;
		}
	}
	
	if (board[x2][y2].get_hasPc())
	{
		Move mv(x1, y1, x2, y2, false, true, board[x2][y2].get_piece());
		last_move = mv;
	}
	else
	{
		Move mv(x1, y1, x2, y2, false, false);
		last_move = mv;
	}

	board[x2][y2].set_hasPc(true);
	board[x2][y2].set_pc(board[x1][y1].get_piece());
	board[x1][y1].set_hasPc(false);
}

void Board::undo_move()
{
	board[last_move.sx][last_move.sy].set_hasPc(true);
	board[last_move.sx][last_move.sy].set_pc(board[last_move.ex][last_move.ey].get_piece());
	board[last_move.ex][last_move.ey].set_hasPc(false);

	if (last_move.is_enpass)
	{
		if (board[last_move.sx][last_move.sy].get_piece().get_color() == COLOR::WHITE)
		{
			if (last_move.ex == last_move.sx - 1 && last_move.ey == last_move.sy + 1)
			{
				board[last_move.sx][last_move.sy + 1].set_hasPc(true);
				board[last_move.sx][last_move.sy + 1].set_pc(Piece(PIECE::PAWN, COLOR::BLACK));
			}
			else if (last_move.ex == last_move.sx - 1 && last_move.ey == last_move.sy - 1)
			{
				board[last_move.sx][last_move.sy - 1].set_hasPc(true);
				board[last_move.sx][last_move.sy - 1].set_pc(Piece(PIECE::PAWN, COLOR::BLACK));
			}
		}
		else if (last_move.ex == last_move.sx + 1 && last_move.ey == last_move.sy + 1)
		{
			board[last_move.sx][last_move.sy + 1].set_hasPc(true);
			board[last_move.sx][last_move.sy + 1].set_pc(Piece(PIECE::PAWN, COLOR::WHITE));
		}
		else if (last_move.ex == last_move.sx + 1 && last_move.ey == last_move.sy - 1)
		{
			board[last_move.sx][last_move.sy - 1].set_hasPc(true);
			board[last_move.sx][last_move.sy - 1].set_pc(Piece(PIECE::PAWN, COLOR::WHITE));
		}	
	}
	else if (last_move.ate_piece)
	{
		board[last_move.ex][last_move.ey].set_hasPc(true);
		board[last_move.ex][last_move.ey].set_pc(last_move.pc);
	}
}

Tile Board::get_tile(int x, int y)
{
	if (x >= 8 || x < 0 || y >= 8 || y < 0)
	{
		throw(exception("Invalid coordinates"));
	}
	return board[x][y];
}
