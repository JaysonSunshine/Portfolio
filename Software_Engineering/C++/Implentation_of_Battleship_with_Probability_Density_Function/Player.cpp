#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Ship
{
	int length;
	char symbol;
	string name;
};

bool operator<(const Point &a, const Point &b)
{
	if(a.r != b.r)
		return (a.r < b.r);
	else
		return (a.c < b.c);
}

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if (!b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c) //WHAT DOES THIS DO?  JUST CHECK FOR NUMERICAL INPUT?
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player
{
  public:
    HumanPlayer(string nm, const Game& g);
	virtual bool isHuman() const { return true;}
	bool dirCheck(char a, Direction& dir);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
};

HumanPlayer::HumanPlayer(string nm, const Game& g)
	: Player(nm, g)
{}

bool HumanPlayer::dirCheck(char a, Direction& dir)
{
	if(a == 'h' || a == 'v')
	{	
		if(a == 'h')
			dir = HORIZONTAL;
		if(a == 'v')
			dir = VERTICAL;
		return true;
	}
	else
		return false;
}

bool HumanPlayer::placeShips(Board& b)
{
	char a;
	bool place;
	Direction dir;
	int r, c, counter = 0, i = 0;
	Point p;
	cout << name() << " must place " << game().nShips() - i << " ships." << endl;
	while(i < game().nShips() && counter < MAX_PLACEMENT_TRIES)
	{	
		b.display(false);		
		do
		{	
			cout << "Enter h or v for direction of " << game().shipName(i)
			 << " <length " << game().shipLength(i) << ">: ";
			cin.get(a);
			cin.ignore(1000,'\n');
			if(!dirCheck(a, dir))
				cout << "Direction must be h or v." << endl;
		}
		while(!dirCheck(a, dir));
		do
		{
			cout << "Enter row and column of leftmost cell <e.g. 3 5>: ";
			if(getLineWithTwoIntegers(r, c))
			{	
				p.r = r;
				p.c = c;
			}

			if(!game().isValid(p))
				cout << "The ship cannot be placed there." << endl;
			else
			{				
				place = b.placeShip(p, i, dir);
				if(!place)
					cout << "The ship cannot be placed there." << endl;
			}
		}
		while(!(game().isValid(p) && place));
		counter++;
		i++;
	}
	if(counter == MAX_PLACEMENT_TRIES)
		return false;
	return true;
}

Point HumanPlayer::recommendAttack()
{
	int r, c;
	Point p;
	bool check;
	do
	{
		cout << "Enter the row and column to attack <e.g. 3 5>: ";
		check = getLineWithTwoIntegers(r,c);
		if(check)
		{
			p.r = r;
			p.c = c;
		}
		else
			cout << "You must enter numbers!" << endl;
	}
	while(!check);
	return p;
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId)
{}


//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer : public Player
{
  public:
	MediocrePlayer(string nm, const Game& g)
		:Player(nm, g), m_shotHit(false), m_shipDestroyed(false), m_state(false){}
	virtual bool placeShips(Board& b);
	bool placeShipsh(Board& b, Point p, int i, Direction a);
	virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
  private:
	  Point m_p;
	  Point m_tran;
	  bool m_shotHit;
	  bool m_shipDestroyed;
	  bool m_state;
};

bool MediocrePlayer::placeShips(Board& b)
{
	b.clear();
	b.block();
	///b.display(false);
	bool a = false;
	a = placeShipsh(b, Point(), 0, VERTICAL);
	if(a == false)
	{
		a = placeShipsh(b, Point(), 0, HORIZONTAL);
	}
	b.unblock();
	return a;
}

bool MediocrePlayer::placeShipsh(Board& b, Point p, int i, Direction a)
{
	if(i == game().nShips())
		return true;
	if(p.r == game().rows())
		return false;
	if(b.placeShip(p, i, a))
	{	
		//b.display(false);
		if(p.c == game().cols() - 1)
		{
			if(!placeShipsh(b, Point(), i + 1, VERTICAL))
			{	
				if(!placeShipsh(b, Point(), i + 1, HORIZONTAL))
				{
					b.unplaceShip(p, i, a);
					if(i == 0 && a == VERTICAL)
					{	
						if(!placeShipsh(b, Point(p.r + 1, 0), i, a))
							return placeShipsh(b, Point(), i, HORIZONTAL);
						else
							return true;
					}
					else
						return placeShipsh(b, Point(p.r + 1, 0), i, a);
				}
				else
					return true;
			}
			else
				return true;
		}
		else
		{	
			if(!placeShipsh(b, Point(), i + 1, VERTICAL))
			{
				if(!placeShipsh(b, Point(), i + 1, HORIZONTAL))
				{
					b.unplaceShip(p, i, a);
					if(i == 0 && a == VERTICAL)
					{	
						if(!placeShipsh(b, Point(p.r, p.c + 1), i, a))
							return placeShipsh(b, Point(), i, HORIZONTAL);
						else
							return true;
					}
					else
						return placeShipsh(b, Point(p.r, p.c + 1), i, a);
				}
				else
					return true;
			}
			else
				return true;
		}

	}
	else
	{	
		if(p.c == game().cols() - 1)
		{
			return placeShipsh(b, Point(p.r + 1, 0), i, a);
		}
		else
			return placeShipsh(b, Point(p.r, p.c + 1), i, a);
	}
}

