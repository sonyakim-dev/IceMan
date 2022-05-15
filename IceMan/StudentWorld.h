#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
private:
  Iceman* ice_man;
  Ice* ice[63][59];
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}

	virtual int init() override
	{
    for (int x = 0; x < 63; ++x) {
      for (int y = 0; y < 59; ++y) {
        ice[x][y] = new Ice(x, y, this);
        if ((x < 30 || x > 33) || (y < 4 || y > 59))
          ice[x][y]->setVisible(true);
      }
    }
    
    ice_man = new Iceman(this);
    ice_man->setVisible(true);
    
    return GWSTATUS_CONTINUE_GAME;
  }

  
  virtual int move() override
  {
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
//    decLives();
//    return GWSTATUS_PLAYER_DIED;
//    return GWSTATUS_FINISHED_LEVEL;

      ice_man->doSomething();
    
    return GWSTATUS_CONTINUE_GAME;
	}

  
	virtual void cleanUp() override
	{
	}

private:
};

#endif // STUDENTWORLD_H_
