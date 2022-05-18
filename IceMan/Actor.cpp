#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

/*================ ICEMAN ================*/
void Iceman::doSomething() {
  if (!isActive()) return;
  
  int ch;
  if (getWorld()->getKey(ch) == true) {
    switch (ch) {
      case KEY_PRESS_UP :
        if (getDirection() == up) {
          moveTo(getX(), (getY() == 60) ? getY() : getY() + 1);
          getWorld()->deleteIce(getX(), getY(), KEY_PRESS_UP);
        }
        setDirection(up);
        break;
        
      case KEY_PRESS_DOWN :
        if (getDirection() == down) {
          moveTo(getX(), (getY() == 0) ? getY() : getY() - 1);
          getWorld()->deleteIce(getX(), getY(), KEY_PRESS_DOWN);
        }
        setDirection(down);
        break;
        
      case KEY_PRESS_LEFT :
        if (getDirection() == left) {
          moveTo((getX() == 0) ? getX() : getX() - 1, getY());
          getWorld()->deleteIce(getX(), getY(), KEY_PRESS_LEFT);
        }
        setDirection(left);
        break;
        
      case KEY_PRESS_RIGHT :
        if (getDirection() == right) {
          moveTo((getX() == 60) ? getX() : getX() + 1, getY());
          getWorld()->deleteIce(getX(), getY(), KEY_PRESS_RIGHT);
        }
        setDirection(right);
        break;
        
      case KEY_PRESS_SPACE :
        // fire squirt(add Squirt obj)
        break;
        
      case KEY_PRESS_ESCAPE :
        // completely annoyed and abort th curr lev
        break;
    }
  }
}


bool Goodies::isInRange(const unsigned int& x, const unsigned int& y, const float& radius) {
  int goodieX = x+2, goodieY = y+2;
    if (sqrt(pow((getX()+2)-(goodieX), 2) + pow((getY()+2)-(goodieY), 2)) <= radius)
        return true;
    else return false;
}

void Oil::doSomething() {
    if (!isActive()) return;
  
    auto x = getWorld()->getIce_man()->getX();
    auto y = getWorld()->getIce_man()->getY();
    if (!isVisible() && isInRange(x, y, 4.0)) {
        setVisible(true);
        return;
    }
    else if (isInRange(x, y, 3.0)) {
      setUnactive();
      setVisible(false);
      getWorld()->playSound(SOUND_FOUND_OIL);
      getWorld()->increaseScore(1000);
      getWorld()->foundOil();
      return;
    }
}

void Gold::doSomething() {
    if (!isActive()) return;
  
    auto x = getWorld()->getIce_man()->getX();
    auto y = getWorld()->getIce_man()->getY();
    if (!isVisible() && isInRange(x, y, 4.0)) {
        setVisible(true);
        return;
    }
    else if (isInRange(x, y, 3.0) /*&& pickable by Iceman*/) {
      setUnactive();
      setVisible(false);
      getWorld()->playSound(SOUND_GOT_GOODIE);
      getWorld()->increaseScore(10);
      getWorld()->getIce_man()->addGold();
      return;
    }
//    else if (isInRange(x, y, 3.0) /*&& pickable by Protester*/) {
//      setDead();
//      setVisible(false);
//      getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
//      // 3) protestor react as bribed
//      // 4) increase 25pts
//    }
}

void Boulder::doSomething() {
  if (!isActive()) return;
  if (isStable()) return;
  
  // wait for 30 ticks
}

bool Boulder::isStable() {
  for (int i = 0; i < 4; ++i) {
    if (getWorld()->isIcy(getX() + i, getY())) return true;
  }
  return false;
}

void Sonar::doSomething() {
  if (!isActive()) return;
  
}
