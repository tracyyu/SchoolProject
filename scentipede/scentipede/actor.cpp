
#include "actor.h"
#include <time.h>
#include "StudentWorld.h"
#include "GameController.h"

// students - add your code here!

GameObject::GameObject(StudentWorld* ptr, int imageID, int x, int y)
	:GraphObject(imageID), m_dead(false) 
{world = ptr; setInitialLocation(x,y);}

bool GameObject::isDead() const
{return m_dead;}

void GameObject::setDead()
{m_dead = true;}

StudentWorld* GameObject::getMyWorld()
{return world;}

bool GameObject::canRest()
{
	return rest;
}

void GameObject::setRest(bool choice)
{
	rest = choice;
}

Insect::Insect(StudentWorld* sw, int x, int y, char horizDir, char vertDir, int myID, int pointsForKill)
	:GameObject(sw, myID, x, y) , m_points(pointsForKill), m_horizon(horizDir), m_vertical(vertDir) 
{}

      // get/set the insect's current horizontal/vertical direction of movement
char Insect::getHorizDir() const
{return m_horizon;}

char Insect::getVertDir() const
{return m_vertical;}


void Insect::setHorizDir(char d)
{m_horizon = d;}

void Insect::setVertDir(char d)
{m_vertical = d;}

      // If the player is in the specified square, return a pointer to it,
      // otherwise NULL.
GameObject* Insect::getContentsOfSquareOnlyPlayer(int x, int y)
{
	if( getMyWorld()->getPointerToObject(x,y) != NULL && getMyWorld()->getPointerToObject(x,y)->getID() == IID_PLAYER)
		return getMyWorld()->getPointerToObject(x,y);
	else 
		return NULL;
}

      // If a mushroom is in the specified square, return a pointer to it,
      // otherwise NULL.
GameObject* Insect::getContentsOfSquareOnlyShroom(int x, int y)
{
	if( getMyWorld()->getPointerToObject(x,y) != NULL && ( getMyWorld()->getPointerToObject(x,y)->getID() == IID_MUSHROOM || getMyWorld()->getPointerToObject(x,y)->getID() == IID_POISON_MUSHROOM ))
		return getMyWorld()->getPointerToObject(x,y);
	else 
		return NULL;
}

GameObject* Insect::getContentsOfSquareOnlyCentipedeSegment(int x, int y)
{
	if( getMyWorld()->getPointerToObject(x,y) != NULL && getMyWorld()->getPointerToObject(x,y)->getID() == IID_SCENTIPEDE_SEGMENT)
		return getMyWorld()->getPointerToObject(x,y);
	else 
		return NULL;
}
      // attempt to move to the specified x,y location, and if this would
      // move into the player, then kill the player 
void Insect::moveAndCheckForPlayer(int x, int y)
{
	if(getContentsOfSquareOnlyPlayer(x,y) != NULL)
	{	getContentsOfSquareOnlyPlayer(x,y)->hit(true,true);
	}
}

      // introduce a mushroom into the specified location with a 1 in chance
      // probability (e.g., if chance is 20, then 5% of the time this function
      // would introduce a mushroom, and 95% of the time, nothing would happen)
void Insect::dropShroomWithChance(int x, int y, int chance)
{
	double probability = 1/ chance;
	// add mushroom by this probability
	srand (time (0));
	if((rand () % 100) < (probability*100) )  
		getMyWorld()->addGameObject(x, y, IID_MUSHROOM);;
}

Centipede::Centipede(StudentWorld* sw,int x, int y)
	: Insect(sw, x, y, '6', '8', IID_SCENTIPEDE_SEGMENT, 10), poison(false)
{}


void Centipede::hit(bool kill, bool points)
{
	if(kill == true && points == true)
	{
		getMyWorld()->getPlayerScore();
		getMyWorld()->increaseScore(10);
		setDead();
		displayMe(false);
	}
	else if( kill == false && points == false)
	{
		setDead();		
		displayMe(false);
	}
}

void Centipede::Check(int x, int y)
{
	if( getMyWorld()->getPointerToObject(x,y) != NULL && getMyWorld()->getPointerToObject(x,y)->getID() == IID_WATER_DROPLET && y < GARDEN_HEIGHT)
	{
		hit(true, true);
		srand(0);
		if((rand () % 100) < 33)
		{
			getMyWorld()->addGameObject(x, y, IID_MUSHROOM);
		}
	}
}

