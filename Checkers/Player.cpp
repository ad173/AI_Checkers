#include "Player.h"

Player::Player()
{
}

Player::Player(Color r)
{
	c = r;
	for (unsigned int i = 0; i < 12; i++)
	{
		p[i] = new Piece(r, status(onB));
	}
}

Player::Player(Player& pl)
{
	c = pl.c;
	for (unsigned int i = 0; i < 12; i++)
	{
		p[i] = new Piece(*pl.p[i]);
	}
}

Piece* Player::getPiece(tile t)
{
	for (unsigned int i = 0; i < 12; i++)
	{
		if (p[i]->getStatus() == onB && p[i]->getLoc().col == t.col && p[i]->getLoc().row == t.row)
		{
			return p[i];
		}
	}	
	return NULL;
}

Player::~Player()
{
	for (unsigned int i = 0; i < 12; i++)
	{
		delete p[i];
	}
}
