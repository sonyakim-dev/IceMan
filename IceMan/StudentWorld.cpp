#include "StudentWorld.h"

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

void StudentWorld::setDisplayText() {
    int health = ice_man->getHP(10);

    string s = "Lvl: " + to_string(getLevel()) + " Lives: " + to_string(getLives()) + " Hlth: " + to_string(health) + " Wtr: " +
        to_string(ice_man->getWater()) + " Gld: " + to_string(ice_man->getGold()) + " Oil Left: " + to_string(oil_left) + "  Sonar: " + to_string(ice_man->getSonars()) +
        " Scr: " + to_string(getScore());

    setGameStatText(s);
}


void StudentWorld::initIce() {
    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 60; ++y) {
            if ((x < 30 || x > 33) || (y < 4 || y > 59)) {
                ice[x][y] = std::make_shared<Ice>(x, y, this);
                ice[x][y]->setVisible(true);
            }
        }
    }
}

void StudentWorld::initGold() { //Hi Chris
  gold = std::make_shared<Gold>(rand() % 61 , rand() % 57, this); // FIX: should not appear in the middle aisle
  gold->setVisible(true);
}

void StudentWorld::initSonar() {
  sonar = std::make_shared<Sonar>(rand() % 61 , rand() % 57, this); // FIX: should not appear in the middle aisle
  sonar->setVisible(true);
}

void StudentWorld::deleteIce(const unsigned int& x, const unsigned int& y, const int& dir) {
  bool isThereIce = false;
  
  switch (dir) {
    case KEY_PRESS_UP :
      for (int i = 0; i < 4; ++i) {
        if (ice[x + i][y + 3]) {
          isThereIce = true;
          ice[x + i][y + 3].reset();
        }
      }
      if (isThereIce) playSound(SOUND_DIG);
      break;
      
    case KEY_PRESS_DOWN :
      for (int i = 0; i < 4; ++i) {
        if (ice[x + i][y]) {
          isThereIce = true;
          ice[x + i][y].reset();
        }
      }
      if (isThereIce) playSound(SOUND_DIG);
      break;
      
    case KEY_PRESS_RIGHT :
      for (int i = 0; i < 4; ++i) {
        if (ice[x + 3][y + i]) {
          isThereIce = true;
          ice[x + 3][y + i].reset();
        }
      }
      if (isThereIce) playSound(SOUND_DIG);
      break;
      
    case KEY_PRESS_LEFT :
      for (int i = 0; i < 4; ++i) {
        if (ice[x][y + i]) {
          isThereIce = true;
          ice[x][y + i].reset();
        }
      }
      if (isThereIce) playSound(SOUND_DIG);
      break;
  }
}


// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
