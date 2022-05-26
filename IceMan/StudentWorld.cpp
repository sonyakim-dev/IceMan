#include "StudentWorld.h"

GameWorld* createStudentWorld(std::string assetDir)
{
	return new StudentWorld(assetDir);
}

std::string StudentWorld::setPrecision(const unsigned int& val, const unsigned int&& precision, const char&& placeholder) const {
  int digit = (val < 10) ? 1 : int(log10(val) + 1); /// prevent log10(0) error
  if (digit > precision) return std::to_string(val); /// if val is longer than the given precision, just print val
  else return std::string(precision - digit, placeholder).append(std::to_string(val));
}

void StudentWorld::setDisplayText() {
  std::string s = "Lvl: " + setPrecision(getLevel(), 2, ' ') + "  Lives: " + std::to_string(getLives()) +
      "  Hlth: " + setPrecision(ice_man->getHP()*10, 2, ' ') + "%  Wtr: " + setPrecision(ice_man->getWater(), 2, ' ') +
      "  Gld: " + setPrecision(ice_man->getGold(), 2, ' ') + "  Oil Left: " + setPrecision(num_oil, 2, ' ') +
      "  Sonar: " + setPrecision(ice_man->getSonar(), 2, ' ') + "  Scr: " + setPrecision(getScore(), 6, '0');

  setGameStatText(s);
}


void StudentWorld::initIce() {
    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 64; ++y) {
            ice[x][y] = std::make_shared<Ice>(x, y, this);
            if (((x >= 30 && x <= 33) && (y >= 4 && y <= 59)) || y >= 60) {
              ice[x][y]->setDead();
            }
        }
    }
}


void StudentWorld::digIce(const unsigned int& x, const unsigned int& y, const int& dir) {
  /// dig ice(set invisible and dead state) when iceman moves
  bool isThereIce = false; /// if there's even one ice to get deleted, set true
  
  switch (dir) {
    case KEY_PRESS_UP :
      for (int i = 0; i < 4; ++i) {
        if (ice[x + i][y + 3]->isAlive()) {
          isThereIce = true;
          ice[x + i][y + 3]->setDead();
        }
      }
      break;
    
    case KEY_PRESS_DOWN :
      for (int i = 0; i < 4; ++i) {
        if (ice[x + i][y]->isAlive()) {
          isThereIce = true;
          ice[x + i][y]->setDead();
        }
      }
      break;
      
    case KEY_PRESS_RIGHT :
      for (int i = 0; i < 4; ++i) {
        if (ice[x + 3][y + i]->isAlive()) {
          isThereIce = true;
          ice[x + 3][y + i]->setDead();
        }
      }
      break;
      
    case KEY_PRESS_LEFT :
      for (int i = 0; i < 4; ++i) {
        if (ice[x][y + i]->isAlive()) {
          isThereIce = true;
          ice[x][y + i]->setDead();
        }
      }
      break;
  }
  if (isThereIce) playSound(SOUND_DIG); /// prevent redundant sound play. even though 4 ice got deleted, play sound once
}


bool StudentWorld::isIcy(const int& x, const int& y, const int& dir) const {
  /// check is there ice right next to the given item's x, y
  /// if the given dir is right, it will check 4 ice of right side
  /// this func could also be used when a protester hit ice, turn around and go apposite side..?
  switch (dir) {
    case Actor::up :
      if (y >= 60) return true; /// prevent ice out of range
      for (int i = 0; i < 4; ++i) {
        if (ice[x + i][y + 4]->isAlive()) { return true; }
      }
      break;
      
    case Actor::down :
      if (y <= 0) return true; /// prevent ice out of range
      for (int i = 0; i < 4; ++i) {
        if (ice[x + i][y - 1]->isAlive()) { return true; }
      }
      break;
      
    case Actor::right :
      if (x >= 60) return true; /// prevent ice out of range
      for (int i = 0; i < 4; ++i) {
        if (ice[x + 4][y + i]->isAlive()) { return true; }
      }
      break;
      
    case Actor::left :
      if (x <= 0) return true; /// prevent ice out of range
      for (int i = 0; i < 4; ++i) {
        if (ice[x - 1][y + i]->isAlive()) { return true; }
      }
      break;
  }
  return false;
}

