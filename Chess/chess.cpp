#include "chess.h"
#include "utilities.h"
#include <iostream>

using namespace std;

void Chess::switch_turn()
{
	if (turn == TURN::WHITE)
		turn = TURN::BLACK;
	else turn = TURN::WHITE;
}

template <class T>
bool is_in(T ele, const vector<T>& vec)
{
	for (T x : vec)
		if (x == ele)
			return true;
	return false;
}

void display_vec(const vector<pair<int, int>>& vec)
{
	for (auto x : vec)
		cout << x.first << " " << x.second << endl;
	cout << endl;
}

void Chess::game_loop()
{
	board.display();
	ifstream file_read("file.txt");
	ofstream file_write("file.txt");
	file_write << "h1h3" << endl;
	while (true)
	{
		try
		{
			string temp = "BLACK";
			if (turn == TURN::WHITE)
				temp = "WHITE";
			cout << "Turn: " << temp << endl;

			if (white_check_mate())
			{
				cout << "CHECKMATE" << endl;
				return;
			}
			else if (black_check_mate())
			{
				cout << "CHECKMATE" << endl;
				return;
			}

			if (white_check())
				cout << "CHECK!" << endl;
			if (black_check())
				cout << "CHECK!" << endl;

			string str;
			cin >> str;

			if (str == "u")
			{
				board.undo_move();
				board.display();
				switch_turn();
				continue;
			}

			if (!is_chess_not(str))
				throw(exception("Invalid chess notation!"));

			pair<int, int> temp0 = convert_chess_not(str.substr(0, 2));
			pair<int, int> temp1 = convert_chess_not(str.substr(2, 2));

			int x1 = temp0.first, y1 = temp0.second;
			int x2 = temp1.first, y2 = temp1.second;
			cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;

			//If piece color matches turn
			if (board.get_tile(x1, y1).get_hasPc() &&
				((board.get_tile(x1, y1).get_piece().get_color() == COLOR::WHITE && turn == TURN::WHITE)
					||
				 (board.get_tile(x1, y1).get_piece().get_color() == COLOR::BLACK && turn == TURN::BLACK)))
			{
				auto poss_movs = generate_poss_mvs(x1, y1);
				display_vec(poss_movs);
				if (is_in<pair<int, int>>(make_pair(x2, y2), poss_movs))
				{
					board.make_move(x1, y1, x2, y2);
					if (turn == TURN::WHITE)
					{
						if (white_check())
						{
							board.undo_move();
							board.display();
							cout << "White is in check!" << endl;
							continue;
						}
					}
					else if (black_check())
					{
						board.undo_move();
						board.display();
						cout << "Black is in check!" << endl;
						continue;
					}
					file_write << str << endl;
					switch_turn();
				}
				else
				{
					board.display();
					throw(exception("Wrong move"));
				}
			}
			else
			{
				board.display();
				throw(exception("Unable to make move"));
			}
			board.display();
		}
		catch (exception& e)
		{
			cerr << e.what() << endl;
		}
		catch (...)
		{
			cerr << "Unknown exception found!" << endl;
		}
	}
}

