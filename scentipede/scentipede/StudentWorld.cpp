#include "StudentWorld.h"
#include <iostream>
#include <time.h>

StudentWorld::StudentWorld()
	:m_count(0), player_count(0), wd_count(0), total_count(0),
	C(0), sc_count(0), fl_count(0), cen_count(0), sp_count(0), addedInsects(false), player_dead(false)
{}

StudentWorld::~StudentWorld()
{
	// nothing to do here
}
	
unsigned int StudentWorld::randInt(int lowest, int limit)
{
  return lowest + (rand() % limit);
}

void StudentWorld::init()
{
	addingPlayer();
	addingInitMushroom();
}

int StudentWorld::move()
{
	C++;
	addingInsect();

	for( unsigned int i = 0; i < m_myobjects.size(); i++)
	{
		if(!m_myobjects[i]->isDead())
			m_myobjects[i]->doSomething();
	}

	player_dead = m_myobjects[0]->isDead();	// gets whether player is dead or not
	// Remove recently-dead Game Objects after each tick
		removeDeadGameObjects(); // “delete” dead bugs/shrooms/etc	// need to fix

	// return the proper result
	if (player_dead)		// need to fix
	{
		return GWSTATUS_PLAYER_DIED;
	}
	else
		return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp() 
{
	 // delete all pointers in the vector
	 while (!m_myobjects.empty())
	{
	int k = m_myobjects.size()-1;
	 // delete the pointer
	 delete m_myobjects[k];
	 // remove it from the list
	 m_myobjects.erase(m_myobjects.begin() + k);
	}
	 m_count = 0; player_count = 0;
	 wd_count = 0;sc_count =0; 
	 fl_count=0; cen_count =0; 
	 sp_count=0; total_count = 0;
}

GameObject* StudentWorld::getPointerToObject( int x, int y)	// returns a pointer to an item in a certain spot
	{
		if( x < 0 || x > (GARDEN_WIDTH) || y < 0 || y > GARDEN_HEIGHT)
			return NULL;
		for(std::vector<GameObject*>::iterator point = m_myobjects.begin();  point != m_myobjects.end(); point++)
		{	int x_other, y_other;
			(*point)->getLocation(x_other,y_other);
			if( x_other == x && y_other == y  )
				return *point;
		}
		return NULL;
	}

void StudentWorld::addingPlayer()
{
	m_myobjects.push_back(new PlayerShip(this, 15, 0));
	m_myobjects.front()->displayMe(true);
	player_count ++;;
	total_count++;
}

bool StudentWorld::playerPosition(int x, int y) // if on player's position return true, else false
{
	if( x == 15 && y == 0)
		return true;
	else
		return false;
}

void StudentWorld::addingInitMushroom()
{	m_count = 0;
	int numMushrooms;			// how many mushrooms to be added
	if (testParamsProvided())
	{	if( getTestParam(TEST_PARAM_STARTING_MUSHROOMS) < (GARDEN_WIDTH*GARDEN_HEIGHT)) 
			numMushrooms = getTestParam(TEST_PARAM_STARTING_MUSHROOMS);
		else
		{std::cout << "Too many mushrooms!"<< std::endl; return;}
	}
	else
		numMushrooms = 25; // default number of Mushrooms to start with

	for( int i = 0; m_count < numMushrooms; i ++)
	{
	   int y = randInt(2, GARDEN_HEIGHT-2);
		int x = randInt(1, GARDEN_WIDTH);
		if( y < 1 || y >= GARDEN_HEIGHT-1)		// cannot be placed in row 0 or row 19
			continue;
		// condition to check if its okay to add mushroom
		if( playerPosition(x,y))
			continue;
		// check if another mushroom already exist there
		if( getPointerToObject(x,y) != NULL && getPointerToObject(x,y)->getID() == IID_MUSHROOM)
			continue;
		m_myobjects.push_back(new Mushroom(this, x, y));
		m_count++;				// increase count
		total_count++;
		(m_myobjects.back())->displayMe(true);
	}
}

void StudentWorld::addingInsect()
{
	if (testParamsProvided())
	{
		if( C > 0 && C % getTestParam(TEST_PARAM_CENT_CHANCE_INDEX) == 0 && addedInsects == false) 
		{	
			addGameObject(0, 0, IID_SCENTIPEDE_SEGMENT);
			addedInsects = true;			// shows insects have been added
		}
		if( C > 0 && C % getTestParam(TEST_PARAM_SPIDER_CHANCE_INDEX) == 0 && addedInsects == false)
		{
			addGameObject(0, 0, IID_SPIDER);
			addedInsects = true;
		}
		if(C > 0 && C % getTestParam(TEST_PARAM_FLEA_CHANCE_INDEX) == 0 && addedInsects == false)
		{
			addGameObject(0, 0, IID_FLEA);
			addedInsects = true;
		}
		if( C > 0 && C % getTestParam(TEST_PARAM_SCORPION_CHANCE_INDEX) == 0 && addedInsects == false)
		{
			addGameObject(0, 0, IID_SCORPION);
			addedInsects = true;
		}
		addedInsects = false;
	}
	else
	{
		srand (time (0));
		int currentLevel = getPlayerScore()/ 500;
		int k;
		for( k = 0; k < GARDEN_WIDTH-1; k++)
		{	if( getPointerToObject(k,19) != NULL && getPointerToObject(k,19)->getID() == IID_SCENTIPEDE_SEGMENT)
				return;
		}
		if( k  >= GARDEN_WIDTH-1)
		{
			if( cen_count >= 20 || cen_count <= ((currentLevel +1)*3))
					return;
		}
		else if( cen_count == 0  && (rand () % 20) == 1)
		{
			addGameObject(0, 0, IID_SCENTIPEDE_SEGMENT);
		}
		else if(cen_count >= 1 && (rand () % (randInt(50, 300-(currentLevel*30))) == 1))
		{
			addGameObject(0, 0, IID_SCENTIPEDE_SEGMENT);
		}
		if( (sc_count + fl_count + sp_count) < (currentLevel/2 + 1))
		{	
			if((rand () % (randInt(100, 300-currentLevel*30))) == 1)
			{	
				addGameObject(0, 0, IID_FLEA);
				return;
			}
			if((rand () % (randInt(100, 350-currentLevel*30))) == 1)
			{
				addGameObject(0, 0, IID_SPIDER);
				return;
			}
			if((rand () % (randInt(100, 400-currentLevel*30))) == 1)
			{
				addGameObject(0, 0, IID_SCORPION);
				return;
			}
		}
	}
}

void StudentWorld::removeDeadGameObjects()
{
	for(unsigned int k = 0; k < m_myobjects.size(); k++)
	{	
		if( m_myobjects[k]->isDead())
		{ 
			// delete the pointer
			delete m_myobjects[k];

			// remove it from the list
			 m_myobjects.erase(m_myobjects.begin() + k );	// need to fix
		
		}
	}

}

int StudentWorld::Ctime()
{return C;}

void StudentWorld::addGameObject(int x, int y, int ID)
{
	if(ID == IID_MUSHROOM)
	{
		m_myobjects.push_back(new Mushroom(this, x, y));
		m_count++;				// increase count
		total_count++;
		(m_myobjects.back())->displayMe(true);
	}
	if(ID == IID_WATER_DROPLET)
	{
		m_myobjects.push_back(new WaterDroplet(this, x, y));
		(m_myobjects.back())->displayMe(true);		
		wd_count++;
		total_count++;
	}
	if(ID == IID_SCORPION)
	{
		m_myobjects.push_back(new Scorpion(this));
		(m_myobjects.back())->displayMe(true);
		sc_count++;
		total_count++;
	}
	if(ID == IID_SPIDER)
	{
		m_myobjects.push_back(new Spider(this));
		(m_myobjects.back())->displayMe(true);
		sp_count++;
		total_count++;
	}
	if(ID == IID_FLEA)
	{
		m_myobjects.push_back(new Flea(this));
		m_myobjects.back()->displayMe(true);
		fl_count++;
		total_count++;
	}
	if(ID == IID_SCENTIPEDE_SEGMENT)
	{
		int n = randInt(6,12);
		for( int k = n-1; k >= 0; k--)
		{
			m_myobjects.push_back(new Centipede(this,k,19));
			m_myobjects.back()->displayMe(true);
			cen_count++;
			total_count++;
		}
	}	
}