Point MediocrePlayer::recommendAttack()
{
	if(m_shotHit == false && m_state == false)
		return Point(randInt(10), randInt(10));
	else
	{
		if(m_state == false)
		{	
			m_tran.c = m_p.c;
			m_tran.r = m_p.r;
		}
		m_state = true;
		if(m_shipDestroyed == true)
		{	
			m_state = false;
			m_shotHit = false;
			return Point(randInt(10), randInt(10));
		}
		else
		{	
			int sector = randInt(4);
			if(sector == 0)
				return Point(m_tran.r - randInt(4) - 1, m_tran.c);
			else if(sector == 1)
				return Point(m_tran.r, m_tran.c + randInt(4) + 1);
			else if(sector == 2)
				return Point(m_tran.r + randInt(4) + 1, m_tran.c);
			else
				return Point(m_tran.r, m_tran.c - randInt(4) - 1);
		}
	}
}


void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
	m_p.r = p.r;
	m_p.c = p.c;
	m_shotHit = shotHit;
	m_shipDestroyed = shipDestroyed;
	//m_shipId = shipId;
}

//*********************************************************************
//  GoodPlayer
//*********************************************************************

class GoodPlayer : public Player
{
  public:
	GoodPlayer(string nm, const Game& g)
		:Player(nm, g), m_squareProb(game().rows(), vector<int>(game().cols(), 0)),
			m_playerboard(game().rows(), vector<char>(game().cols(), '.')), m_shotHit(false), m_shipDestroyed(false), m_state(false),
			m_deadshiplengths(0)
		{}
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
	Point probDensity(bool target);
	void createShipVec();
	void clear();
	bool placeShip(Point topOrLeft, int shipId, Direction dir);
	bool placeShiph(Point topOrLeft, int shipId, Direction dir, bool buffer);
	bool ifFired(Point p);
  private:
	  vector< vector<int>> m_squareProb;
	  vector<Ship> m_shipAlive;
	  vector< vector<char> > m_playerboard;
	  map<Point, bool> m_firedOn;
	  Point m_p;
	  Point m_tran;
	  bool m_shotHit;
	  bool m_shipDestroyed;
	  bool m_state;
	  int m_deadshiplengths;

};

void GoodPlayer::createShipVec()
{
	for(int i = 0; i < game().nShips(); i++)
	{	
		Ship a;
		a.length = game().shipLength(i);
		a.symbol = game().shipSymbol(i);
		a.name = game().shipName(i);
		m_shipAlive.push_back(a);
	}
}

void GoodPlayer::clear()
{
	vector <vector<char> > temp(game().rows(), vector<char>(game().cols(), '.'));
	m_playerboard = temp; //SIMPLER WAY TO IMPLEMENT?
}

