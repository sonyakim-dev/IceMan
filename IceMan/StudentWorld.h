#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <memory>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
private:
  std::shared_ptr<Ice> ice[64][64];
  Iceman* ice_man;
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir) { }
  virtual ~StudentWorld() { }

	virtual int init() override
	{
    // initialize ice
    for (int x = 0; x < 64; ++x) {
      for (int y = 0; y < 60; ++y) {
        if ((x < 30 || x > 33) || (y < 4 || y > 59)) {
          ice[x][y] = std::make_shared<Ice>(x, y);
          ice[x][y]->setVisible(true);
        }
      }
    }
    
    // initialize iceman
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
//    deleteIce(ice_man->getX(), ice_man->getY());
    
    return GWSTATUS_CONTINUE_GAME;
	}

  
	virtual void cleanUp() override
	{
    delete ice_man;
    for (int i = 0; i < 64; ++i) {
      for (int j = 0; j < 64; ++j) {
        ice[i][j].reset();
      }
    }
	}
  
  void deleteIce(const unsigned int& x, const unsigned int& y, const int& dir);
};

#endif // STUDENTWORLD_H_
