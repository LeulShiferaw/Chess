#include <vector>
#include "utilities.h"

using namespace std;

string get_last_move()
{
	ifstream file_read("file.txt");
	file_read.seekg(-6, ios::end);
	string str;
	file_read >> str;
	return str;
}

bool is_correct_pos(int x, int y)
{
	if (x < 0 || x>7 || y < 0 || y>7)
		return false;
	return true;
}

bool is_chess_not(string str)
{
	if (str.size() != 4)
		return false;
	if (str[0] < 'a' || str[0] > 'h' || str[2] < 'a' || str[2] > 'h')
		return false;
	if (str[1] < '1' || str[1]>'8' || str[3] < '1' || str[3]>'8')
		return false;
	return true;
}

//Convert from chess notation to (int, int) form
//Example: e4 = (4, 3)
pair<int, int> convert_chess_not(string chessNot)
{
	if (chessNot[0] > 'h' || chessNot[0] < 'a' || chessNot[1] < '1' || chessNot[1]>'8')
		throw(exception("Invalid input!"));
	pair<int, int> res;
	res.first = 7 - (chessNot[1] - '0' - 1);
	res.second = chessNot[0] - 'a';
	return res;
}