bool StudentWorld::isBouldery(const int& x, const int& y, const int& dir) const {
  for (const auto& actor : actors) {
    if (!actor->isAlive()) continue;
    
    if (typeid(Boulder) == typeid(*actor) && isInRange(x, y, actor->getX(), actor->getY(), 4.0f)) {
      switch (dir) {
        case Actor::up :
          if (actor->getY() > y) { return true; }
          break;
        case Actor::down :
          if (actor->getY() < y) { return true; }
          break;
        case Actor::right :
          if (actor->getX() > x) { return true; }
          break;
        case Actor::left :
          if (actor->getX() < x) { return true; }
          break;
      }
    }
  }
  return false;
}


void StudentWorld::initGold() {}
void StudentWorld::initSonar() {}
void StudentWorld::initBoulder() {}

void StudentWorld::dropGold(const int& x, const int& y) {
  actors.emplace_back(std::make_shared<Gold>(x, y, this, TEMP));
  ice_man->useGold();
}

void StudentWorld::squirtWater(const int& x, const int& y, const Actor::Direction& dir) {
  playSound(SOUND_PLAYER_SQUIRT);
  ice_man->useWater();
  
//  if (x < 4 || x > 56 || y < 4 || y > 60) return; /// prevent out of range
  switch (dir) {
    case Actor::up :
      for (int i = 0; i < 4; ++i) {
        if (isIcy(x, y+i, dir)) return; /// do not make squirt if there's no 4X4 space
      }
      actors.emplace_back(std::make_shared<Squirt>(x, y+4, this, dir));
      break;
      
    case Actor::down :
      for (int i = 0; i < 4; ++i) {
        if (isIcy(x, y-i, dir)) return; /// do not make squirt if there's no 4X4 space
      }
      actors.emplace_back(std::make_shared<Squirt>(x, y-4, this, dir));
      break;
      
    case Actor::right :
      for (int i = 0; i < 4; ++i) {
        if (isIcy(x+i, y, dir)) return; /// do not make squirt if there's no 4X4 space
      }
      actors.emplace_back(std::make_shared<Squirt>(x+4, y, this, dir));
      break;
      
    case Actor::left :
      for (int i = 0; i < 4; ++i) {
        if (isIcy(x-i, y, dir)) return; /// do not make squirt if there's no 4X4 space
      }
      actors.emplace_back(std::make_shared<Squirt>(x-4, y, this, dir));
      break;
  }
}

void StudentWorld::discoverGoodies(const int& x, const int& y) {
  ice_man->useSonar();
  for (const auto& actor : actors) {
    if (typeid(*actor) == typeid(Gold) || typeid(*actor) == typeid(Oil)) {
      if (isInRange(x, y, actor->getX(), actor->getY(), 12.0f)) {
        actor->setVisible(false); // THIS IS TEST: need to change to true.
      }
    }
  }
}


bool StudentWorld::bribeProtester(const int& goldX, const int& goldY) {
  for (const auto& protester: protesters) {
    if (isInRange(goldX, goldY, protester->getX(), protester->getY(), 3.0f)) {
      playSound(SOUND_PROTESTER_FOUND_GOLD);
      if (typeid(*protester) == typeid(RegProtester)) {
        increaseScore(25);
        protester->setState(LEAVE);
      }
      else {
        increaseScore(50);
        protester->setState(WAIT);
      }
      return true;
    }
  }
  return false;
}

bool StudentWorld::shootProtester(const int& waterX, const int& waterY) {
  for (const auto& protester : protesters) {
    if (isInRange(waterX, waterY, protester->getX(), protester->getY(), 3.0f)) {
      // ADD
      protester->getAnnoyed(2);
      playSound(SOUND_PROTESTER_ANNOYED);
      return true;
    }
  }
  return false;
}

bool StudentWorld::bonkProtester(const int& boulderX, const int& boulderY) {
  for (const auto& protester : protesters) {
    if (isInRange(boulderX, boulderY, protester->getX(), protester->getY(), 3.0f)) {
      // ADD
      return true;
    }
  }
  return true;
}

