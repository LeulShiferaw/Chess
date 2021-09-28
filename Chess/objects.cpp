#include "objects.h"

//Piece Functions
//Defaultly creates a white pawn
Piece::Piece():col(COLOR::WHITE), pc(PIECE::PAWN)
{
}

Piece::Piece(PIECE pc, COLOR col)
{
	this->col = col;
	this->pc = pc;
}
//End Piece Functions

//Tile Functions
Tile::Tile():has_pc(false)
{
}

Tile::Tile(COLOR col, bool has_pc)
{
	this->col = col;
	this->has_pc = has_pc;
}

Tile::Tile(COLOR col, bool has_pc, Piece pc)
{
	this->col = col;
	this->has_pc = has_pc;
	if (has_pc)
	{
		this->pc = pc;
	}
}
//End Tile Functions
