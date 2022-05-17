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
  std::shared_ptr<Iceman> ice_man;
  int currentGameLevel{};
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir) { }
    virtual ~StudentWorld() {};

    void initIce();

    void setDisplayText();

	virtual int init() override {

    // initialize ice
    initIce();
    
    // initialize iceman
    ice_man = std::make_shared<Iceman>(this);
    ice_man->setVisible(true);

    // initialize score board
    setDisplayText();
    
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
    //return GWSTATUS_FINISHED_LEVEL;
    
    return GWSTATUS_CONTINUE_GAME;
	}

  
	virtual void cleanUp() override
	{
	}
  
  void deleteIce(const unsigned int& x, const unsigned int& y, const int& dir);
};

#endif // STUDENTWORLD_H_
