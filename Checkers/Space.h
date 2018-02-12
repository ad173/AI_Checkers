#pragma once
#include "Piece.h"
class Space
{
private:
	Color c;
	Piece* p;
	tile t;
public:
	Space();
	Space(Color, tile, Piece*);
	Space(Space&);
	~Space();
	tile getLoc() { return t; }
	Piece* getPiece();
	Color getColor() { return c; }
	void setPiece(Piece* pie);
};

