#pragma once
#include "Player.h"
#include "Space.h"
#include <iostream>

struct jump
{
	tile chosen;
	tile target;
};

class Board
{
private:
	Space* s[8][8];
	Player* p[2];
	int currentPlayer;
	bool CPUlost;
public:
	Board();
	Board(Board& b);
	~Board();
	void Display();
	bool CheckForWin();
	int getCP() { return currentPlayer; }
	void changePlayer();
	vector<jump>* getMoves();
	vector<jump>* getMoves(Piece*);
	Space* getSpace(int row, char col);
	Player* getPlayer(int i) { return p[i]; }
	bool RiskofJump(Piece*);
	bool jumpA(Piece* p);
	bool didCPUlose() { return CPUlost; }
	void CPUdidLose() { CPUlost = true; }
};

