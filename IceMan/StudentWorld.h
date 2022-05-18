#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
private:
  std::shared_ptr<Ice> ice[64][64];
  std::shared_ptr<Iceman> ice_man;
  std::shared_ptr<Gold> gold;
  std::shared_ptr<Sonar> sonar;
  std::shared_ptr<Oil> oil;
  int currentGameLevel {0};
  int oil_left;
public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir) { }
  virtual ~StudentWorld() {};

	virtual int init() override {

    // initialize ice // Hi Sonya
    initIce();
    
    // initialize iceman
    ice_man = std::make_shared<Iceman>(this);
    ice_man->setVisible(true);

    // initialize score board
    setDisplayText();
    
    // number of items depending on the current game level
    oil_left = std::min(2 + currentGameLevel, 21); // barrel
    int B = std::min(currentGameLevel/2 + 2, 9); // boulder
    int G = std::max(5 - currentGameLevel/2, 2); // gold
    
    std::srand((unsigned int)std::time(NULL));
    
    initGold();
    initSonar();
    oil = std::make_shared<Oil>(rand() % 61 , rand() % 57, this); // FIX: should not appear in the middle aisle
    oil->setVisible(true);
    
    return GWSTATUS_CONTINUE_GAME;
  }

  
  virtual int move() override
  {
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    if (getLives() < 1) return GWSTATUS_PLAYER_DIED;
    
    ice_man->doSomething();
    setDisplayText();
    gold->doSomething();
    oil->doSomething();
    //return GWSTATUS_FINISHED_LEVEL;
    
    if (oil_left < 0) {
      ++currentGameLevel;
      return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
	}

  
	virtual void cleanUp() override
	{
	}
  
  void initIce();
  void initGold();
  void initSonar();
  void setDisplayText();
  std::shared_ptr<Iceman> getIce_man() { return ice_man; }
  void deleteIce(const unsigned int& x, const unsigned int& y, const int& dir);
  bool isIcy(const unsigned int& x, const unsigned int& y) const { return (ice[x][y]) ? true : false; }
  void foundOil() { oil_left; }
};

#endif // STUDENTWORLD_H_
