#pragma once
#include "Piece.h"
class Player
{
private:
	Piece* p[12];
	Color c;
public:
	Player();
	Player(Color);
	Player(Player&);
	~Player();
	Piece* getPiece(int i) { return p[i]; }
	Piece* getPiece(tile);
	Color getColor() { return c; }
};

