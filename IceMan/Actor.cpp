#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
bool Iceman::isAlive() const {
  return getWorld()->getLives() > 0;
}

void Iceman::doSomething() {
  if (!isAlive()) return;
  
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
          GWSTATUS_FINISHED_LEVEL;
        break;
    }
  }
}
