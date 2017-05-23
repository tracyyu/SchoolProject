#ifndef _ACTOR_H_

#define _ACTOR_H_

#include "GraphObj.h"
#include "GameConstants.h"

#include "GameWorld.h"

class StudentWorld;

// students - add your code here!

class GameObject: public GraphObject
{
	public:
		GameObject(StudentWorld* ptr, int imageID, int x, int y);
		virtual ~GameObject(){};
		virtual void doSomething() = 0;
		bool isDead() const; 
		void setDead();
		virtual void hit(bool kill = false, bool points = true) = 0;
		StudentWorld* getMyWorld();
		 bool canRest();
		 void setRest(bool choice);
	private:
		bool rest;
		bool m_dead;
		int m_x;
		int m_y;
		StudentWorld* world;
};

class Insect: public GameObject
{
  public:
      // constructor
      // horizDir and vertDir specify the initial horizontal and vertical
      // movement direction, if any, for this insect.
      // pointsForKill specifies how many points are earned by the player for
      // killing this insect.
    Insect(StudentWorld* sw, int x, int y, char horizDir, char vertDir, int myID, int pointsForKill);
	~Insect(){};
	void hit(bool kill = false, bool points = true) = 0;
    virtual void doSomething() = 0;

      // get/set the insect's current horizontal/vertical direction of movement
    char getHorizDir() const;
    char getVertDir() const;
    void setHorizDir(char d);
    void setVertDir(char d);

      // If the player is in the specified square, return a pointer to it,
      // otherwise NULL.
    GameObject* getContentsOfSquareOnlyPlayer(int x, int y);

      // If a mushroom is in the specified square, return a pointer to it,
      // otherwise NULL.
    GameObject* getContentsOfSquareOnlyShroom(int x, int y);

	GameObject* getContentsOfSquareOnlyCentipedeSegment(int x, int y);

      // attempt to move to the specified x,y location, and if this would
      // move into the player, then kill the player GameObject* 
    void moveAndCheckForPlayer(int x, int y);

      // introduce a mushroom into the specified location with a 1 in chance
      // probability (e.g., if chance is 20, then 5% of the time this function
      // would introduce a mushroom, and 95% of the time, nothing would happen)
    void dropShroomWithChance(int x, int y, int chance);

  private:
	  int m_points;
	  char m_horizon;
	  char m_vertical;
};

class Centipede: public Insect
{
public:
    Centipede (StudentWorld* sw,int x, int y);
	~Centipede(){};
	virtual void hit(bool kill = false, bool points = true);	
	void Check(int x, int y);
    virtual void doSomething();
	bool isPoison();
private:
	bool poison;
};

class Spider: public Insect
{
  public:
    Spider(StudentWorld* sw);
	~Spider(){};
	virtual void hit(bool kill = false, bool points = true);
	int chance(int x1 = 0, int x2 = GARDEN_WIDTH-1); 
	void moveUp(int x, int y);
	void moveDown(int x, int y);
    virtual void doSomething();
private:
	int R;
	int xint;
};

class Scorpion: public Insect
{
  public:
    Scorpion(StudentWorld* sw);
	~Scorpion(){};
	virtual void hit(bool kill = false, bool points = true);
    virtual void doSomething();
};

class Flea: public Insect
{
  public:
    Flea(StudentWorld* sw);
	~Flea(){};
	virtual void hit(bool kill = false, bool points = true);
    virtual void doSomething();
};


class PlayerShip : public GameObject
{
	public:
		PlayerShip(StudentWorld* ptr, int x, int y);
		virtual ~PlayerShip();
		virtual void hit(bool kill = false, bool points = true);
		virtual void doSomething();
	private:
};

class GameObjectWithLives: public GameObject
{
  public:

      // constructor.  Specify coordinates, starting number of lives, and ID/type
    GameObjectWithLives(StudentWorld* sw, int x, int y, unsigned int numLives, int type);
	virtual ~GameObjectWithLives(){};
    virtual void doSomething() = 0;
	void hit(bool kill = false, bool points = true) = 0;

      // get/set/decrement how many lives this gameobject has left
    unsigned int getLives() const;
    void setLives(unsigned int lives);
    void decLives();
  private:
	int m_lives;
};

class Mushroom: public GameObjectWithLives
{
  public:
    Mushroom(StudentWorld* sw, int x, int y);
	void hit(bool kill = false, bool points = true);
	virtual ~Mushroom();
    void doSomething();
};

class WaterDroplet: public GameObjectWithLives
{
  public:
    WaterDroplet(StudentWorld* sw, int x, int y);
	virtual ~WaterDroplet();
    virtual void doSomething();
	virtual void hit( bool kill = false, bool points = true);
};


#endif // #ifndef _ACTOR_H_
