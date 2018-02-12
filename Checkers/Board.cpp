#include "Board.h"

Board::Board()
{
	for (unsigned int i = 0; i < 2; i++)
	{
		p[i] = new Player(Color(i + 1));
	}
	int index[2] = { 0,0 };
	for (unsigned int i = 0; i < 8; i++)
	{
		for (unsigned int j = 0; j < 8; j++)
		{
			tile t;
			t.col = 'A' + j;
			t.row = i;
			int Cindex = ((i % 2 * -1) + j % 2);
			if (Cindex < 0)
			{
				Cindex = 1;
			}
			Color c = Color(Cindex);
			Piece* pie = NULL;
			if (c != Color(white))
			{
				if (i / 3 == 0)
				{
					pie = p[0]->getPiece(index[0]);
					pie->changeLoc(t);
					index[0]++;
				}
				else if (i / 3 == 2 || i == 5)
				{
					pie = p[1]->getPiece(index[1]);
					pie->changeLoc(t);
					index[1]++;
				}
			}
			s[i][j] = new Space(c,t,pie);
		}
	}
	currentPlayer = 0;
	CPUlost = false;
}

Board::Board(Board& b)
{
	for (unsigned int i = 0; i < 2; i++)
	{
		p[i] = new Player(*b.p[i]);
	}
	for (unsigned int i = 0; i < 8; i++)
	{
		for (unsigned int j = 0; j < 8; j++)
		{
			s[i][j] = new Space(*b.s[i][j]);
			Piece* pie = s[i][j]->getPiece();
			if (pie != NULL)
			{
				Player* play= p[0];
				if (pie->getColor() == 'R')
				{
					play = p[1];
				}
				for (unsigned int x = 0; x < 12; x++)
				{
					if (play->getPiece(x)->getStatus() == onB && pie->getLoc().col == play->getPiece(x)->getLoc().col && pie->getLoc().row == play->getPiece(x)->getLoc().row)
					{
						s[i][j]->setPiece(play->getPiece(x));

						break;
					}
				}
			}
		}	
	}
	currentPlayer = b.currentPlayer;
	CPUlost = b.CPUlost;
}
	
void Board::changePlayer()
{
	currentPlayer++;
	if (currentPlayer == 2)
	{
		currentPlayer = 0;
	}
}

bool inBounds(char col, int row)
{
	if (row < 0 || row > 7)
	{
		return false;
	}
	if (col < 'A' || col > 'H')
	{
		return false;
	}
	return true;
}

enum direction {BLeft, BRight, RLeft, RRight};

Space* Board::getSpace(int row, char col)
{
	return s[row][col - 'A'];
}

bool jumpAvailable(tile t, Piece* p, direction d, Board* b)
{
	Space* sp= b->getSpace(t.row, t.col);
	Piece* target = sp->getPiece();
	if (target != NULL && p != NULL && target->getStatus() == onB && p->getStatus() == onB && target->getColor() != p->getColor())
	{
		switch (d)
		{
		case BLeft:
			if (inBounds(t.col - 1, t.row + 1))
			{
				if (b->getSpace(t.row + 1,t.col - 1)->getPiece() == NULL)
				{
					return true;
				}
			}
			break;
		case BRight:
			if (inBounds(t.col + 1, t.row + 1))
			{
				if (b->getSpace(t.row + 1,t.col + 1)->getPiece() == NULL)
				{
					return true;
				}
			}
			break;
		case RLeft:
			if (inBounds(t.col - 1, t.row - 1))
			{
				if (b->getSpace(t.row - 1,t.col - 1)->getPiece() == NULL)
				{
					return true;
				}
			}
			break;
		case RRight:
			if (inBounds(t.col + 1, t.row - 1))
			{
				if (b->getSpace(t.row - 1,t.col + 1)->getPiece() == NULL)
				{
					return true;
				}
			}
			break;
		}
	}
	return false;
}

void BlackMoves(vector<jump> *&j, Piece*& pie, vector<tile>*& moves, Board* b)
{
	if (pie == NULL || pie->getStatus() == offB)
	{
		return;
	}
	if (inBounds(pie->getLoc().col - 1, pie->getLoc().row + 1))
	{
		tile loc;
		loc.col = pie->getLoc().col - 1;
		loc.row = pie->getLoc().row + 1;
		if (jumpAvailable(loc, pie, BLeft, b))
		{
			jump temp;
			temp.chosen = pie->getLoc();
			temp.target = loc;
			temp.target.col--;
			temp.target.row++;
			j->push_back(temp);
		}
		if (b->getSpace(loc.row, loc.col)->getPiece() == NULL)
		{
			moves->push_back(loc);
		}
	}
	if (inBounds(pie->getLoc().col + 1, pie->getLoc().row + 1))
	{
		tile loc;
		loc.col = pie->getLoc().col + 1;
		loc.row = pie->getLoc().row + 1;
		if (jumpAvailable(loc, pie, BRight, b))
		{
			jump temp;
			temp.chosen = pie->getLoc();
			temp.target = loc;
			temp.target.col++;
			temp.target.row++;
			j->push_back(temp);
		}
		if (b->getSpace(loc.row, loc.col)->getPiece() == NULL)
		{
			moves->push_back(loc);
		}
	}
	pie->setMoves(moves);
}

