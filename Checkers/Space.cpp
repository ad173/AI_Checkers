#include "Space.h"

Space::Space()
{
}

Space::Space(Color col, tile loc, Piece* pie)
{
	c = col;
	t = loc;
	p = pie;
}

Space::Space(Space& sp)
{
	c = sp.c;
	t = sp.t;
	p = sp.p;
}

void Space::setPiece(Piece* pie)
{
	p = pie;
	if (p != NULL)
	{
		p->changeLoc(t);
	}
}

Piece* Space::getPiece()
{
	if (p != NULL)
	{
		return p;
	}
	return NULL;
}

Space::~Space()
{
}