bool GoodPlayer::placeShiph(Point topOrLeft, int shipId, Direction dir, bool buffer) //checks to see if board could be placed
	//with one square buffer(exluding diagonals)
{
	if(buffer == true && (topOrLeft.r == 0 || topOrLeft.r == game().cols() -1 || topOrLeft.c == 0 || topOrLeft.c == game().cols() - 1))
		return false;
	if(m_shipAlive[shipId].length == -1)
		return false;
	if(dir == HORIZONTAL && (topOrLeft.c + m_shipAlive[shipId].length + 1 > game().cols()))
		return false;//COULD USE ISVALID
	if(dir == VERTICAL && (topOrLeft.r + m_shipAlive[shipId].length + 1> game().rows()))
		return false;
	if(dir == HORIZONTAL)
	{	
		if(!((m_playerboard[topOrLeft.r][topOrLeft.c - 1] == '.' || m_playerboard[topOrLeft.r][topOrLeft.c - 1] == '#')
			|| (m_playerboard[topOrLeft.r][topOrLeft.c + m_shipAlive[shipId].length] == '.' || m_playerboard[topOrLeft.r][topOrLeft.c + m_shipAlive[shipId].length] == '#')))
			return false;
		for(int j = -1; j < 2; j++)
		{	
			for(int i = 0; i < m_shipAlive[shipId].length; i++)
			{
				if(j == 0 && !(m_playerboard[topOrLeft.r + j][i + topOrLeft.c] == '.'))
					return false;
				if(!(m_playerboard[topOrLeft.r + j][i + topOrLeft.c] == '#' || m_playerboard[topOrLeft.r + j][i + topOrLeft.c] == '.'))
					return false;
			}
		}
		for(int j = -1; j < 2; j++)
		{	
			for(int i = 0; i < m_shipAlive[shipId].length; i++)
			{	
				if(j == 0)
					m_playerboard[topOrLeft.r + j][i + topOrLeft.c] = m_shipAlive[shipId].symbol;
				else
					m_playerboard[topOrLeft.r + j][i + topOrLeft.c] = '#';
			}
		}
		m_playerboard[topOrLeft.r][topOrLeft.c - 1] = '#';
		m_playerboard[topOrLeft.r][topOrLeft.c + m_shipAlive[shipId].length] = '#';
	}
	else
	{
		if(!((m_playerboard[topOrLeft.r - 1][topOrLeft.c] == '.' || m_playerboard[topOrLeft.r - 1][topOrLeft.c] == '#')
			|| (m_playerboard[topOrLeft.r + m_shipAlive[shipId].length][topOrLeft.c] == '.' || m_playerboard[topOrLeft.r + m_shipAlive[shipId].length][topOrLeft.c] == '#')))
			return false;
		for(int i = -1; i < 2; i++)
		{	
			for(int j = 0; j < m_shipAlive[shipId].length; j++)
			{
				if(i == 0 && !(m_playerboard[topOrLeft.r + j][i + topOrLeft.c] == '.'))
					return false;
				if(!(m_playerboard[topOrLeft.r + j][i + topOrLeft.c] == '.' || m_playerboard[topOrLeft.r + j][i + topOrLeft.c] == '#'))
					return false;
			}
		}
		for(int i = -1; i < 2; i++)
		{	
			for(int j = 0; j < m_shipAlive[shipId].length; j++)
			{
				if(i == 0)
					m_playerboard[topOrLeft.r + j][i + topOrLeft.c] = m_shipAlive[shipId].symbol;
				else
					m_playerboard[topOrLeft.r + j][i + topOrLeft.c] = '#';
			}
		}
		m_playerboard[topOrLeft.r - 1][topOrLeft.c] = '#';
		m_playerboard[topOrLeft.r + m_shipAlive[shipId].length][topOrLeft.c] = '#';
	}
	return true;
}

bool GoodPlayer::placeShips(Board& b)//random with buffer square surrounding OKAY TO ASSUME ALWAYS BOARD 10 BY 10 FOR GOOD PLAYER?
{
	clear();
	b.clear();
	createShipVec();
	int counter;
	//b.display(false);
	for(int k = 0; k < (int) m_shipAlive.size(); k++)
	{	
		int r, c;
		r = randInt(10);
		c = randInt(10);
		Direction e;
		int d = randInt(2);
		if(d == 0)
			e = HORIZONTAL;
		else
			e = VERTICAL;
		bool a = placeShiph(Point(r, c), k, e, true);
		counter = 0;
		while(a == false && counter < MAX_PLACEMENT_TRIES)
		{	
			r = randInt(10);
			c = randInt(10);
			d = randInt(2);
			if(d == 0)
				e = HORIZONTAL;
			else
				e = VERTICAL;
			a = placeShiph(Point(r, c), k, e, true);
			counter++;
		}
		if(counter == MAX_PLACEMENT_TRIES)
			return false;
		b.placeShip(Point(r, c), k, e);
		//b.display(false);
	}
	clear();
	return true;
}

Point GoodPlayer::recommendAttack()
{
	if(m_shotHit == false && m_state == false)
		
		return probDensity(false);
	else
	{
		/*if(m_state == false)
		{	
			m_tran.c = m_p.c;
			m_tran.r = m_p.r;
		}*/
		m_state = true;
		if(m_shipDestroyed == true)
		{	
			m_state = false;
			m_shotHit = false;
			return probDensity(false);
		}
		else
			return probDensity(true);
	}
}

