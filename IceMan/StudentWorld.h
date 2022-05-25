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
    int P = target_num_protester = std::min(15, int(2 + getLevel() * 1.5)); /// protester
    int B = std::min((int)getLevel() / 2 + 2, 9); /// boulder
    int G = std::max(5 - (int)getLevel() / 2, 2); /// gold
    
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
      
      actors.emplace_back(std::make_shared<Gold>(x, y, this, PERM));
    
      --G;
    }
    
    
    goodie_spawn_probability = getLevel() * 25 + 300;
    hardcore_spawn_probability = std::min(90, (int)getLevel() * 10 + 30);
    timeToAddProtester = std::max(25, 200 - (int)getLevel());
    
//    actors.emplace_back(std::make_shared<RegProtester>(this));
//    ++num_protester;
    
    
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
    for (const auto& actor : actors) {
      if (actor->isAlive()) {
        actor->doSomething();
      }
    }
    
    int addGoodie = rand() % goodie_spawn_probability;
    if (addGoodie == 0) {
      int sonar_or_water = rand() % 5;
      if (sonar_or_water == 0) {
        actors.emplace_back(std::make_shared<Sonar>(this));
      }
      else {
        actors.emplace_back(std::make_shared<Water>(5, 60, this));
      }
    }
    
    
    
    if (timeToAddProtester == 0) {
      int addHardcore = rand() % 101;
      if (addHardcore <= hardcore_spawn_probability) {
        actors.emplace_back(std::make_shared<HardProtester>(this));
      }
      else {
        actors.emplace_back(std::make_shared<RegProtester>(this));
      }
      ++num_protester;
      if (num_protester < target_num_protester) { /// if curr num_protester in field is less than max creatable protester num, recount time
        timeToAddProtester = std::max(25, 200 - (int)getLevel());
      }
      else timeToAddProtester = -1;
    }
    else {
      --timeToAddProtester;
    }
    
    /// deallocate dead actors
//    std::remove_if(actors.begin(), actors.end(), [](std::shared_ptr<Actor> actor) {
//      return (!actor->isAlive()) ? true : false;
//    });
    
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
    
    actors.clear(); /// clear actors vector
    
    /// reset ticks, etc
    timeToAddProtester = 0;
    goodie_spawn_probability = 0;
	}
  
  std::shared_ptr<Iceman> getIce_man() const { return ice_man; }
  std::string setPrecision(const unsigned int& val, const unsigned int& precision) const;
  void setDisplayText();
  void initIce();
  void initGold();
  void initSonar();
  void initBoulder();
  
  void digIce(const unsigned int& x, const unsigned int& y, const int& dir);
  
  void foundOil() { playSound(SOUND_FOUND_OIL); increaseScore(1000); --num_oil; }
  void foundGold() { playSound(SOUND_GOT_GOODIE); increaseScore(10); ice_man->addGold(); }
  void foundSonar() { playSound(SOUND_GOT_GOODIE); increaseScore(75); ice_man->addSonar(); }
  void foundWater() { playSound(SOUND_GOT_GOODIE); increaseScore(100); ice_man->addWater(); }
  
  void findGoodies(const int& x, const int& y); /// using sonar
  void dropGold(const int& x, const int& y); /// using gold
  void squirtWater(const int& x, const int& y, const Actor::Direction& dir); /// using water
  void bribeProtester(const int& x, const int& y); // not complete. may arguments get added
  void shoutAtIceman() {
    playSound(SOUND_PROTESTER_YELL);
    ice_man->getAnnoyed(2);
  }
  
  bool isInRange(const int& x1, const int& y1, const int& x2, const int& y2, const float& radius) const {
    return (sqrt(pow(x1-x2, 2) + pow(y1-y2, 2)) <= radius) ? true : false;
  }
  bool isOccupied(const int& x, const int& y, const float& radius) const {
    for (const auto& actor : actors) {
      if (isInRange(x, y, actor->getX(), actor->getY(), radius)) return true;
    }
    return false;
  }
  bool isIcy(const int& x, const int& y, const int& dir) const;
  bool isBouldery(const int& x, const int& y, const int& dir) const;
};

#endif // STUDENTWORLD_H_
