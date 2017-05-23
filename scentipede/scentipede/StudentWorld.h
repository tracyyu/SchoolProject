#ifndef _StudentWorld_H_
#define _StudentWorld_H_

#include "GameWorld.h"
#include <vector>
#include <cstdlib>
#include "actor.h"

class StudentWorld : public GameWorld
{
public:

	StudentWorld();	

	virtual ~StudentWorld();
	
	unsigned int randInt(int lowest, int limit);

	virtual void init();

	virtual int move();

	virtual void cleanUp(); 

	GameObject* getPointerToObject( int x, int y);	

	virtual void addingPlayer();

	virtual bool playerPosition(int x, int y);

	virtual void addingInitMushroom();

	virtual void addingInsect();

	virtual void removeDeadGameObjects();

	int Ctime();

	// add a new gameobject to the garden
    void addGameObject(int x, int y, int ID);

private:
	std::vector<GameObject*> m_myobjects;	// creates an array of objects like, mushroom, insects, ....
	int m_count;							// keeps count of the total number of mushrooms
	int player_count;
	int wd_count;							
	int total_count;
	int sc_count;							// keeps count of the total number of scorpion
	int fl_count;							// keeps count of the total number of fleas
	int cen_count;							// keeps count of the total number of centipede segments
	int sp_count;							// keeps count of the total number of spiders
	int C;
	bool addedInsects;
	bool player_dead;
};

#endif // #ifndef _StudentWorld_H_