void RedMoves(vector<jump> *&j, Piece*& pie, vector<tile>*& moves, Board* b)
{
	if (pie == NULL || pie->getStatus() == offB)
	{
		return;
	}
	if (inBounds(pie->getLoc().col - 1, pie->getLoc().row - 1))
	{
		tile loc;
		loc.col = pie->getLoc().col - 1;
		loc.row = pie->getLoc().row - 1;
		if (jumpAvailable(loc, pie, RLeft, b))
		{
			jump temp;
			temp.chosen = pie->getLoc();
			temp.target = loc;
			temp.target.col--;
			temp.target.row--;
			j->push_back(temp);
		}
		if (b->getSpace(loc.row, loc.col)->getPiece() == NULL)
		{
			moves->push_back(loc);
		}
	}
	if (inBounds(pie->getLoc().col + 1, pie->getLoc().row - 1))
	{
		tile loc;
		loc.col = pie->getLoc().col + 1;
		loc.row = pie->getLoc().row - 1;
		if (jumpAvailable(loc, pie, RRight, b))
		{
			jump temp;
			temp.chosen = pie->getLoc();
			temp.target = loc;
			temp.target.col++;
			temp.target.row--;
			j->push_back(temp);
		}
		if (b->getSpace(loc.row, loc.col)->getPiece() == NULL)
		{
			moves->push_back(loc);
		}
	}
	pie->setMoves(moves);
}

void KingMoves(vector<jump> *&j, Piece*& pie, vector<tile>*& moves, Board* b)
{
	if (pie == NULL || pie->getStatus() == offB)
	{
		return;
	}
	if (inBounds(pie->getLoc().col - 1, pie->getLoc().row + 1))
	{
		tile loc;
		loc.col = pie->getLoc().col - 1;
		loc.row = pie->getLoc().row + 1;
		if (jumpAvailable(loc, pie, BLeft, b))
		{
			jump temp;
			temp.chosen = pie->getLoc();
			temp.target = loc;
			temp.target.col--;
			temp.target.row++;
			j->push_back(temp);
		}
		if (b->getSpace(loc.row, loc.col)->getPiece() == NULL)
		{
			moves->push_back(loc);
		}
	}
	if (inBounds(pie->getLoc().col + 1, pie->getLoc().row + 1))
	{
		tile loc;
		loc.col = pie->getLoc().col + 1;
		loc.row = pie->getLoc().row + 1;
		if (jumpAvailable(loc, pie, BRight, b))
		{
			jump temp;
			temp.chosen = pie->getLoc();
			temp.target = loc;
			temp.target.col++;
			temp.target.row++;
			j->push_back(temp);
		}
		if (b->getSpace(loc.row, loc.col)->getPiece() == NULL)
		{
			moves->push_back(loc);
		}
	}
	if (inBounds(pie->getLoc().col - 1, pie->getLoc().row - 1))
	{
		tile loc;
		loc.col = pie->getLoc().col - 1;
		loc.row = pie->getLoc().row - 1;
		if (jumpAvailable(loc, pie, RLeft, b))
		{
			jump temp;
			temp.chosen = pie->getLoc();
			temp.target = loc;
			temp.target.col--;
			temp.target.row--;
			j->push_back(temp);
		}
		if (b->getSpace(loc.row, loc.col)->getPiece() == NULL)
		{
			moves->push_back(loc);
		}moves->push_back(loc);
	}
	if (inBounds(pie->getLoc().col + 1, pie->getLoc().row - 1))
	{
		tile loc;
		loc.col = pie->getLoc().col + 1;
		loc.row = pie->getLoc().row - 1;
		if (jumpAvailable(loc, pie, RRight, b))
		{
			jump temp;
			temp.chosen = pie->getLoc();
			temp.target = loc;
			temp.target.col++;
			temp.target.row--;
			j->push_back(temp);
		}
		if (b->getSpace(loc.row, loc.col)->getPiece() == NULL)
		{
			moves->push_back(loc);
		}
	}
	pie->setMoves(moves);
}

