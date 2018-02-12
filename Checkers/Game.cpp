#include "Board.h"
#include <string>
#include <sstream>

tile* getTiles(string s)
{
	tile t[2];
	int size = s.size();

	t[0].col = *s.substr(0, 1).c_str();
	string s0 = s.substr(1, 2);
	stringstream convert0(s0);
	convert0 >> t[0].row;
	t[0].row--;

	t[1].col = *s.substr(size - 2, size - 1).c_str();
	string s1 = s.substr(size - 1, size);
	stringstream convert1(s1);
	convert1 >> t[1].row;
	t[1].row--;
	return t;
}

bool isAJump(tile* t, vector<jump>* jumps)
{
	for (unsigned int i = 0; i < jumps->size(); i++)
	{
		if (jumps->at(i).chosen.col == t[0].col && jumps->at(i).chosen.row == t[0].row && jumps->at(i).target.col == t[1].col && jumps->at(i).target.row == t[1].row)
		{
			return true;
		}
	}
	return false;
}

bool aValidMove(tile t[2], Board* b)
{
	vector<tile>* moves = new vector<tile>;
	Piece* p = b->getPlayer(b->getCP())->getPiece(t[0]);
	if (p != NULL && p->compareColor(Color(black + b->getCP())))
	{
		moves = p->getMoves();
		for (unsigned int i = 0; i < moves->size(); i++)
		{
			if (moves->at(i).col == t[1].col && moves->at(i).row == t[1].row)
			{
				return true;
			}
		}
	}
	return false;
}

bool checkForKing(tile t)
{
	if (t.row == 7 || t.row == 0)
	{
		return true;
	}
	return false;
}

struct AlphaBeta
{
	int Alpha;
	int Beta;
	Board* current;
	Board* first;
	AlphaBeta* previous;
};

void makeMove(tile* t, Board*& b, bool jump, bool CPU, AlphaBeta*& ab, Color c);

int getHeuristic(Board* b, Color c)
{
	int heuristic = 0;
	for (unsigned int j = 0; j < 2; j++)
	{
		Player* p = b->getPlayer(j);
		for (unsigned int i = 0; i < 12; i++)
		{
			Piece* pie = p->getPiece(i);
			if (pie->getStatus() == onB && pie->compareColor(c))
			{
				heuristic++;
				if (pie->isKing())
				{
					heuristic += 2;
					if (b->getCP() != c - 1 && b->RiskofJump(pie))
					{
						heuristic -= 3;
					}
					else if (b->getCP() == c - 1 && b->jumpA(pie))
					{
						heuristic++;
					}
				}
				else
				{
					if (b->getCP() != c - 1 && b->RiskofJump(pie))
					{
						heuristic -= 2;
					}
					else if (b->getCP() == c - 1 && b->jumpA(pie))
					{
						heuristic++;
					}
				}
			}
			else if(pie->getStatus() == onB)
			{
				heuristic--;
				if (pie->isKing())
				{
					heuristic -= 3;
				}
			}
		}
	}
	return heuristic;
}

