#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdlib>
#include <ctime>

class StudentWorld : public GameWorld
{
private:
  std::shared_ptr<Ice> ice[64][64];
  std::shared_ptr<Iceman> ice_man;
  std::vector<std::shared_ptr<Actor>> actors;
  unsigned int num_oil {0}; /// curr num of oil left in the field
  unsigned int num_protester {0}; /// curr num of protester in the field
  unsigned int target_num_protester; /// num of protester should be created
  unsigned int hardcore_spawn_probability {0};
  unsigned int goodie_spawn_probability {0};
  /// below is a tick counter
  unsigned int timeToAddProtester {0};
  
public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir) {}
  virtual ~StudentWorld() {}

	virtual int init() override {
    std::srand((unsigned int)std::time(NULL)); /// to get random number

    /// initialize ice // Hi Sonya
    initIce();
    
    /// initialize iceman
    ice_man = std::make_shared<Iceman>(this);
    ice_man->setVisible(true);

    /// initialize score board
    setDisplayText();
    
    
    /// number of items to be created
    int L = num_oil = std::min(2 + (int)getLevel(), 21); /// oil
    int B = std::min((int)getLevel()/2 + 2, 9); /// boulder
    int G = std::max(5 - (int)getLevel()/2, 2); /// gold
    int P = target_num_protester = std::min(15, int(2 + getLevel() * 1.5)); /// protester
    
    /// make enough size for vector -> save memory and time
    actors.reserve(L + B + G + P + 10);
    
    /// initialize boulder
    while (B > 0) {
      int x = rand() % 61, y = rand() % 37 + 20;
      
      /// if x and y are in the middle aisel or if there's already other item, regenerate randome num
      if ((x >= 26 && x <= 33) && (y >= 1 && y <= 64)) continue;
      if (isOccupied(x, y, 6.0f)) continue;
      
      actors.emplace_back(std::make_shared<Boulder>(x, y, this));
      
      /// make ice invisible where the boulder is
      for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
          if (ice[x+i][y+j]->isAlive()) {
            ice[x+i][y+j]->setVisible(false);
            ice[x+i][y+j]->setDead();
          }
        }
      }
      --B;
    }
    
    /// initialize oil
    while (L > 0) {
      int x = rand() % 61, y = rand() % 57;
      
      if ((x >= 26 && x <= 33) && (y >= 1 && y <= 64)) continue;
      if (isOccupied(x, y, 6.0f)) continue;
      
      actors.emplace_back(std::make_shared<Oil>(x, y, this));
      
      --L;
    }
    
    /// initialize gold
    while (G > 0) {
      int x = rand() % 61, y = rand() % 57;
      
      if ((x >= 26 && x <= 33) && (y >= 1 && y <= 64)) continue;
      if (isOccupied(x, y, 6.0f)) continue;
      
      actors.emplace_back(std::make_shared<Gold>(x, y, this));
      actors.back()->setState(PERM); /// pickable by iceman
    
      --G;
    }
    
    
    goodie_spawn_probability = getLevel() * 25 + 300;
    timeToAddProtester = std::max(25, 200 - (int)getLevel());
    
//    actors.emplace_back(std::make_shared<RegProtester>(this));
    
    // test obj
    actors.emplace_back(std::make_shared<Sonar>(this));
    
    
    
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
    
    if (num_oil <= 0) { /// when you collected all oils on the field!
      playSound(SOUND_FINISHED_LEVEL);
      return GWSTATUS_FINISHED_LEVEL;
    }
    
    /// do something
    ice_man->doSomething();
    for (const auto& item : actors) {
      if (item->isAlive()) {
        item->doSomething();
      }
    }
    
//    int goodie = rand() % goodie_spawn_probability;
    
    /// counting ticks
    if (timeToAddProtester == 0) {
      // TEST OBJ
      actors.emplace_back(std::make_shared<RegProtester>(this));
      if (num_protester < target_num_protester) { /// if curr num_protester in field is less than max creatable protester num, recount time
        timeToAddProtester = 0;
      }
    }
    else {
      --timeToAddProtester;
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
    
    actors.clear(); /// clean actors vector
    
    /// reset counting ticks
    timeToAddProtester = 0;
    goodie_spawn_probability = 0;
	}
  
  std::shared_ptr<Iceman> getIce_man() { return ice_man; }
  void initIce();
  void initGold();
  void initSonar();
  void initBoulder();
  void initSquirt(const int& dir);
  void setDisplayText();
  void digIce(const unsigned int& x, const unsigned int& y, const int& dir);
  bool isIcy(const int& x, const int& y, const int& dir) const;
  void foundOil() { --num_oil; }
  std::string setPrecision(const unsigned int& val, const unsigned int& precision);
  
  bool isInRange(const int& x1, const int& y1, const int& x2, const int& y2, const float& radius) const {
    if (sqrt(pow((x1+2)-(x2+2), 2) + pow((y1+2)-(y2+2), 2)) <= radius) return true;
    else return false;
  }
  
  bool isOccupied(const int& x, const int& y, const float& radius) {
    /// check if there's any other actors within 6.0 radius
    for (const auto& actor : actors) {
      if (isInRange(x, y, actor->getX(), actor->getY(), radius)) return true;
    }
    return false;
  }
  
  void findGoodies() {
    for (const auto& actor : actors) {
      // FIX: this make boulder's visible state together...
      // if actor is boulder or water, continue;
      if (isInRange(ice_man->getX(), ice_man->getY(), actor->getX(), actor->getY(), 12.0f)) {
        actor->setVisible(false); // need to change to true. THIS IS TEST
      }
    }
  }
};

#endif // STUDENTWORLD_H_
