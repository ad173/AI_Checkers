#include "Piece.h"

Piece::Piece()
{
}

Piece::Piece(Color p, status stat)
{
	c = p;
	s = stat;
	king = false;
}

Piece::Piece(Piece& p)
{
	c = p.c;
	s = p.s;
	moves = new vector<tile>;
	for (unsigned int i = 0; p.moves != NULL && i < p.moves->size(); i++)
	{
		moves->push_back(p.moves->at(i));
	}
	loc.col = p.loc.col;
	loc.row = p.loc.row;
	king = p.king;
}

bool Piece::compareColor(Color com)
{
	return (c == com);
}

char Piece::getColor()
{
	if (c == Color(red))
	{
		return 'R';
	}
	return 'B';
}

Piece::~Piece()
{
	delete moves;
}