void Centipede::doSomething()
{
	int x,y;
	getLocation(x,y);
	if( getMyWorld()->getPointerToObject(x,y) != NULL && getMyWorld()->getPointerToObject(x,y)->getID() == IID_WATER_DROPLET && y < GARDEN_HEIGHT)
	{
		hit(true, true);
		srand(0);
		if((rand () % 100) < 33)
		{
		getMyWorld()->addGameObject(x, y, IID_MUSHROOM);
		}
	}
	if(poison == true && y == 0)
	{
		poison = false;
	}
	if( poison == true && y > 0)
	{
		moveTo(x, y-1);
		getLocation(x,y);
		Check(x,y);
		moveAndCheckForPlayer(x, y);
		if( getContentsOfSquareOnlyShroom(x, y) != NULL)
			getContentsOfSquareOnlyShroom(x, y)->hit(true, false);
		if( getContentsOfSquareOnlyCentipedeSegment(x, y) != NULL)
			getContentsOfSquareOnlyCentipedeSegment(x, y)->hit(true,false);
		return;
	}
	else if( poison == false)
	{
		int curx, cury;
		int newx, newy;		// create a target square
		getLocation(curx, cury);
		if( getHorizDir() =='6')
		{	newx= curx+1; newy = cury;}
		else if( getHorizDir() == '4')
		{	newx = curx -1; newy = cury;}
		if( newx < 0)
			setHorizDir('6');
		else if( newx == GARDEN_WIDTH)
			setHorizDir('4');
		else if( newx >= 0 && newx < GARDEN_WIDTH)
		{
				if(getContentsOfSquareOnlyCentipedeSegment(newx,newy) == NULL &&  getContentsOfSquareOnlyShroom(newx, newy) == NULL && getContentsOfSquareOnlyPlayer(newx, newy) == NULL)
				{	moveTo(newx,newy); Check(newx, newy); return;} 
				else if( getContentsOfSquareOnlyPlayer(newx, newy) != NULL)
				{	moveAndCheckForPlayer(newx, newy); return;}
				else if(getContentsOfSquareOnlyShroom(newx, newy)!= NULL && getContentsOfSquareOnlyShroom(newx, newy)->getID() == IID_MUSHROOM) 
				{;}
				else if( getContentsOfSquareOnlyCentipedeSegment(newx, newy) != NULL && getContentsOfSquareOnlyCentipedeSegment(newx, newy)->getID() == IID_SCENTIPEDE_SEGMENT)
				{;}
			if(getContentsOfSquareOnlyShroom(newx, newy) != NULL && getContentsOfSquareOnlyShroom(newx, newy)->getID() == IID_POISON_MUSHROOM)
				{poison = true; setVertDir('8');}
		}
		getLocation(curx,cury);
		if(getVertDir() == '2')
		{newx = curx; newy = cury+1;}
		else if(getVertDir()=='8')
		{newx = curx; newy = cury-1;}
		if( newy < 0)
		{	setVertDir('2'); newy = 1;}
		else if( newy == GARDEN_HEIGHT)
		{ setVertDir('8'); newy = GARDEN_HEIGHT-2;}
		else 
		{
			if(getContentsOfSquareOnlyCentipedeSegment(newx,newy) == NULL &&  getContentsOfSquareOnlyShroom(newx, newy) == NULL && getContentsOfSquareOnlyPlayer(newx, newy) == NULL)
			{	moveTo(newx,newy);Check(newx, newy); return;} 
			else if( getContentsOfSquareOnlyPlayer(newx, newy) != NULL)
			{	moveAndCheckForPlayer(newx, newy); return;}
			else if(getContentsOfSquareOnlyShroom(newx, newy)!= NULL && getContentsOfSquareOnlyShroom(newx, newy)->getID() == IID_MUSHROOM) 
			{;}
			else if( getContentsOfSquareOnlyCentipedeSegment(newx, newy) != NULL && getContentsOfSquareOnlyCentipedeSegment(newx, newy)->getID() == IID_SCENTIPEDE_SEGMENT)
			{;}
			if(getContentsOfSquareOnlyShroom(newx, newy) != NULL && getContentsOfSquareOnlyShroom(newx, newy)->getID() == IID_POISON_MUSHROOM)
			{poison = true; setVertDir('8'); moveTo(newx, newy); doSomething();}

			if(getMyWorld()->getPointerToObject(newx, newy) != NULL && (getMyWorld()->getPointerToObject(newx, newy)->getID() == IID_POISON_MUSHROOM || getMyWorld()->getPointerToObject(newx, newy)->getID() == IID_POISON_MUSHROOM 
				|| getMyWorld()->getPointerToObject(newx,newy)->getID() == IID_SCENTIPEDE_SEGMENT || getMyWorld()->getPointerToObject(newx, newy)->getID() == IID_PLAYER))
			{	getMyWorld()->getPointerToObject(newx,newy)->hit(true, false);}

			moveTo(newx,newy);
			Check(newx, newy);
			return;
		}
	}
}

