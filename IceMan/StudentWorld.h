#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
private:
  std::shared_ptr<Ice> ice[64][64];
  std::shared_ptr<Iceman> ice_man;
  std::vector<std::shared_ptr<Actor>> actors;
  int currentGameLevel {0};
  int num_oil {0};
//  int timeToAddProtester {0};
//  int timeToStayProtester {0};
  
public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir) {}
  virtual ~StudentWorld() {}

	virtual int init() override {
    std::srand((unsigned int)std::time(NULL));

    // initialize ice // Hi Sonya
    initIce();
    
    // initialize iceman
    ice_man = std::make_shared<Iceman>(this);
    ice_man->setVisible(true);

    // initialize score board
    setDisplayText();
    
    // number of items depending on the current game level
    int L = num_oil = std::min(2 + currentGameLevel, 21); // oil
    int B = std::min(currentGameLevel/2 + 2, 9); // boulder
    int G = std::max(5 - currentGameLevel/2, 2); // gold
    
    
    // initialize goodies
    actors.reserve(L + B + G + 10);
    
    // initialize boulder ->>>> will put this in initBoulder() later
    while (B > 0) {
      int x = rand() % 61, y = rand() % 37 + 20;
      if ((x >= 26 && x <= 33) && (y >= 4 && y <= 64)) continue;
      if (isOccupied(x, y)) continue;
      
      
      actors.emplace_back(std::make_shared<Boulder>(x, y, this));
      
      // make ice invisible where the boulder is
      for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
          if (isIcy(x+i, y+j)) {
            ice[x+i][y+j]->setVisible(false);
          }
        }
      }
      
      --B;
    }
    
    // initialize oil
    while (L > 0) {
      int x = rand() % 61, y = rand() % 57;
      
      if ((x >= 26 && x <= 33) && (y >= 4 && y <= 64)) continue;
      if (isOccupied(x, y)) continue;
      
      actors.emplace_back(std::make_shared<Oil>(x, y, this));
      
      --L;
    }
    
    while (G > 0) {
      int x = rand() % 61, y = rand() % 57;
      
      if ((x >= 26 && x <= 33) && (y >= 4 && y <= 64)) continue;
      if (isOccupied(x, y)) continue;
      
      actors.emplace_back(std::make_shared<Gold>(x, y, this));
    
      --G;
    }
    
//    actors.emplace_back(std::make_shared<Sonar>(rand() % 61 , rand() % 57, this));
    
    return GWSTATUS_CONTINUE_GAME;
  }

  
  virtual int move() override
  {
    setDisplayText();
    
    if (ice_man->getHP() <= 0) {
      decLives();
      playSound(SOUND_PLAYER_GIVE_UP);
      return GWSTATUS_PLAYER_DIED;
    }
    
    if (num_oil <= 0) {
      ++currentGameLevel;
      return GWSTATUS_FINISHED_LEVEL;
    }
    
    ice_man->doSomething();
    for (auto item : actors) {
      item->doSomething();
    }
    
    
    return GWSTATUS_CONTINUE_GAME;
	}

  
	virtual void cleanUp() override
	{
    ice_man.reset();
    
    for (int i = 0; i < 64; ++i) {
      for (int j = 0; j < 64; ++j) {
        ice[i][j].reset();
      }
    }
    for (auto actor : actors) {
      actor.reset();
    }
    
	}
  
  std::shared_ptr<Iceman> getIce_man() { return ice_man; }
  void initIce();
  void initGold();
  void initSonar();
  void initBoulder();
  void setDisplayText();
  void deleteIce(const unsigned int& x, const unsigned int& y, const int& dir);
  bool isIcy(const unsigned int& x, const unsigned int& y) const { return (ice[x][y]->isVisible()) ? true : false; }
  void foundOil() { --num_oil; }
  std::string leadingZero(const int& val, const int& n);
  
  bool isInRange(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const float& radius) const {
    if (sqrt(pow((x1+2)-(x2+2), 2) + pow((y1+2)-(y2+2), 2)) <= radius) return true;
    else return false;
  }
  bool isOccupied(const unsigned int& x, const unsigned int& y) {
    int x2 = x; int y2 = y;
    for (auto actor : actors) {
      if (isInRange(x2, y2, actor->getX(), actor->getY(), 6.0)) return true;
    }
    return false;
  }
};

#endif // STUDENTWORLD_H_