vector<pair<int, int>> Chess::generate_poss_mvs(int x, int y)
{
	if (x < 0 || x>7 || y < 0 || y>7)
		throw(exception("Invalid entry!"));

	if (!board.get_tile(x, y).get_hasPc())
		throw(exception("Unable to generate moves"));

	vector<pair<int, int>> res;
	Piece pc = board.get_tile(x, y).get_piece();

	if (pc.get_piece() == PIECE::BISHOP)
	{
		//Diagonal 1
		int i = x - 1, j = y + 1;
		while (true)
		{
			if (i < 0 || i>7 || j < 0 || j>7)
				break;

			if (board.get_tile(i, j).get_hasPc())//If the board[i][j] has a piece
			{
				//If you find a piece with the same color on the path
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
				break;
			}
			else
				res.push_back(make_pair(i, j));
			--i, ++j;
		}

		//Diagonal 2
		i = x + 1, j = y - 1;
		while (true)
		{
			if (i < 0 || i>7 || j < 0 || j>7)
				break;
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
				break;
			}
			else
				res.push_back(make_pair(i, j));

			++i, --j;
		}

		//Diagonal 3
		i = x + 1, j = y + 1;
		while (true)
		{
			if (i < 0 || i>7 || j < 0 || j>7)
				break;
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
				break;
			}
			else
				res.push_back(make_pair(i, j));

			++i, ++j;
		}

		//Diagonal 4
		i = x - 1, j = y - 1;
		while (true)
		{
			if (i < 0 || i>7 || j < 0 || j>7)
				break;
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
				break;
			}
			else
				res.push_back(make_pair(i, j));

			--i, --j;
		}
	}
	else if (pc.get_piece() == PIECE::KING)
	{
		//Castling moves
		if (x == 7 && y == 4 && pc.get_color() == COLOR::WHITE)
		{
			if (!white_check())
			{
				if (board.get_tile(x, y + 1).get_hasPc() == false && board.get_tile(x, y+2).get_hasPc() == false)
				{
					if (board.get_tile(x, y + 3).get_piece().get_color() == COLOR::WHITE && board.get_tile(x, y + 3).get_piece().get_piece() == PIECE::ROOK)
					{
						if (board.wht_csl_king)
						{
							res.push_back(make_pair(x, y + 2));
						}
					}
				}

				if (board.get_tile(x, y - 1).get_hasPc() == false && board.get_tile(x, y-2).get_hasPc() == false && board.get_tile(x, y-3).get_hasPc() == false)
				{
					if (board.get_tile(x, y - 4).get_piece().get_color() == COLOR::WHITE && board.get_tile(x, y - 4).get_piece().get_piece() == PIECE::ROOK)
					{
						if (board.wht_csl_queen)
						{
							res.push_back(make_pair(x, y - 2));
						}
					}
				}
				
			}
		}

		if (x == 0 && y == 4 && pc.get_color() == COLOR::BLACK)
		{
			if (!black_check())
			{
				if (board.get_tile(x, y + 1).get_hasPc() == false && board.get_tile(x, y+2).get_hasPc() == false)
				{
					if (board.get_tile(x, y + 3).get_piece().get_color() == COLOR::BLACK && board.get_tile(x, y + 3).get_piece().get_piece() == PIECE::ROOK)
					{
						if (board.blk_csl_king)
						{
							res.push_back(make_pair(x, y + 2));
						}
					}
				}

				if (board.get_tile(x, y - 1).get_hasPc() == false && board.get_tile(x, y-2).get_hasPc() == false && board.get_tile(x, y-3).get_hasPc() == false)
				{
					if (board.get_tile(x, y - 4).get_piece().get_color() == COLOR::BLACK && board.get_tile(x, y - 4).get_piece().get_piece() == PIECE::ROOK)
					{
						if (board.blk_csl_queen)
						{
							res.push_back(make_pair(x, y - 2));
						}
					}
				}
			}
		}
		int i = x + 1, j = y;
		if (is_correct_pos(i, j))
		{
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
			}
			else
				res.push_back(make_pair(i, j));
		}
		
		i = x + 1, j = y + 1;
		if (is_correct_pos(i, j))
		{
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
			}
			else
				res.push_back(make_pair(i, j));
		}

		i = x, j = y + 1;
		if (is_correct_pos(i, j))
		{
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
			}
			else
				res.push_back(make_pair(i, j));
		}

		i = x - 1 , j = y + 1;
		if (is_correct_pos(i, j))
		{
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
			}
			else
				res.push_back(make_pair(i, j));
		}

		i = x - 1 , j = y;
		if (is_correct_pos(i, j))
		{
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
			}
			else
				res.push_back(make_pair(i, j));
		}

		i = x - 1 , j = y - 1;
		if (is_correct_pos(i, j))
		{
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
			}
			else
				res.push_back(make_pair(i, j));
		}

		i = x, j = y - 1;
		if (is_correct_pos(i, j))
		{
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
			}
			else
				res.push_back(make_pair(i, j));
		}

		i = x + 1 , j = y - 1;
		if (is_correct_pos(i, j))
		{
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
			}
			else
				res.push_back(make_pair(i, j));
		}
	}
	else if (pc.get_piece() == PIECE::KNIGHT)
	{
		int i = x - 2, j = y - 1;
		if (is_correct_pos(i, j))
		{
			if (!board.get_tile(i, j).get_hasPc())
			{
				res.push_back(make_pair(i, j));
			}
			else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
			{
				res.push_back(make_pair(i, j));
			}
		}

		i = x - 2, j = y + 1;
		if (is_correct_pos(i, j))
		{
			if (!board.get_tile(i, j).get_hasPc())
			{
				res.push_back(make_pair(i, j));
			}
			else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
			{
				res.push_back(make_pair(i, j));
			}
		}

		i = x - 1, j = y + 2;
		if (is_correct_pos(i, j))
		{
			if (!board.get_tile(i, j).get_hasPc())
			{
				res.push_back(make_pair(i, j));
			}
			else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
			{
				res.push_back(make_pair(i, j));
			}
		}

		i = x + 1, j = y + 2;
		if (is_correct_pos(i, j))
		{
			if (!board.get_tile(i, j).get_hasPc())
			{
				res.push_back(make_pair(i, j));
			}
			else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
			{
				res.push_back(make_pair(i, j));
			}
		}

		i = x + 2, j = y + 1;
		if (is_correct_pos(i, j))
		{
			if (!board.get_tile(i, j).get_hasPc())
			{
				res.push_back(make_pair(i, j));
			}
			else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
			{
				res.push_back(make_pair(i, j));
			}
		}
		
		i = x + 2, j = y - 1;
		if (is_correct_pos(i, j))
		{
			if (!board.get_tile(i, j).get_hasPc())
			{
				res.push_back(make_pair(i, j));
			}
			else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
			{
				res.push_back(make_pair(i, j));
			}
		}		
		
		i = x - 1, j = y - 2;
		if (is_correct_pos(i, j))
		{
			if (!board.get_tile(i, j).get_hasPc())
			{
				res.push_back(make_pair(i, j));
			}
			else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
			{
				res.push_back(make_pair(i, j));
			}
		}

		i = x + 1, j = y - 2;
		if (is_correct_pos(i, j))
		{
			if (!board.get_tile(i, j).get_hasPc())
			{
				res.push_back(make_pair(i, j));
			}
			else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
			{
				res.push_back(make_pair(i, j));
			}
		}
	}
	//Left off at PAWN
	else if (pc.get_piece() == PIECE::PAWN)
	{
		if (pc.get_color() == COLOR::WHITE)
		{
			//Enpassant move
			string ls_mv = get_last_move();
			pair<int, int> st_mv = convert_chess_not(ls_mv.substr(0, 2));
			pair<int, int> end_mv = convert_chess_not(ls_mv.substr(2, 2));
			if (st_mv.first == x - 2 && st_mv.second == y - 1 && end_mv.first == x && end_mv.second == y - 1 && x == 3)
				res.push_back(make_pair(x - 1, y - 1));
			else if (st_mv.first == x - 2 && st_mv.second == y + 1 && end_mv.first == x && end_mv.second == y + 1 && x == 3)
				res.push_back(make_pair(x - 1, y + 1));

			int i = x - 1, j = y;
			if (is_correct_pos(i, j) && !board.get_tile(i, j).get_hasPc())
				res.push_back(make_pair(i, j));
			
			i = x - 2, j = y;
			if (is_correct_pos(i, j) && !board.get_tile(i + 1, j).get_hasPc() && !board.get_tile(i, j).get_hasPc() && x == 6)
				res.push_back(make_pair(i, j));

			i = x - 1, j = y - 1;
			if (is_correct_pos(i, j) && board.get_tile(i, j).get_hasPc() && board.get_tile(i, j).get_piece().get_color() != pc.get_color())
				res.push_back(make_pair(i, j));

			i = x - 1, j = y + 1;
			if (is_correct_pos(i, j) && board.get_tile(i, j).get_hasPc() && board.get_tile(i, j).get_piece().get_color() != pc.get_color())
				res.push_back(make_pair(i, j));
		}
		else
		{
			//Enpassant move
			string ls_mv = get_last_move();
			pair<int, int> st_mv = convert_chess_not(ls_mv.substr(0, 2));
			pair<int, int> end_mv = convert_chess_not(ls_mv.substr(2, 2));
			if (st_mv.first == x + 2 && st_mv.second == y - 1 && end_mv.first == x && end_mv.second == y - 1 && x == 4)
				res.push_back(make_pair(x + 1, y - 1));
			else if (st_mv.first == x + 2 && st_mv.second == y + 1 && end_mv.first == x && end_mv.second == y + 1 && x == 4)
				res.push_back(make_pair(x + 1, y + 1));

			int i = x + 1, j = y;
			if (is_correct_pos(i, j) && !board.get_tile(i, j).get_hasPc())
				res.push_back(make_pair(i, j));

			i = x + 2, j = y;
			if (is_correct_pos(i, j) && !board.get_tile(i - 1, j).get_hasPc() && !board.get_tile(i, j).get_hasPc() && x == 1)//for pushing twice
				res.push_back(make_pair(i, j));

			i = x + 1, j = y - 1;
			if (is_correct_pos(i, j) && board.get_tile(i, j).get_hasPc() && board.get_tile(i, j).get_piece().get_color() != pc.get_color())
				res.push_back(make_pair(i, j));

			i = x + 1, j = y + 1;
			if (is_correct_pos(i, j) && board.get_tile(i, j).get_hasPc() && board.get_tile(i, j).get_piece().get_color() != pc.get_color())
				res.push_back(make_pair(i, j));

		}
	}
	else if (pc.get_piece() == PIECE::QUEEN)
	{
		//Up
		int i = x - 1, j = y;
		while (true)
		{
			if (is_correct_pos(i, j))
			{
				if (!board.get_tile(i, j).get_hasPc())
				{
					res.push_back(make_pair(i, j));
				}
				else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
				{
					res.push_back(make_pair(i, j));
					break;
				}
				else
					break;
			}
			else
				break;
			--i;
		}

		//Down
		i = x + 1, j = y;
		while (true)
		{
			if (is_correct_pos(i, j))
			{
				if (!board.get_tile(i, j).get_hasPc())
				{
					res.push_back(make_pair(i, j));
				}
				else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
				{
					res.push_back(make_pair(i, j));
					break;
				}
				else
					break;
			}
			else
				break;
			++i;
		}

		//Left
		i = x, j = y - 1;
		while (true)
		{
			if (is_correct_pos(i, j))
			{
				if (!board.get_tile(i, j).get_hasPc())
				{
					res.push_back(make_pair(i, j));
				}
				else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
				{
					res.push_back(make_pair(i, j));
					break;
				}
				else
					break;
			}
			else
				break;
			--j;
		}

		//Right
		i = x, j = y + 1;
		while (true)
		{
			if (is_correct_pos(i, j))
			{
				if (!board.get_tile(i, j).get_hasPc())
				{
					res.push_back(make_pair(i, j));
				}
				else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
				{
					res.push_back(make_pair(i, j));
					break;
				}
				else
					break;
			}
			else
				break;
			++j;
		}
		
		//Diagonal 1
		i = x - 1, j = y + 1;
		while (true)
		{
			if (i < 0 || i>7 || j < 0 || j>7)
				break;

			if (board.get_tile(i, j).get_hasPc())//If the board[i][j] has a piece
			{
				//If you find a piece with the same color on the path
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
				break;
			}
			else
				res.push_back(make_pair(i, j));
			--i, ++j;
		}

		//Diagonal 2
		i = x + 1, j = y - 1;
		while (true)
		{
			if (i < 0 || i>7 || j < 0 || j>7)
				break;
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
				break;
			}
			else
				res.push_back(make_pair(i, j));

			++i, --j;
		}

		//Diagonal 3
		i = x + 1, j = y + 1;
		while (true)
		{
			if (i < 0 || i>7 || j < 0 || j>7)
				break;
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
					res.push_back(make_pair(i, j));
				break;
			}
			else
				res.push_back(make_pair(i, j));

			++i, ++j;
		}

		//Diagonal 4
		i = x - 1, j = y - 1;
		while (true)
		{
			if (i < 0 || i>7 || j < 0 || j>7)
				break;

			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
				{
					res.push_back(make_pair(i, j));
					break;
				}
				else
					break;
			}
			else
				res.push_back(make_pair(i, j));

			--i, --j;
		}
	}
	else if (pc.get_piece() == PIECE::ROOK)
	{
		//Up
		int i = x - 1, j = y;
		while (true)
		{
			if (is_correct_pos(i, j))
			{
				if (!board.get_tile(i, j).get_hasPc())
				{
					res.push_back(make_pair(i, j));
				}
				else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
				{
					res.push_back(make_pair(i, j));
					break;
				}
				else
					break;
			}
			else
				break;
			--i;
		}

		//Down
		i = x + 1, j = y;
		while (true)
		{
			if (is_correct_pos(i, j))
			{
				if (!board.get_tile(i, j).get_hasPc())
				{
					res.push_back(make_pair(i, j));
				}
				else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
				{
					res.push_back(make_pair(i, j));
					break;
				}
				else
					break;
			}
			else
				break;
			++i;
		}

		//Left
		i = x, j = y - 1;
		while (true)
		{
			if (is_correct_pos(i, j))
			{
				if (!board.get_tile(i, j).get_hasPc())
				{
					res.push_back(make_pair(i, j));
				}
				else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
				{
					res.push_back(make_pair(i, j));
					break;
				}
				else
					break;
			}
			else
				break;
			--j;
		}

		//Right
		i = x, j = y + 1;
		while (true)
		{
			if (is_correct_pos(i, j))
			{
				if (!board.get_tile(i, j).get_hasPc())
				{
					res.push_back(make_pair(i, j));
				}
				else if (board.get_tile(i, j).get_piece().get_color() != pc.get_color())
				{
					res.push_back(make_pair(i, j));
					break;
				}
				else
					break;
			}
			else
				break;
			++j;
		}
	}

	return res;
}

