#include "StudentWorld.h"

GameWorld* createStudentWorld(std::string assetDir)
{
	return new StudentWorld(assetDir);
}

std::string StudentWorld::leadingZero(const int& val, const int& n) {
  int digit = (val < 10) ? 1 : int(log10(val) + 1); // prevent log10(0) error
  if (digit > n) return std::to_string(val); // if val is longer than the given n length, just print val
  else return std::string(n - digit, '0').append(std::to_string(val));
}
void StudentWorld::setDisplayText() {
    std::string s = "Lvl: " + leadingZero(getLevel(), 2) + "  Lives: " + std::to_string(getLives()) +
        "  Hlth: " + leadingZero(ice_man->getHP(), 3) + "%  Wtr: " + leadingZero(ice_man->getWater(), 2) +
        "  Gld: " + leadingZero(ice_man->getGold(), 2) + "  Oil Left: " + leadingZero(num_oil, 2) +
        "  Sonar: " + leadingZero(ice_man->getSonar(), 2) + "  Scr: " + leadingZero(getScore(), 6);

    setGameStatText(s);
}


void StudentWorld::initIce() {
    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 60; ++y) {
          ice[x][y] = std::make_shared<Ice>(x, y, this);
          if ((x >= 30 && x <= 33) && (y >= 4 && y <= 59)) {
            ice[x][y]->setVisible(false);
          }
          else {
            ice[x][y]->setVisible(true);
          }
        }
    }
}

void StudentWorld::deleteIce(const unsigned int& x, const unsigned int& y, const int& dir) {
  bool isThereIce = false;
  
  switch (dir) {
    case KEY_PRESS_UP :
      for (int i = 0; i < 4; ++i) {
        if (ice[x + i][y + 3]) {
          isThereIce = true;
          ice[x + i][y + 3]->setVisible(false);
        }
      }
      break;
      
    case KEY_PRESS_DOWN :
      for (int i = 0; i < 4; ++i) {
        if (ice[x + i][y]) {
          isThereIce = true;
          ice[x + i][y]->setVisible(false);
        }
      }
      break;
      
    case KEY_PRESS_RIGHT :
      for (int i = 0; i < 4; ++i) {
        if (ice[x + 3][y + i]) {
          isThereIce = true;
          ice[x + 3][y + i]->setVisible(false);
        }
      }
      break;
      
    case KEY_PRESS_LEFT :
      for (int i = 0; i < 4; ++i) {
        if (ice[x][y + i]) {
          isThereIce = true;
          ice[x][y + i]->setVisible(false);
        }
      }
      break;
  }
  if (isThereIce) playSound(SOUND_DIG);
}


void StudentWorld::initGold() { //Hi Chris
  std::make_shared<Gold>(rand() % 61 , rand() % 57, this); // FIX: should not appear in the middle aisle
}

void StudentWorld::initSonar() {
  std::make_shared<Sonar>(rand() % 61 , rand() % 57, this); // FIX: should not appear in the middle aisle
}

void StudentWorld::initBoulder() {
//  while (B > 0) {
//    int x = rand() % 61, y = rand() % 37 + 20;
//    
////      if (!isIcy(x, y)) continue;
//    
//    
//    actors.emplace_back(std::make_shared<Boulder>(x, y, this));
//
//    for (int i = 0; i < 4; ++i) {
//      for (int j = 0; j < 4; ++j) {
//        if (isIcy(x+i, y+j)) {
//          ice[x+i][y+j].reset();
//        }
//      }
//    }
//    
//    --B;
//  }
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