bool GoodPlayer::placeShip(Point topOrLeft, int shipId, Direction dir) //checks to if surviving ship can be placed on the board
	//without being placed over a missed shot; used for probDensity function
{
	if(m_shipAlive[shipId].length == -1 || topOrLeft.c < 0 || topOrLeft.r < 0)
		return false;
	if(dir == HORIZONTAL && (topOrLeft.c + m_shipAlive[shipId].length > game().cols()))
		return false;//COULD USE ISVALID
	if(dir == VERTICAL && (topOrLeft.r + m_shipAlive[shipId].length > game().rows()))
		return false;
	if(dir == HORIZONTAL)
	{	
		for(int c = 0; c < m_shipAlive[shipId].length; c++)
		{	
			if(!(m_playerboard[topOrLeft.r][c + topOrLeft.c] == '.' || m_playerboard[topOrLeft.r][c + topOrLeft.c] == 'X'))
				return false;
		}
	}
	else
	{
		for(int c = 0; c < m_shipAlive[shipId].length; c++)
		{	
			if(!(m_playerboard[c + topOrLeft.r][topOrLeft.c] == '.' || m_playerboard[c + topOrLeft.r][topOrLeft.c] == 'X'))
				return false;
		}
	}
	return true;
}

Point GoodPlayer::probDensity(bool target)
{
	int i = 0; //cols
	int j = 0; //rows
	int k = 0; //ship
	int l = 0; //ship length
	for(j = 0; j < game().rows(); j++)
	{	
		for(i = 0; i < game().cols(); i++)
			m_squareProb[j][i] = 0;
	}
	if(target == true)
	{	
		for(int m = 0; m < game().rows(); m++)
		{	
			for(int n = 0; n < game().cols(); n++)
			{	
				if(m_playerboard[m][n] == 'X')
				{	
					for(k = 0; k < (int) m_shipAlive.size(); k ++)
					{	
						for(i = -m_shipAlive[k].length + 1; i <= 0; i++)
						{
							if(placeShip(Point(m, n + i), k, HORIZONTAL))
							{
								for(l = 0; l < m_shipAlive[k].length; l++)
									m_squareProb[m][n + i + l] += 100;
							}
						}
						for(j = -m_shipAlive[k].length + 1; j <= 0; j++)			
						{
							if(placeShip(Point(m + j, n), k, VERTICAL))
							{
								for(l = 0; l < m_shipAlive[k].length; l++)
									m_squareProb[m + j + l][n] += 100;
							}
						}
					}
				}
			}
		}
	}		
	for(k = 0; k < (int) m_shipAlive.size(); k++)
	{	
		for(j = 0; j < game().rows(); j++)
		{	
			for(i = 0; i < game().cols(); i++)
			{	
				if(placeShip(Point(j,i), k, HORIZONTAL))
				{	
					for(int l = 0; l < m_shipAlive[k].length; l++)
						m_squareProb[j][i + l]++;
				}
				if(placeShip(Point(j,i),k,VERTICAL))
				{
					for(int l = 0; l < m_shipAlive[k].length; l++)
						m_squareProb[j + l][i]++;
				}
			}
		}
	}
	int sum = 0;
	for(j = 0; j < game().rows(); j++)
		for(i = 0; i < game().rows(); i++)
			sum += m_squareProb[j][i];
	for(j = 0; j < game().rows(); j++)
	{	
		for(i = 0; i < game().cols(); i++)
			if(ifFired(Point(j,i)))
				m_squareProb[j][i] = 0;
	}
	int r = 0;
	int c = 0;
	for(j = 0; j < game().rows(); j++)
	{	
		for(i = 0; i < game().cols(); i++)
			if(m_squareProb[j][i] > m_squareProb[r][c])
			{	
				r = j;
				c = i;
			}
	}
	m_firedOn[Point(r,c)] = true;
	return Point(r, c);
}//PROBABILITY DISTRIBUTION FUNCTION

bool GoodPlayer::ifFired(Point p)
{
	map<Point, bool>::iterator it;
	it = m_firedOn.find(p);
	if(it == m_firedOn.end())
		return false;
	return true;
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId)
{
	m_p.r = p.r;
	m_p.c = p.c;
	m_shotHit = shotHit;
	if(m_shotHit == false)
		m_playerboard[p.r][p.c] = 'o';
	else
		m_playerboard[p.r][p.c] = 'X';
	if(shipDestroyed == false)
		m_shipDestroyed = false;
	if(shipDestroyed == true)
	{	
		int counter = 0;
		m_deadshiplengths += m_shipAlive[shipId].length;
		for(int j = 0; j < game().rows(); j++)
		{	
			for(int i = 0; i < game().cols(); i++)
			{	
				if(m_playerboard[j][i] == 'X')
					counter ++;
			}
		}
		if(counter == m_deadshiplengths)
		{
			m_shipDestroyed = shipDestroyed;
			m_deadshiplengths = 0;
			for(int j = 0; j < game().rows(); j++)
			{	
				for(int i = 0; i < game().cols(); i++)
				{	
					if(m_playerboard[j][i] == 'X')
						m_playerboard[j][i] = 'o';
				}
			}
		}
		m_shipAlive[shipId].length = -1;
	}
}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return NULL;
    }
}
