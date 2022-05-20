#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

/*================ ICEMAN ================*/
void Iceman::doSomething() {
  if (!isAlive()) return;
  
  int ch;
  if (getWorld()->getKey(ch) == true) {
    switch (ch) {
      case KEY_PRESS_UP :
        if (getDirection() == up) {
          moveTo(getX(), (getY() == 60) ? getY() : getY() + 1);
          getWorld()->digIce(getX(), getY(), KEY_PRESS_UP);
        }
        setDirection(up);
        break;
        
      case KEY_PRESS_DOWN :
        if (getDirection() == down) {
          moveTo(getX(), (getY() == 0) ? getY() : getY() - 1);
          getWorld()->digIce(getX(), getY(), KEY_PRESS_DOWN);
        }
        setDirection(down);
        break;
        
      case KEY_PRESS_LEFT :
        if (getDirection() == left) {
          moveTo((getX() == 0) ? getX() : getX() - 1, getY());
          getWorld()->digIce(getX(), getY(), KEY_PRESS_LEFT);
        }
        setDirection(left);
        break;
        
      case KEY_PRESS_RIGHT :
        if (getDirection() == right) {
          moveTo((getX() == 60) ? getX() : getX() + 1, getY());
          getWorld()->digIce(getX(), getY(), KEY_PRESS_RIGHT);
        }
        setDirection(right);
        break;
        
      case KEY_PRESS_SPACE :
        // fire squirt(add Squirt obj)
        if (getWater() <= 0) return;
        getWorld()->playSound(SOUND_PLAYER_SQUIRT);
        break;
        
      case KEY_PRESS_ESCAPE :
        // completely annoyed and abort the curr lev
        getAnnoyed(100);
        break;
    }
  }
}


/*================ Oil ================*/
void Oil::doSomething() {
    if (!isAlive()) return;
  
    auto x = getWorld()->getIce_man()->getX();
    auto y = getWorld()->getIce_man()->getY();
  
    if (!isVisible() && getWorld()->isInRange(x, y, getX(), getY(), 4.0f)) {
        setVisible(true);
        return;
    }
    // if iceman and oil is in range 3.0, take that oil!
    else if (getWorld()->isInRange(x, y, getX(), getY(), 3.0f)) {
      setDead();
      setVisible(false);
      getWorld()->playSound(SOUND_FOUND_OIL);
      getWorld()->increaseScore(1000);
      getWorld()->foundOil(); // this decrement the number of oil has to be found
      return;
    }
}

/*================ GOLD ================*/
void Gold::doSomething() {
    if (!isAlive()) return;
  
    auto x = getWorld()->getIce_man()->getX();
    auto y = getWorld()->getIce_man()->getY();
  
    if (!isVisible() && getWorld()->isInRange(x, y, getX(), getY(), 4.0f)) {
        setVisible(true);
        return;
    }
    // PERM means gold is pickable by iceman
    else if ((getWorld()->isInRange(x, y, getX(), getY(), 3.0f)) && (getState() == PERM)) {
      setDead();
      setVisible(false);
      getWorld()->playSound(SOUND_GOT_GOODIE);
      getWorld()->increaseScore(10);
      getWorld()->getIce_man()->addGold();
      return;
    }
    // TEMP means gold is pickable by protestor and it will be deleted after few ticks
    else if (getWorld()->isInRange(x, y, getX(), getY(), 3.0f) && (getState() == TEMP)) {
      setDead();
      setVisible(false);
      getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
      // ADD: protestor react as bribed
      getWorld()->increaseScore(25);
    }
}

/*================ BOULDER ================*/
void Boulder::doSomething() {
  if (!isAlive()) return;

  if (getState() == WAIT) {
    // ADD: wait for 30 ticks
    setState(FALL);
    return;
  }
  
  if (getState() == FALL) {
    // FIX: need to disapear after certain ticks
    moveTo(getX(), getY()-1);
  }
  
  // if there's no ice below, boulder will be in wait state.
  (getWorld()->isIcy(getX(), getY(), getDirection())) ? setState(STABLE) : setState(WAIT);
}

/*================ SONAR ================*/
void Sonar::doSomething() {
  if (!isAlive()) return;
  
  auto x = getWorld()->getIce_man()->getX();
  auto y = getWorld()->getIce_man()->getY();

  if (getWorld()->isInRange(x, y, getX(), getY(), 3.0f)) {
    setDead();
    setVisible(false);
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->increaseScore(75);
    getWorld()->getIce_man()->addSonar();
    return;
  }
}