bool Centipede::isPoison()
{return poison;}

Spider::Spider(StudentWorld* sw)
	:Insect(sw, chance(0, GARDEN_WIDTH-1), getMyWorld()->randInt(5,19), '0', '8', IID_SPIDER, 20)
{	R = getMyWorld()->randInt(1,5);}

void Spider::hit(bool kill, bool points)
{
	if( kill == true)
	{
		setDead();
		displayMe(false);
	}
	if( points == true)
		getMyWorld()->increaseScore(20);
}

int Spider::chance(int x1, int x2)
{
	srand (time (0));
	((rand() % 100) < 50 ) ? xint = 0 : xint= GARDEN_WIDTH-1;
	return xint;
}

void Spider::moveUp(int x, int y)
{
	if( xint == 0)
	{
		if(getMyWorld()->getPointerToObject(x,y) != NULL && getMyWorld()->getPointerToObject(x,y)->getID() == IID_WATER_DROPLET)
			hit(true, true);
		moveAndCheckForPlayer(x+1, y+1);
		if( getContentsOfSquareOnlyShroom(x+1, y+1) != NULL)
			getContentsOfSquareOnlyShroom(x+1, y+1)->hit(true,false);
		moveTo(x+1, y+1);
		if(x+1 >= GARDEN_WIDTH)
		{
			hit(true, false);
		}
		R--;
	}
	if( xint == GARDEN_WIDTH-1)
	{
		if(getMyWorld()->getPointerToObject(x,y) != NULL && getMyWorld()->getPointerToObject(x,y)->getID() == IID_WATER_DROPLET )
			hit( true, true);
		moveAndCheckForPlayer(x-1, y +1);
		if( getContentsOfSquareOnlyShroom(x-1, y+1) != NULL)
			getContentsOfSquareOnlyShroom(x-1, y+1)->hit(true,false);
		moveTo(x-1, y+1);
		if( x-1 <= -1)
		{
			hit(true, false);
		}
		R--;
	}
}

void Spider::moveDown(int x, int y)
{
	if( xint == 0)
	{
		if(getMyWorld()->getPointerToObject(x,y) != NULL && (getMyWorld()->getPointerToObject(x,y)->getID() == IID_WATER_DROPLET ))
			hit( true, true);
		moveAndCheckForPlayer(x+1, y-1);
		if( getContentsOfSquareOnlyShroom(x+1, y-1) != NULL)
			getContentsOfSquareOnlyShroom(x+1, y-1)->hit(true, false);
		moveTo(x+1, y-1);
		if( x+1 <= -1 || x+1 >= GARDEN_WIDTH)
		{	
			hit(true, false);
		}
		R--;
	}
	if( xint == GARDEN_WIDTH-1)
	{
		if(getMyWorld()->getPointerToObject(x,y) != NULL && getMyWorld()->getPointerToObject(x,y)->getID() == IID_WATER_DROPLET )
			hit( true, true);
		moveAndCheckForPlayer(x-1, y -1);
		if( getContentsOfSquareOnlyShroom(x-1, y-1) != NULL)
			getContentsOfSquareOnlyShroom(x-1, y-1)->hit(true, false);
		moveTo(x-1, y-1);
		if( x-1 <= -1 || x-1 >= GARDEN_WIDTH)
		{
			hit(true, false);
		}
		R--;
	}
}

