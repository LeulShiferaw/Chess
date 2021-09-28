#include <iostream>
#include "chess.h"

using namespace std;

int main()
{
	try
	{
		Chess chs;
		chs.game_loop();
	}
	catch (exception e)
	{
		cerr << e.what() << endl;
		return 1;
	}
	catch (...)
	{
		cerr << "Unknown exception" << endl;
		return 2;
	}
	return 0;
}