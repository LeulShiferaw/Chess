#pragma once
#include <string>
#include "constants.h"

using namespace std;

string get_last_move();
bool is_correct_pos(int, int);
bool is_chess_not(string);
pair<int, int> convert_chess_not(string);

