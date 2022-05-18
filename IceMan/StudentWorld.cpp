#include "StudentWorld.h"
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

void StudentWorld::setDisplayText() {
    int level = getLevel();
    int lives = getLives();
    int health = ice_man->getHP(10);
    int squirts = ice_man->getWater();
    int gold = ice_man->getGold();
    int barrelsLeft = 0;
    int sonar = ice_man->getSonars();
    int score = getScore();

    string s = "Lvl: " + to_string(level) + " Lives: " + to_string(lives) + " Hlth: " + to_string(health) + " Wtr: " +
        to_string(squirts) + " Gld: " + to_string(gold) + " Oil Left: " + to_string(barrelsLeft) + "  Sonar: " + to_string(sonar) +
        " Scr: " + to_string(score);

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
  std::srand((unsigned int)std::time(0));
  
  gold = std::make_shared<Gold>(rand() % 61 , rand() % 57, this);
  gold->setVisible(true);
}

void StudentWorld::deleteIce(const unsigned int& x, const unsigned int& y, const int& dir) {
  switch (dir) {
    case KEY_PRESS_UP :
      for (int i = 0; i < 4; ++i) {
        if (ice[x + i][y + 3]) {
          playSound(SOUND_DIG);
          ice[x + i][y + 3].reset();
        }
      }
      break;
      
    case KEY_PRESS_DOWN :
      for (int i = 0; i < 4; ++i) {
        if (ice[x + i][y]) {
          playSound(SOUND_DIG);
          ice[x + i][y].reset();
        }
      }
      break;
      
    case KEY_PRESS_RIGHT :
      for (int i = 0; i < 4; ++i) {
        if (ice[x + 3][y + i]) {
          playSound(SOUND_DIG);
          ice[x + 3][y + i].reset();
        }
      }
      break;
      
    case KEY_PRESS_LEFT :
      for (int i = 0; i < 4; ++i) {
        if (ice[x][y + i]) {
          playSound(SOUND_DIG);
          ice[x][y + i].reset();
        }
      }
      break;
  }
}


// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
