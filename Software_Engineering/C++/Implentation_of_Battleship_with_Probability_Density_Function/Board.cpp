#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <vector>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
	bool ShipDestroyed(char symbol);
    bool allShipsDestroyed() const;
	int findShip(char symbol);
  private:
    const Game& m_game;
	vector< vector<char> > m_board;
	int m_shipsLeft;
};

BoardImpl::BoardImpl(const Game& g)
	: m_game(g), m_board(m_game.rows(), vector<char>(m_game.cols(), '.')), m_shipsLeft(m_game.nShips())
{
}

void BoardImpl::clear()
{
	vector <vector<char> > temp(m_game.rows(), vector<char>(m_game.cols(), '.'));
	m_board = temp; //SIMPLER WAY TO IMPLEMENT?
}

void BoardImpl::block()
{
      // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            if (randInt(2) == 0)
            {
				m_board[r][c] = '#'; 
            }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
			if(m_board[r][c] == '#')
				m_board[r][c] = '.'; 
        }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
	if(shipId < 0 || shipId >= m_game.nShips())
		return false;
	if(dir == HORIZONTAL && (topOrLeft.c + m_game.shipLength(shipId) > m_game.cols()))
		return false;
	if(dir == VERTICAL && (topOrLeft.r + m_game.shipLength(shipId) > m_game.rows()))
		return false;
	for(int r = 0; r < m_game.rows(); r++)
	{	
		for(int c = 0; c < m_game.cols(); c++)
		{	
			if(m_board[r][c] == m_game.shipSymbol(shipId))
				return false;
		}
	}
	if(dir == HORIZONTAL)
	{	
		for(int c = 0; c < m_game.shipLength(shipId); c++)
		{	
			if(m_board[topOrLeft.r][topOrLeft.c + c] != '.')
				return false;
		}
		for(int c = 0; c < m_game.shipLength(shipId); c++)
		{	
			m_board[topOrLeft.r][c + topOrLeft.c] = m_game.shipSymbol(shipId);
		}
	}
	else
	{
		for(int c = 0; c < m_game.shipLength(shipId); c++)
		{	
			if(m_board[c + topOrLeft.r][topOrLeft.c] != '.')
				 return false;
		}
		for(int c = 0; c < m_game.shipLength(shipId); c++)
		{	
			m_board[c + topOrLeft.r][topOrLeft.c] = m_game.shipSymbol(shipId);
		}
	}
	return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
	if(shipId < 0 || shipId >= m_game.nShips())
		return false;
	if(dir == HORIZONTAL)
	{
	   for(int c = 0; c < m_game.shipLength(shipId); c++)
	   {
		   if(m_board[topOrLeft.r][c + topOrLeft.c] != m_game.shipSymbol(shipId))
				return false;
	   }
	   for(int c = 0; c < m_game.shipLength(shipId); c++)
	   {
		   m_board[topOrLeft.r][c + topOrLeft.c] = '.';
	   }
	}
	else
	{
	   for(int c = 0; c < m_game.shipLength(shipId); c++)
	   {
		   if(m_board[c + topOrLeft.r][topOrLeft.c] != m_game.shipSymbol(shipId))
				return false;
	   }
	   for(int c = 0; c < m_game.shipLength(shipId); c++)
	   {
		   m_board[c + topOrLeft.r][topOrLeft.c] = '.';
	   }
	}
	return true;
}

void BoardImpl::display(bool shotsOnly) const
{
	cout << "  ";
	for(int i = 0; i < m_game.cols(); i++)
		cout << i;
	cout << endl;
	for(int i = 0; i < m_game.rows(); i++)
	{	
		cout << i << " ";
		for(int j = 0; j < m_game.cols(); j++)
		{	
			if(shotsOnly)
			{	
				if(m_board[i][j] == '.' || m_board[i][j] == 'X' || m_board[i][j] == 'o')
					cout << m_board[i][j]; //COULD I OUTPUT THE ENTIRE ROW OF THE VECTOR?
				else
					cout << '.';
			}
			else
				cout << m_board[i][j];

		}
		cout << endl;
	}
}

bool BoardImpl::ShipDestroyed(char symbol)
{
	for(int i = 0; i < m_game.rows(); i++)
	{
		for(int j = 0; j < m_game.cols(); j++)
		{
			if(m_board[i][j] == symbol)
				return false;
		}
	}

	m_shipsLeft--;
	return true;
}

int BoardImpl::findShip(char symbol)
{
	for(int i = 0; i < m_game.nShips(); i++)
	{	
		if(m_game.shipSymbol(i) == symbol)
			return i;
	}
	return -1;
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    shotHit = false;
	shipDestroyed = false;
	if(!m_game.isValid(p))
		return false;
	if(m_board[p.r][p.c] == 'X' || m_board[p.r][p.c] == 'o')
		return false;
	if(m_board[p.r][p.c] != '.')
	{
		shotHit = true;
		char symbol = m_board[p.r][p.c];
		m_board[p.r][p.c] = 'X';
		if(ShipDestroyed(symbol))
		{	
			shipDestroyed = true;
			shipId = findShip(symbol);
			allShipsDestroyed();
		}
	}
	else
		m_board[p.r][p.c] = 'o';
	return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    if(m_shipsLeft == 0)
		return true;
	
	return false;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