void Spider::doSomething()
{
	if( canRest() == true)
	{
		setRest(false);
		return;
	}
	int x,y;
	getLocation(x,y);
	getLocation(x,y);
	char c = getVertDir();
	if( (R == 0 &&  c == '8') || y <= 0)
	{	
		R = getMyWorld()->randInt(1,y-1);
		setVertDir('2');
	}
	if((R == 0 && c == '2') || y == GARDEN_HEIGHT)
	{
		R = getMyWorld()->randInt(1,GARDEN_HEIGHT-y-1);
		setVertDir('8');
	}
	if( c == '8')
		moveDown(x, y);
	else if( c == '2')
		moveUp(x, y);
	setRest(true);
}


Scorpion:: Scorpion(StudentWorld* sw)
	:Insect(sw, 0, getMyWorld()->randInt(4, GARDEN_HEIGHT-1) , '6', '0', IID_SCORPION, 25)
{}

void Scorpion::hit(bool kill, bool points)
{
	if(kill == true)
	{
		setDead();
		displayMe(false);
	}
	if( points == true)
		getMyWorld()->increaseScore(25);
}

void Scorpion::doSomething()
{
	int x, y;
	getLocation(x,y);
	if(getContentsOfSquareOnlyShroom(x, y) != NULL)
	{
		srand (time (0));
		if((rand () % 100) < 33 )  
		{	//convert Mushroom to posion
			if( getContentsOfSquareOnlyShroom(x, y)->getID() == IID_MUSHROOM)
				getContentsOfSquareOnlyShroom(x, y)->setID(IID_POISON_MUSHROOM);
		}
	}
	else
		;
	if(getMyWorld()->getPointerToObject(x,y) != NULL && getMyWorld()->getPointerToObject(x,y)->getID() == IID_WATER_DROPLET )
		hit( true, true);
	moveTo(x+1, y);
	moveAndCheckForPlayer(x+1, y);
	if( x+1 >= GARDEN_WIDTH)
	{
		hit(true, false);
	}
}


Flea::Flea(StudentWorld* sw)
	:Insect(sw, getMyWorld()->randInt(0, GARDEN_WIDTH) , GARDEN_HEIGHT-1 , '0', '8', IID_FLEA, 50)
{}

void Flea::hit(bool kill, bool points)
{
	if( kill == true)
	{
		setDead();
		displayMe(false);
	}
	else
		;
	if( points == true)
		getMyWorld()->increaseScore(50);
	else
		;
}
		// take action if this game object is hit (e.g., by a water cannon or by
      // running into another gameobject)
      // If kill is true, this hit outright kills the gameobject; otherwise,
      // it just does normal damage
      // If points is true, give the player points for the hit (e.g., because
      // the player initiated the hit)

void Flea::doSomething()
{
	if(!isDead())
	{
		int x, y;
		getLocation(x,y);
		if( getContentsOfSquareOnlyShroom(x, y) == NULL && y > 0 && y < GARDEN_HEIGHT)
			dropShroomWithChance(x, y, 25);
		if(getMyWorld()->getPointerToObject(x,y)!= NULL && getMyWorld()->getPointerToObject(x,y)->getID() == IID_WATER_DROPLET) // gets hit by a waterdroplet
			hit(true, true);
		if( y - 1 == -1)
			hit(true, false);
		else
		{
			moveTo( x, y -1);
			moveAndCheckForPlayer(x, y -1);
		}
	}
}

PlayerShip::PlayerShip(StudentWorld* ptr, int x, int y)
	:GameObject(ptr, IID_PLAYER, x , y)
{}


PlayerShip::~PlayerShip()
{}

void PlayerShip::hit(bool kill, bool points)
{
	if( kill == true)
	{	
		setDead();
		displayMe(false);
	}
		
}