vector<jump>* Board::getMoves()
{
	vector<jump>* jumps = new vector<jump>;
	vector<tile>* moves;
	for (unsigned int i = 0; i < 12; i++)
	{
		moves = new vector<tile>;
		Piece* pie = p[currentPlayer]->getPiece(i);
		if (pie != NULL && pie->getStatus() == onB)
		{
			if (pie->isKing())
			{
				KingMoves(jumps, pie, moves, this);
				pie->setMoves(moves);
			}
			else if (pie->compareColor(black))
			{
				BlackMoves(jumps, pie, moves, this);
				pie->setMoves(moves);
			}
			else if (pie->compareColor(red))
			{
				RedMoves(jumps, pie, moves, this);
				pie->setMoves(moves);
			}
		}
	}
	return jumps;
}

vector<jump>* Board::getMoves(Piece* pie)
{
	vector<jump>* jumps = new vector<jump>;
	vector<tile>* moves = new vector<tile>;
	if (pie!= NULL && pie->getStatus() == onB)
	{
		if (pie->isKing())
		{
			KingMoves(jumps, pie, moves, this);
			pie->setMoves(moves);
		}
		else if (pie->compareColor(black))
		{
			BlackMoves(jumps, pie, moves, this);
			pie->setMoves(moves);
		}
		else if (pie->compareColor(red))
		{
			RedMoves(jumps, pie, moves, this);
			pie->setMoves(moves);
		}
	}
	return jumps;
}

bool Board::CheckForWin()
{
	for (unsigned int i = 0; i < 12; i++)
	{
		if (p[currentPlayer]->getPiece(i)->getStatus() == status(onB))
		{
			return false;
		}
	}
	return true;
}

bool Board::RiskofJump(Piece* p)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		tile t[2];
		t[0] = p->getLoc();
		t[1].col = t[0].col + (((i % 2)*-1) + ((((i+1)% 2)*1)));
		t[1].row = t[0].row + (((i / 2)*-2) + 1);
		if (inBounds(t[1].col, t[1].row))
		{
			Piece* danger = getSpace(t[1].row, t[1].col)->getPiece();
			direction d = direction(BLeft + i);
			if (danger != NULL && jumpAvailable(t[0], danger, d, this))
			{
				return true;
			}
		}
	}
	return false;
}

bool Board::jumpA(Piece* p)
{
	int limit = 4;
	int start = 0;
	if (!p->isKing())
	{
		if (p->getColor()== red)
		{
			start = 2;
		}
		else
		{
			limit = 2;
		}
	}
	for (unsigned int i = start; i < limit; i++)
	{
		tile t;
		t.row = p->getLoc().row + (((i / 2)*-1) * 1);
		t.col = p->getLoc().row + (((i % 2)*-1) * 1);
		if (inBounds(t.col, t.row))
		{
			direction d = direction(BLeft + i);
			if (jumpAvailable(t, p, d, this))
			{
				return true;
			}
		}
	}
	return false;
}

void Board::Display()
{
	char col = 'A';
	int row = 1;
	int p[2] = { 0,0 };
	for (int i = -1; i < 33; i++)
	{
		for (int j = -1; j < 33; j++)
		{
			if (i == -1)
			{
				if (j % 4 == 2)
				{
					cout << col;
					col++;
				}
				else
				{
					cout << " ";
				}
			}
			else
			{
				if (j == -1)
				{
					if (i % 4 == 2)
					{
						cout << row;
						row++;
					}
					else
					{
						cout << " ";
					}
				}
				else
				{
					switch (i % 4)
					{
					case 0:
						cout << "#";
						break;
					case 1:
					case 2:
					case 3:
						switch (j % 4)
						{
						case 0:
							cout << "#";
							break;
						case 1:
						case 3:
							if (s[i / 4][j / 4]->getColor() == Color(white))
							{
								cout << "|";
							}
							else if (s[i/4][j/4]->getPiece() != NULL && s[i / 4][j / 4]->getPiece()->isKing())
							{
								cout << "^";
							}
							else
							{
								cout << " ";
							}
							break;
						case 2:
							if (i % 4 == 2)
							{
								if (s[i / 4][j / 4]->getColor() == Color(white))
								{
									cout << "|";
								}
								else
								{
									if (s[i / 4][j / 4]->getPiece() != NULL)
									{
										cout << s[i / 4][j / 4]->getPiece()->getColor();
									}
									else
									{
										cout << " ";
									}
								}
							}
							else
							{
								if (s[i / 4][j / 4]->getColor() == Color(white))
								{
									cout << "|";
								}
								else
								{
									cout << " ";
								}
							}
						}

					}
				}
			}
		}
		cout << endl;
	}
}

Board::~Board()
{
	for (unsigned int i = 0; i < 2; i++)
	{
		delete p[i];
	}
	for (unsigned int i = 0; i < 8; i++)
	{
		for (unsigned int j = 0; j < 8; j++)
		{
			delete s[i][j];
		}
	}
}
