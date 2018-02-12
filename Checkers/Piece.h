#pragma once
#include "Enum.h"
#include <vector>
struct tile
{
	char col;
	int row;
};

using namespace std;

class Piece
{
private:
	Color c;
	tile loc;
	vector<tile>* moves;
	status s;
	bool king;
public:
	Piece();
	Piece(Color, status);
	Piece(Piece&);
	~Piece();
	bool compareColor(Color);
	char getColor();
	tile getLoc() { return loc; }
	vector<tile>* getMoves() { return moves; }
	void setMoves(vector<tile>* m) { moves = m; }
	void changeLoc(tile l) { loc.col = l.col; loc.row = l.row; }
	status getStatus() { return s; }
	void removePiece() { s = offB; }
	void KingMe() { king = true; }
	bool isKing() { return king; }
};

