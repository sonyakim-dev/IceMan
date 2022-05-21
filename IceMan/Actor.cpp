#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

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
        if (getWater() <= 0) return;
        getWorld()->playSound(SOUND_PLAYER_SQUIRT);
        // FIX: fire squirt(add Squirt obj)
//        getWorld()->initSquirt(getDirection());
        getWorld()->getIce_man()->useWater();
        break;
        
      case KEY_PRESS_TAB :
        if (getGold() <= 0) return;
        // ADD: init gold, set state to TEMP
        getWorld()->getIce_man()->useGold();
        break;
        
      case 'z' :
        if (getSonar() <= 0) return;
        // ADD: map the contents of the oil field wihtin 12.0 radius
        getWorld()->findGoodies();
        useSonar();
        break;
        
      case KEY_PRESS_ESCAPE :
        /// completely get annoyed and abort the curr lev
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
    /// if iceman and oil is in range 3.0, take that oil!
    else if (getWorld()->isInRange(x, y, getX(), getY(), 3.0f)) {
      setDead();
      setVisible(false);
      getWorld()->playSound(SOUND_FOUND_OIL);
      getWorld()->increaseScore(1000);
      getWorld()->foundOil(); /// this decrement the number of oil has to be found
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
    /// PERM means gold is pickable by iceman
    else if ((getWorld()->isInRange(x, y, getX(), getY(), 3.0f)) && (getState() == PERM)) {
      setDead();
      setVisible(false);
      getWorld()->playSound(SOUND_GOT_GOODIE);
      getWorld()->increaseScore(10);
      getWorld()->getIce_man()->addGold();
      return;
    }
    /// TEMP means gold is pickable by protestor and it will be deleted after few ticks
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

  switch (getState()) {
    case STABLE :
      /// if there's no ice below, boulder will be in wait state.
      (getWorld()->isIcy(getX(), getY(), getDirection())) ? setState(STABLE) : setState(WAIT);
      break;
      
    case WAIT :
      /// wait for 30 ticks
      if (time_wait == 30) {
        setState(FALL);
        getWorld()->playSound(SOUND_FALLING_ROCK);
      }
      else {
        ++time_wait;
      }
      break;
      
    case FALL :
          /// 1) when it hit the ground OR
      if (getY() <= 0 ||
          /// 2) when it hit another boulder OR
          
          /// 3) when it hit the ice
          getWorld()->isIcy(getX(), getY(), getDirection()) )
      {
        setVisible(false);
        setDead();
      }
      
      // ADD: when it hit iceman or protester

      moveTo(getX(), getY()-1);
      break;
  }
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
  
  if (life_time == std::max(100, 300 - 10 * (int)getWorld()->getLevel())) { // need improvement
    setDead();
    setVisible(false);
  }
  else {
    ++life_time;
  }
  
}
