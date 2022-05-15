#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

void StudentWorld::deleteIce(const unsigned int& x, const unsigned int& y, const int& dir) {
//    if (x >= 64 || y >= 60) return;
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
