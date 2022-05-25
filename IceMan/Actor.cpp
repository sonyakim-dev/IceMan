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
          moveTo(getX(), (getY() == 60 || getWorld()->isBouldery(getX(), getY(), up)) ? getY() : getY() + 1);
          getWorld()->digIce(getX(), getY(), KEY_PRESS_UP);
        }
        setDirection(up);
        break;
        
      case KEY_PRESS_DOWN :
        if (getDirection() == down) {
          moveTo(getX(), (getY() == 0 || getWorld()->isBouldery(getX(), getY(), down)) ? getY() : getY() - 1);
          getWorld()->digIce(getX(), getY(), KEY_PRESS_DOWN);
        }
        setDirection(down);
        break;
        
      case KEY_PRESS_LEFT :
        if (getDirection() == left) {
          moveTo((getX() == 0 || getWorld()->isBouldery(getX(), getY(), left)) ? getX() : getX() - 1, getY());
          getWorld()->digIce(getX(), getY(), KEY_PRESS_LEFT);
        }
        setDirection(left);
        break;
        
      case KEY_PRESS_RIGHT :
        if (getDirection() == right) {
          moveTo((getX() == 60 || getWorld()->isBouldery(getX(), getY(), right)) ? getX() : getX() + 1, getY());
          getWorld()->digIce(getX(), getY(), KEY_PRESS_RIGHT);
        }
        setDirection(right);
        break;
        
      case KEY_PRESS_SPACE :
        if (getWater() <= 0) return;
        getWorld()->squirtWater(getX(), getY(), getDirection());
        break;
        
      case KEY_PRESS_TAB :
        if (getGold() <= 0) return;
        getWorld()->dropGold(getX(), getY());
        break;
        
      case 'z' :
        if (getSonar() <= 0) return;
        getWorld()->findGoodies(getX(), getY());
        break;
        
      case KEY_PRESS_ESCAPE :
        getAnnoyed(100); /// completely get annoyed and abort the curr lev
        break;
    }
  }
}

void Iceman::getAnnoyed(unsigned int damage) {
  hit_points -= damage;
  if (hit_points <= 0) {
    getWorld()->playSound(SOUND_PLAYER_ANNOYED);
    setDead();
  }
}


/*================ REG PROTESTER ================*/
void RegProtester::doSomething() {
  if (!isAlive()) return;
  
  if (move_ticks == std::max(0, 3 - (int)getWorld()->getLevel() / 4)) {
    switch (getState()) {
      case LEAVE :
        if (getX() == 60 && getY() == 60) { setDead(); return; }
        // ADD: move back to 60x60
        break;
        
      case STAY :
//        moveTo(getX()-1, getY()); // THIS IS TEST
        move_ticks = 0;
        
        int x = getWorld()->getIce_man()->getX();
        int y = getWorld()->getIce_man()->getY();
        
        if (getWorld()->isInRange(getX(), getY(), x, y, 4.0f)) {
          if (canShout) {
            switch (getDirection()) {
              case up :
                if (getY() < y) { getWorld()->shoutAtIceman(); canShout = false; }
                break;
              case down :
                if (getY() > y) { getWorld()->shoutAtIceman(); canShout = false; }
                break;
              case right :
                if (getX() < x) { getWorld()->shoutAtIceman(); canShout = false; }
                break;
              case left :
                if (getX() > x) { getWorld()->shoutAtIceman(); canShout = false; }
                break;
            }
          }
          else { // if "can't shout" state
            if (non_resting_ticks == 15) {
              non_resting_ticks = 0;
              canShout = true;
            }
//            else { ++non_resting_ticks; }
          }
        }
        break;
    }
  }
  else { ++move_ticks; ++non_resting_ticks; }
}

void RegProtester::getAnnoyed(unsigned int damage) {
  hit_points -= damage;
  if (hit_points <= 0) {
    getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
    setState(LEAVE);
  }
}

/*================ HARD PROTESTER ================*/
void HardProtester::doSomething() {
  if (!isAlive()) return;
  
  if (move_ticks == std::max(0, 3 - (int)getWorld()->getLevel() / 4)) {
    switch (getState()) {
      case LEAVE :
        if (getX() == 60 && getY() == 60) { setDead(); return; }
        // ADD: move back to 60x60
        break;
        
      case STAY :
        moveTo(getX()-1, getY()); // THIS IS TEST
        move_ticks = 0;
        
        int x = getWorld()->getIce_man()->getX();
        int y = getWorld()->getIce_man()->getY();
        
        if (getWorld()->isInRange(getX(), getY(), x, y, 4.0f)) {
          if (canShout) {
            switch (getDirection()) {
              case up :
                if (getY() < y) { getWorld()->shoutAtIceman(); canShout = false; }
                break;
              case down :
                if (getY() > y) { getWorld()->shoutAtIceman(); canShout = false; }
                break;
              case right :
                if (getX() < x) { getWorld()->shoutAtIceman(); canShout = false; }
                break;
              case left :
                if (getX() > x) { getWorld()->shoutAtIceman(); canShout = false; }
                break;
            }
          }
          else { // if in "can't shout" state
            if (non_resting_ticks == 15) {
              non_resting_ticks = 0;
              canShout = true;
            }
            else { ++non_resting_ticks; }
          }
        }
        break;
    }
  }
  else { ++move_ticks; }
}