bool Chess::white_check()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_piece() == PIECE::KING)
					continue;

				if (board.get_tile(i, j).get_piece().get_color() == COLOR::BLACK)
				{
					auto mvs = generate_poss_mvs(i, j);
					for (auto x : mvs)
					{
						if (board.get_tile(x.first, x.second).get_hasPc())
						{
							if (board.get_tile(x.first, x.second).get_piece().get_piece() == PIECE::KING && board.get_tile(x.first, x.second).get_piece().get_color() == COLOR::WHITE)
								return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool Chess::black_check()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (board.get_tile(i, j).get_hasPc())
			{
				if (board.get_tile(i, j).get_piece().get_piece() == PIECE::KING)
					continue;

				if (board.get_tile(i, j).get_piece().get_color() == COLOR::WHITE)
				{
					auto mvs = generate_poss_mvs(i, j);
					for (auto x : mvs)
					{
						if (board.get_tile(x.first, x.second).get_hasPc())
						{
							if (board.get_tile(x.first, x.second).get_piece().get_piece() == PIECE::KING && board.get_tile(x.first, x.second).get_piece().get_color() == COLOR::BLACK)
								return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool Chess::white_check_mate()
{
	//We don't want to change the last move when we make the moves for check mate
	Move mv0 = board.get_lst_mv();
	if (white_check())
	{
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				if (board.get_tile(i, j).get_hasPc())
				{
					if (board.get_tile(i, j).get_piece().get_color() == COLOR::WHITE)
					{
						auto poss_mvs = generate_poss_mvs(i, j);
						for (auto mv : poss_mvs)
						{
							board.make_move(i, j, mv.first, mv.second);
							if (!white_check())
							{
								board.undo_move();
								board.set_lst_mv(mv0);
								return false;
							}
							board.undo_move();
						}
					}
				}
			}
		}
	}
	else
	{
		board.set_lst_mv(mv0);
		return false;
	}

	board.set_lst_mv(mv0);
	return true;
}

bool Chess::black_check_mate()
{
	//We don't want to change the last move when we make moves for check mate
	Move mv0 = board.get_lst_mv();
	if (black_check())
	{
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				if (board.get_tile(i, j).get_hasPc())
				{
					if (board.get_tile(i, j).get_piece().get_color() == COLOR::BLACK)
					{
						auto poss_mvs = generate_poss_mvs(i, j);
						for (auto mv : poss_mvs)
						{
							board.make_move(i, j, mv.first, mv.second);
							if (!black_check())
							{
								board.undo_move();
								board.set_lst_mv(mv0);
								return false;
							}
							board.undo_move();
						}
					}
				}
			}
		}
	}
	else
	{
		board.set_lst_mv(mv0);
		return false;
	}

	board.set_lst_mv(mv0);
	return true;
}