void FindMove(AlphaBeta*& ab, Color c, int count)
{
	if(count < 5)
	{
		Player* p = ab->current->getPlayer(ab->current->getCP());
		vector<jump>* jumps = ab->current->getMoves();
		tile* t = new tile[2];
		if (jumps->size() != 0)
		{
			if (count == 0)
			{
				vector<AlphaBeta*> possible;
				for (unsigned int i = 0; i < jumps->size(); i++)
				{
					Piece* pie = p->getPiece(jumps->at(i).chosen);
					if (pie->getStatus() == onB)
					{
						t[0] = jumps->at(i).chosen;
						t[1] = jumps->at(i).target;
						AlphaBeta* Temp = new AlphaBeta;
						Temp->Alpha = ab->Alpha;
						Temp->Beta = ab->Beta;
						Temp->current = new Board(*ab->current);
						Temp->previous = ab;
						makeMove(t, Temp->current, true, true, Temp, c);
						Temp->current->changePlayer();
						Temp->first = Temp->current;
						possible.push_back(Temp);
					}
				}
				int max = 0;
				for (unsigned int i = 1; i < possible.size(); i++)
				{
					int MHeuristic = getHeuristic(possible.at(max)->current, c);
					int OHeuristic = getHeuristic(possible.at(i)->current, c);
					if (MHeuristic < OHeuristic)
					{
						max = i;
					}
				}
				ab = possible.at(max);
				return;
			}
			for (unsigned int i = 0; i < jumps->size(); i++)
			{
				if (!(ab->Beta > ab->Alpha))
				{
					Piece* pie = p->getPiece(jumps->at(i).chosen);
					if (pie->getStatus() == onB)
					{
						t[0] = jumps->at(i).chosen;
						t[1] = jumps->at(i).target;
						AlphaBeta* Temp = new AlphaBeta;
						Temp->Alpha = ab->Alpha;
						Temp->Beta = ab->Beta;
						Temp->current = new Board(*ab->current);
						Temp->previous = ab;
						makeMove(t, Temp->current, true, true, Temp, c);
						Temp->current->changePlayer();
						if (count == 0)
						{
							Temp->first = new Board(*Temp->current);
						}
						else
						{
							Temp->first = ab->first;
						}
						FindMove(Temp, c, count + 1);
					}
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < 12; i++)
			{
				Piece* pie = p->getPiece(i);
				if (pie->getStatus() == onB)
				{
					vector<tile>* moves = pie->getMoves();
					t[0] = pie->getLoc();
					for (unsigned int j = 0; j < moves->size(); j++)
					{
						if (!(ab->Beta > ab->Alpha))
						{
							t[1] = moves->at(j);
							AlphaBeta* Temp = new AlphaBeta;
							Temp->Alpha = ab->Alpha;
							Temp->Beta = ab->Beta;
							Temp->current = new Board(*ab->current);
							Temp->previous = ab;
							makeMove(t, Temp->current, false, true, Temp, c);
							Temp->current->changePlayer();
							if (count == 0)
							{
								Temp->first = new Board(*Temp->current);
							}
							else
							{
								Temp->first = ab->first;
							}
							FindMove(Temp, c, count + 1);
						}
						else
						{
							break;
						}
					}
				}
			}
		}
	}
	else if (count == 5 && ab->current->getCP() == c - 1)
	{
		int beta = getHeuristic(ab->current, c);
		if (ab->Beta < beta)
		{
			ab->Beta = beta;
		}
	}
	else if(count == 5)
	{
		int alpha = getHeuristic(ab->current, c);
		if (ab->Alpha > alpha)
		{
			ab->Alpha = alpha;
		}
	}
	if (ab->previous != NULL)
	{
		if (ab->current->getCP() == c - 1)
		{
			if (ab->previous->Alpha > ab->Beta)
			{
				ab->previous->Alpha = ab->Beta;
				ab->previous->first = ab->first;
			}
		}
		else
		{
			if (ab->previous->Beta < ab->Alpha)
			{
				ab->previous->Beta = ab->Alpha;
				ab->previous->first = ab->first;
			}
		}
	}
	if (ab->previous == NULL && ab->first == NULL)
	{
		ab->first->CPUdidLose();
		ab->current->CPUdidLose();
	}
	if (ab->previous != NULL && ab->previous->previous != NULL)
	{
		delete ab->current;
		delete ab;
	}
}

void continueJump(Board*& b, tile t, bool CPU, AlphaBeta*& ab, Color c)
{
	Piece* p = b->getSpace(t.row, t.col)->getPiece();
	vector<jump>* jumps = b->getMoves(p);
	if (jumps->size() != 0)
	{
		if (!CPU)
		{
			b->Display();
			cout << "There is another jump available!" << endl;
			cout << "\nPlayer " << b->getCP() + 1 << " choose the same piece and where you would like to move.\nRemember the move must be a jump!\nUse format of \"ColRow to ColRow\"" << endl;
			string UInput = "";
			getline(cin, UInput);
			tile* temp = new tile[2];
			temp = getTiles(UInput);
			tile t[2];
			for (unsigned int i = 0; i < 2; i++)
			{
				t[i].col = temp[i].col;
				t[i].row = temp[i].row;
			}
			while (!isAJump(t, jumps))
			{
				cout << "Invalid jump move, Enter a different move";
				getline(cin, UInput);
				temp = new tile[2];
				temp = getTiles(UInput);
				for (unsigned int i = 0; i < 2; i++)
				{
					t[i].col = temp[i].col;
					t[i].row = temp[i].row;
				}
			}
			makeMove(t, b, true, false, ab, c);
		}
		else
		{
			vector<AlphaBeta*> possibles;
			for (unsigned int i = 0; i < jumps->size(); i++)
			{
				tile* t = new tile[2];
				t[0] = jumps->at(i).chosen;
				t[1] = jumps->at(i).target;
				AlphaBeta* Temp = new AlphaBeta;
				Temp->Alpha = ab->Alpha;
				Temp->Beta = ab->Beta;
				Temp->current = new Board(*ab->current);
				Temp->first = ab->first;
				Temp->previous = ab->previous;
				makeMove(t, Temp->current, true, true, Temp, c);
				possibles.push_back(Temp);
			}
			int max = 0;
			for (unsigned int i = 1; i < possibles.size(); i++)
			{
				int other = getHeuristic(possibles.at(i)->current, c);
				if (other > getHeuristic(possibles.at(max)->current, c))
				{
					max = i;
				}
			}
			ab = possibles.at(max);
		}
	}
}

void makeMove(tile* t, Board*& b, bool jump, bool CPU, AlphaBeta*& ab, Color c)
{
	if (CPU)
	{
		Space* p0 = ab->current->getSpace(t[0].row, t[0].col);
		Space* p1 = ab->current->getSpace(t[1].row, t[1].col);
		Piece* pie = p0->getPiece();
		Player* p = ab->current->getPlayer(ab->current->getCP());
		Piece* Pie = p->getPiece(p0->getLoc());
		p1->setPiece(pie);
		p0->setPiece(NULL);
		if (checkForKing(t[1]))
		{
			Piece* king = p1->getPiece();
			if (king != NULL)
			{
				king->KingMe();
			}
		}
		if (jump)
		{
			Space* pDelete = ab->current->getSpace((p0->getLoc().row + p1->getLoc().row) / 2, (p0->getLoc().col + p1->getLoc().col) / 2);
			if (pDelete == NULL)
			{
				return;
			}
			pDelete->getPiece()->removePiece();
			pDelete->setPiece(NULL);
			continueJump(b, t[1], CPU, ab, c);
		}
	}
	else
	{
		Space* p0 = b->getSpace(t[0].row, t[0].col);
		Space* p1 = b->getSpace(t[1].row, t[1].col);
		Piece* pie = p0->getPiece();
		Player* p = b->getPlayer(b->getCP());
		Piece* Pie = p->getPiece(p0->getLoc());
		p1->setPiece(pie);
		Pie->changeLoc(p1->getLoc());
		p0->setPiece(NULL);
		if (checkForKing(t[1]))
		{
			Piece* king = p1->getPiece();
			if (king != NULL)
			{
				king->KingMe();
			}
		}
		if (jump)
		{
			Space* pDelete = b->getSpace((p0->getLoc().row + p1->getLoc().row) / 2, (p0->getLoc().col + p1->getLoc().col) / 2);
			if (pDelete == NULL)
			{
				return;
			}
			pDelete->getPiece()->removePiece();
			pDelete->setPiece(NULL);
			continueJump(b, t[1], CPU, ab, c);
		}
	}
}

void User(Board*& b)
{
	AlphaBeta* ab = new AlphaBeta;
	vector<jump>* jumps = b->getMoves();
	if (jumps->size() != 0)
	{
		cout << "There is a jump available!" << endl;
		cout << "\nPlayer " << b->getCP()+1 << " choose the piece you would like to move and where you would like to move.\nRemember the move must be a jump!\nUse format of \"ColRow to ColRow\"\nFor Example: \"D5 to E4\"" << endl;
		string UInput = "";
		getline(cin, UInput);
		tile* temp = new tile[2];
		temp = getTiles(UInput);
		tile t[2];
		for (unsigned int i = 0; i < 2; i++)
		{
			t[i].col = temp[i].col;
			t[i].row = temp[i].row;
		}
		while (!isAJump(t, jumps))
		{
			cout << "Invalid jump move, Enter a different move\nRemember to use upper case entries" << endl;
			getline(cin, UInput);
			temp = new tile[2];
			temp = getTiles(UInput);
			for (unsigned int i = 0; i < 2; i++)
			{
				t[i].col = temp[i].col;
				t[i].row = temp[i].row;
			}
		}
		makeMove(t, b, true, false, ab, white);
	}
	else
	{
		cout << "\nPlayer " << b->getCP()+1 << " choose the piece you would like to move and where you would like to move.\nUse format of \"ColRow to ColRow\"\nFor Example: \"D5 to E4\"" << endl;
		string UInput = "";
		getline(cin, UInput);
		tile* temp = new tile[2];
		temp = getTiles(UInput);
		tile t[2];
		for (unsigned int i = 0; i < 2; i++)
		{
			t[i].col = temp[i].col;
			t[i].row = temp[i].row;
		}
		while (!aValidMove(t,b))
		{
			cout << "Invalid move! Enter a different move! \nRemember you are the ";
			if (b->getCP() == 0)
			{
				cout << "Black Pieces" << endl;
			}
			else
			{
				cout << " Red Pieces" << endl;
			}
			cout << "Also remember the entry must be in Upper Case!" << endl;
			temp = new tile[2];
			getline(cin, UInput);
			temp = getTiles(UInput);
			for (unsigned int i = 0; i < 2; i++)
			{
				t[i].col = temp[i].col;
				t[i].row = temp[i].row;
			}
		}
		makeMove(t, b, false, false, ab, white);
	}
	delete ab;
	return;
}

void RegPlay(Board*& b)
{
	cout << "Player One is Black\nPlayer Two is Red" << endl;
	while (!b->CheckForWin())
	{
		b->Display();
		User(b);
		b->changePlayer();
	}
	b->Display();
	cout << "Player " << b->getCP() << " loses!" << endl;
}

void CPU(Board*& b, Color c)
{
	cout << "CPU is thinking..." << endl;
	AlphaBeta* start = new AlphaBeta;
	start->Alpha = 10000;
	start->Beta = -10000;
	start->current = new Board(*b);
	start->previous = NULL;
	FindMove(start, c, 0);
	b = start->first;
}

void CPUPlay(Board* b)
{
	Color cpuC = black;
	cout << "What color would you like to be? (1=black, 2=red)" << endl;
	int userIn = 0;
	cin >> userIn;
	while (userIn != 1 && userIn != 2)
	{
		cout << "Invalid selection, choose either Black or Red" << endl;
		cin >> userIn;
	}
	cin.ignore();
	if (userIn == 1)
	{
		cpuC = red;
	}
	while (!b->didCPUlose() && !b->CheckForWin())
	{
		b->Display();
		if (b->getCP() + 1 == cpuC)
		{
			CPU(b, cpuC);
		}
		else
		{
			User(b);
			b->changePlayer();
		}
	}
	if (b->didCPUlose())
	{
		cout << "Congrats! You Won!" << endl;
	}
	else
	{
		cout << "You lost! The Computer is superior!" << endl;
	}
}

int main()
{
	while (true)
	{
		Board* b = new Board();
		cout << "How many players are playing? (1 or 2)" << endl;
		int userIn = 0;
		cin >> userIn;
		cin.ignore();
		while (!(userIn > 0 && userIn <= 2))
		{
			cout << "Invalid Entry! Re-Enter number of Players" << endl;
			cin >> userIn;
		}
		cout << "Remember to use upper case entries to make your moves!" << endl;
		if (userIn == 1)
		{
			CPUPlay(b);
		}
		else
		{
			RegPlay(b);
		}
		cout << "\n Would you like to play again? \n(0 = yes, Anything Else = no)" << endl;
		cin >> userIn;
		delete b;
		if (userIn != 0)
		{
			return 0;
		}
	}
}