void HardProtester::getAnnoyed(unsigned int damage) {
  hit_points -= damage;
  if (hit_points <= 0) {
    getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
    setState(LEAVE);
  }
}


/*================ OIL ================*/
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
      getWorld()->foundOil(); /// this decrement the number of oil has to be found
      setDead();
      return;
    }
}

/*================ GOLD ================*/
void Gold::doSomething() {
  if (!isAlive()) return;

  switch (getState()) {
    case TEMP : /// TEMP means gold is pickable by protestor and it will be deleted after few ticks
      if (getWorld()->isInRange(/*FIX:protester's x*/10, /*FIX:protester's y*/10, getX(), getY(), 3.0f)) {
        getWorld()->bribeProtester(10, 10);
        setDead();
        return;
      }
      else {
        if (life_time == 100) {
          setDead();
          life_time = 0;
        }
        else {
          ++life_time;
        }
      }
      break;
      
    case PERM : /// PERM means gold is pickable by iceman
      auto x = getWorld()->getIce_man()->getX();
      auto y = getWorld()->getIce_man()->getY();
      
      if (!isVisible() && getWorld()->isInRange(x, y, getX(), getY(), 4.0f)) {
        setVisible(true);
      }
      else if (getWorld()->isInRange(x, y, getX(), getY(), 3.0f)) {
        getWorld()->foundGold();
        setDead();
      }
      break;
  }
}

/*================ SONAR ================*/
void Sonar::doSomething() {
  if (!isAlive()) return;
  
  if (getWorld()->isInRange(getX(), getY(), getWorld()->getIce_man()->getX(), getWorld()->getIce_man()->getY(), 3.0f)) {
    getWorld()->foundSonar();
    setDead();
    return;
  }
  
  if (life_time == std::max(100, 300 - 10 * (int)getWorld()->getLevel())) { // need improvement
    setDead();
  }
  else {
    ++life_time;
  }
}

/*================ WATER ================*/
void Water::doSomething() {
  if (!isAlive()) return;
  
  if (getWorld()->isInRange(getX(), getY(), getWorld()->getIce_man()->getX(), getWorld()->getIce_man()->getY(), 3.0f)) {
    getWorld()->foundWater();
    setDead();
    return;
  }
  
  if (life_time == std::max(100, 300 - 10 * (int)getWorld()->getLevel())) { // need improvement
    setDead();
  }
  else {
    ++life_time;
  }
}

/*================ BOULDER ================*/
void Boulder::doSomething() {
  if (!isAlive()) return;

  switch (getState()) {
    case STABLE :
      /// if there's no ice below, boulder will be in wait state.
      (getWorld()->isIcy(getX(), getY(), down)) ? setState(STABLE) : setState(WAIT);
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
          getWorld()->isBouldery(getX(), getY(), down) || //?
          /// 3) when it hit the ice
          getWorld()->isIcy(getX(), getY(), down) )
      { setDead(); }
      
      /// when it hit the iceman
      if (getWorld()->isInRange(getX(), getY(), getWorld()->getIce_man()->getX(), getWorld()->getIce_man()->getY(), 3.0f)){
        getWorld()->getIce_man()->getAnnoyed(100);
      }
      /// when it hit the protester
//      if (within range 3.0) {
//
//      }
      
      moveTo(getX(), getY()-1);
      break;
  }
}

/*================ SQUIRT ================*/
void Squirt::doSomething() {
  if (!isAlive()) return;
  
  // ADD: protester is within 3.0, make annoyed
  
  /// when it hit the ice or boulder set it dead
  if (getWorld()->isIcy(getX(), getY(), getDirection()) ||
      getWorld()->isBouldery(getX(), getY(), getDirection())) {
    setDead();
    return;
  }
  
  if (life_time > 4) { setDead(); return; }
  else ++life_time;
  
  switch (getDirection()) {
    case Actor::up :
      moveTo(getX(), getY()+1);
      break;
    case Actor::down :
      moveTo(getX(), getY()-1);
      break;
    case Actor::right :
      moveTo(getX()+1, getY());
      break;
    case Actor::left :
      moveTo(getX()-1, getY());
      break;
  }
}