void PlayerShip::doSomething()
{
	StudentWorld *sw = getMyWorld(); // get the world
									// in which the Player is located
	int ch;
	int x, y;
	if (sw->getKey(ch))
	{
		switch (ch)
		{
			case KEY_PRESS_LEFT_ARROW:	
				getLocation(x,y);		// must get the player pointer
				if( x-1 < 0 || x-1 >= GARDEN_WIDTH)
					break;
				if( getMyWorld()->getPointerToObject(x-1,y) != NULL && getMyWorld()->getPointerToObject(x-1,y)->getID() == IID_MUSHROOM)
					break;
				moveTo(x-1, y);
				break;
			case KEY_PRESS_RIGHT_ARROW:
				getLocation(x,y);		// must get the player pointer
				if( x+1 < 0 || x+1 >= GARDEN_WIDTH)
					break;
				if(getMyWorld()->getPointerToObject(x+1,y) != NULL && getMyWorld()->getPointerToObject(x+1,y)->getID() == IID_MUSHROOM)
					break;
				moveTo(x+1, y);
				break;
			case KEY_PRESS_UP_ARROW:
				getLocation(x,y);		// must get the player pointer
				if( y+1 < 0 || y+1 > 3)
					break;
				if( getMyWorld()->getPointerToObject(x,y+1) != NULL && getMyWorld()->getPointerToObject(x,y+1)->getID() == IID_MUSHROOM)
					break;
				moveTo(x, y+1);
				break;
			case KEY_PRESS_DOWN_ARROW:
				getLocation(x,y);		// must get the player pointer
				if( y-1 < 0 || y-1 > 3)
					break;
				if( getMyWorld()->getPointerToObject(x,y-1) != NULL && getMyWorld()->getPointerToObject(x,y-1)->getID() == IID_MUSHROOM)
					break;
				moveTo(x, y-1);
				break;
			case KEY_PRESS_SPACE:
				if( canRest() == true)
				{
					setRest(false);
					break;
				}
				getLocation(x,y);		// get player position
				if(y+1 >= GARDEN_HEIGHT)
					break;
				// must add a code that fires every other time
				int dropletX = x;
				int dropletY = y+1;

				while( dropletY < GARDEN_HEIGHT && dropletX < GARDEN_WIDTH)
				{
					GameObject* object = getMyWorld()->getPointerToObject(dropletX, dropletY);
					if( object != NULL)
					{	if( object->getID() == IID_MUSHROOM || object->getID() == IID_FLEA || object->getID() == IID_SCORPION || object->getID() == IID_SPIDER || object->getID() == IID_SCENTIPEDE_SEGMENT)
						{	object->hit(true,true);// ATTACK THE OBJECt
							break;
						}
					}
					else
					{
						if( object == NULL)
						{
							getMyWorld()->addGameObject(dropletX, dropletY, IID_WATER_DROPLET);
						}
					}
					dropletY++;	
				}
				setRest(true);
				break;
		}
	}
}


GameObjectWithLives::GameObjectWithLives(StudentWorld* sw, int x, int y, unsigned int numLives, int type)
	:GameObject(sw, type, x, y), m_lives(numLives)
{}

unsigned int GameObjectWithLives::getLives() const
{return m_lives;}

void GameObjectWithLives::setLives(unsigned int lives)
{m_lives = lives;}

void GameObjectWithLives::decLives()
{m_lives--;}

WaterDroplet::WaterDroplet(StudentWorld* ptr, int x, int y)
	:GameObjectWithLives(ptr, x, y, 2, IID_WATER_DROPLET)
{
	//setInitialLocation(x, y);
	//setLives(2);
}

WaterDroplet:: ~WaterDroplet()
{}

void WaterDroplet::doSomething()
{
	decLives(); 
	if( getLives() == 0)
	{	setDead();}
	if(isDead())
	{
		setDead();
		displayMe(false);
	}
}

void WaterDroplet::hit( bool kill, bool points)
{}

Mushroom::Mushroom(StudentWorld* ptr, int x, int y) 			// initialized to 4 lives!!!!!v nmhb  bnhgvf bnhg 
	:GameObjectWithLives(ptr, x, y, 4, IID_MUSHROOM)
{}					

Mushroom::~Mushroom()
{}

void Mushroom::doSomething()
{}

void Mushroom::hit( bool kill, bool points)
{
	if( kill == true && points == false)
		setDead();
	if(kill == true && getLives() != 0)
	{
		decLives();
		setBrightness(getBrightness()-0.25);
	}
	if(kill == true && getLives() == 0)
		setDead();
	if( points == true)
	{
		getMyWorld()->getPlayerScore();
		getMyWorld()->increaseScore(1);
	}
}
