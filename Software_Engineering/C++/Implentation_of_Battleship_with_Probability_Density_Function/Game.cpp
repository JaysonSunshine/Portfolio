#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

struct Ship
{
	int length;
	char symbol;
	string name;
};

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
  private:
	int m_nRows;
	int m_nCols;
	vector<Ship> shipVec;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
	: m_nRows(nRows), m_nCols(nCols){}

int GameImpl::rows() const
{
    return m_nRows;
}

int GameImpl::cols() const
{
    return m_nCols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    Ship a;
	a.length = length;
	a.symbol = symbol;
	a.name = name;
	shipVec.push_back(a);
	return true;
}

int GameImpl::nShips() const
{
    return shipVec.size();
}

int GameImpl::shipLength(int shipId) const
{
	return shipVec[shipId].length;
}

char GameImpl::shipSymbol(int shipId) const
{
    return shipVec[shipId].symbol;
}

string GameImpl::shipName(int shipId) const
{
    return shipVec[shipId].name;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    int counter = 0;
	bool a = false;
	a = p1->placeShips(b1);
	while(a == false)
	{		
		if(counter == MAX_PLACEMENT_TRIES)
			return NULL;
		b1.clear();
		a = p1->placeShips(b1);
		counter++;
	}
	counter = 0;
	a = p2->placeShips(b2);
	while(a == false)
	{
		if(counter == MAX_PLACEMENT_TRIES)
			return NULL;
		a = p2->placeShips(b2);
		counter++;
	}

	bool shotHit, shipDestroyed;
	int shipId;
	Point p;
	
	while(!(b1.allShipsDestroyed() || b2.allShipsDestroyed())) //HOW CAN HAVE ONE RUNTHROUGH FOR EACH PLAYER
	{
		cout << p1->name() << "'s turn.  Board for " << p2->name() << ":" << endl;
		if(p1->isHuman())
			b2.display(true);
		else
			b2.display(false);
		p = p1->recommendAttack();
		bool a, validShot;
		validShot = true;
		a = b2.attack(p, shotHit, shipDestroyed, shipId);
		//int counter = 0;
		while(a == false && !p1->isHuman())
		{	
			p = p1->recommendAttack();
			a = b2.attack(p, shotHit, shipDestroyed, shipId);//HOW TO HANDLE SHIPS 6 OR GREATER
			/*if(counter > 1000)
				p1->shipDestroyed() = true;
			counter ++;*/
		}
		p1->recordAttackResult(p, validShot, shotHit, shipDestroyed, shipId);
		if(a == false && p1->isHuman())
			cout << p1->name() << " the Human wasted a shot at <" << p.r << "," << p.c << ">." << endl;
		else
		{	
			cout << p1->name() << " attacked <" << p.r << "," << p.c << "> and ";
			if(shipDestroyed == true)
				cout << "destroyed the " << shipVec[shipId].name << ", resulting in:" << endl;
			else if(shotHit == true)
				cout << "hit something, resulting in:" << endl;
			else
				cout << "missed, resulting in:" << endl;
		}
		if(p1->isHuman())
			b2.display(true);
		else
			b2.display(false);
		if(!b2.allShipsDestroyed())
		{
			waitForEnter();//WHEN SHOULD PAUSE
			cout << p2->name() << "'s turn.  Board for " << p1->name() << ":" << endl;
			if(p2->isHuman())
				b1.display(true);
			else
				b1.display(false);
			p = p2->recommendAttack();
			a = b1.attack(p, shotHit, shipDestroyed, shipId);
			while(a == false && !p2->isHuman())
			{	
				p = p2->recommendAttack();
				a = b1.attack(p, shotHit, shipDestroyed, shipId);//HOW TO HANDLE SHIPS 6 OR GREATER
			}
			p2->recordAttackResult(p, validShot, shotHit, shipDestroyed, shipId);
			if(a == false && p2->isHuman())
				cout << p2->name() << " the Human wasted a shot at <" << p.r << "," << p.c << ">." << endl;
			else
			{	
				cout << p2->name() << " attacked <" << p.r << "," << p.c << "> and ";
				if(shipDestroyed == true)
					cout << "destroyed the " << shipVec[shipId].name << ", resulting in:" << endl;
				else if(shotHit == true)
					cout << "hit something, resulting in:" << endl;
				else
					cout << "missed, resulting in:" << endl;
			}
			if(p2->isHuman())
				b1.display(true);
			else
				b1.display(false);
			if(!b1.allShipsDestroyed())
				waitForEnter(); //WHEN SHOULD PAUSE
		}
	}

	if(b1.allShipsDestroyed())
		cout << p2->name() << " wins!" << endl;
	else
		cout << p1->name() << " wins!" << endl;
	
	if(shouldPause)
	{
		waitForEnter();
	}
	
	return NULL;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == NULL  ||  p2 == NULL  ||  nShips() == 0)
        return NULL;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}