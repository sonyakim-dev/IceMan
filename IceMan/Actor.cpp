#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <queue>

/*================ ICEMAN ================*/
void Iceman::doSomething() {
  if (!isAlive()) return;
  
  int ch;
  if (getWorld()->getKey(ch) == true) {
    switch (ch) {
      case KEY_PRESS_UP :
        if (getDirection() == up) {
          if (getY() >= 60 || getWorld()->isBouldery(getX(), getY(), up)) {
            moveTo(getX(), getY());
          }
          else {
            getWorld()->digIce(getX(), getY(), KEY_PRESS_UP);
            moveTo(getX(), getY() + 1);
          }
        }
        else { setDirection(up); }
        break;
        
      case KEY_PRESS_DOWN :
        if (getDirection() == down) {
          if (getY() <= 0 || getWorld()->isBouldery(getX(), getY(), down)) {
            moveTo(getX(), getY());
          }
          else {
            getWorld()->digIce(getX(), getY(), KEY_PRESS_DOWN);
            moveTo(getX(), getY() - 1);
          }
        }
        else { setDirection(down); }
        break;
        
      case KEY_PRESS_RIGHT :
        if (getDirection() == right) {
          if (getX() >= 60 || getWorld()->isBouldery(getX(), getY(), right)) {
            moveTo(getX(), getY());
          }
          else {
            getWorld()->digIce(getX(), getY(), KEY_PRESS_RIGHT);
            moveTo(getX() + 1, getY());
          }
        }
        else { setDirection(right); }
        break;
        
      case KEY_PRESS_LEFT :
        if (getDirection() == left) {
          if (getX() <= 0 || getWorld()->isBouldery(getX(), getY(), left)) {
            moveTo(getX(), getY());
          }
          else {
            getWorld()->digIce(getX(), getY(), KEY_PRESS_LEFT);
            moveTo(getX() - 1, getY());
          }
        }
        else { setDirection(left); }
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
        getWorld()->discoverGoodies(getX(), getY());
        break;
        
      case KEY_PRESS_ESCAPE :
        getAnnoyed(100); /// completely get annoyed and abort the curr lev
        break;
    }
  }
}


/*================ PROTESTER ================*/
void Protester::findShortestPath(const int startX, const int startY, const int finalX, const int finalY) {
  std::queue<std::pair<int,int>> xy;
  int currStep = 0, currX, currY;
  
  stepArray[finalX][finalY] = 0;
  xy.push(std::make_pair(finalX, finalY));
  
  while (!xy.empty()) {
    currX = xy.front().first;
    currY = xy.front().second;
    xy.pop();
    
    if (currX == startX && currY == startY) {
      didFindPath = true;
      step = stepArray[currX][currY];
      return;
    }
    
    currStep = stepArray[currX][currY];
    ++currStep;
    
    if (!getWorld()->isIcyOrBouldery(currX, currY, up) && stepArray[currX][currY+1] == 9999) {
      stepArray[currX][currY+1] = currStep;
      xy.push(std::make_pair(currX, currY+1));
    }
    if (!getWorld()->isIcyOrBouldery(currX, currY, down) && stepArray[currX][currY-1] == 9999) {
      stepArray[currX][currY-1] = currStep;
      xy.push(std::make_pair(currX, currY-1));
    }
    if (!getWorld()->isIcyOrBouldery(currX, currY, right) && stepArray[currX+1][currY] == 9999) {
      stepArray[currX+1][currY] = currStep;
      xy.push(std::make_pair(currX+1, currY));
    }
    if (!getWorld()->isIcyOrBouldery(currX, currY, left) && stepArray[currX-1][currY] == 9999) {
      stepArray[currX-1][currY] = currStep;
      xy.push(std::make_pair(currX-1, currY));
    }
  } /// end of while loop
  
//  didFindPath = false;
  throw; // THIS IS FOR DEBUG
  return;
}

void Protester::getAnnoyed(unsigned int damage) {
  dropHP(damage);
  
  if (getHP() <= 0 && getState() != LEAVE) {
    getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
    setState(LEAVE);
  }
  else if (getState() != LEAVE) {
    getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
  }
}

bool Protester::isTimeToMove() {
  if (can_move || resting_ticks == 0) {
    can_move = false;
    return true;
  }
  else { return false; }
}
void Protester::setRestingTicks() { resting_ticks = std::max(0, 3 - (int)getWorld()->getLevel() / 4); }
void Protester::setStalledTicks() { stalled_ticks = std::max(50, 100 - (int)getWorld()->getLevel() * 10); }

/*================ REGULAR PROTESTER ================*/
void RegProtester::doSomething() {
  if (!isAlive()) return;
  
  if (isTimeToMove()) { /// when resting tick finished counting OR if it's the very first tick the protester being created
    switch (getState()) {
      case WAIT : /// when shot by water
        if (stalled_ticks == 0) { /// when finished being stalled
          setStalledTicks();
          setState(STAY);
          recountRestingTicks();
        }
        else {
          --stalled_ticks;
        }
        break;
        
        
      case LEAVE : /// fully get annoyed and leave the oil field
        setRestingTicks();
        
        if (getX() == 60 && getY() == 60) { setDead(); break; }
        
        if (!didFindPath) {
          findShortestPath(getX(), getY(), 60, 60); /// this is for setting stepArray only once!
        }
        if (didFindPath) {
          if (step - 1 == stepArray[getX()][getY()+1]) {
            setDirection(up);
            moveTo(getX(), getY()+1);
            --step;
          }
          else if (step - 1 == stepArray[getX()][getY()-1]) {
            setDirection(down);
            moveTo(getX(), getY()-1);
            --step;
          }
          else if (step - 1 == stepArray[getX()+1][getY()]) {
            setDirection(right);
            moveTo(getX()+1, getY());
            --step;
          }
          else if (step - 1 == stepArray[getX()-1][getY()]) {
            setDirection(left);
            moveTo(getX()-1, getY());
            --step;
          }
        }
        break;
        
        
      case STAY : /// non-leave-the-oil-field state
        setRestingTicks();
        
        int icemanX = getWorld()->getIce_man()->getX();
        int icemanY = getWorld()->getIce_man()->getY();
        
        /// 4) if iceman is close enough and protester is facing him
        if (canShout && getWorld()->isInRange(getX(), getY(), icemanX, icemanY, 4.0f)) {
          if ((getDirection() == up    && getY() <= icemanY) ||
              (getDirection() == down  && getY() >= icemanY) ||
              (getDirection() == right && getX() <= icemanX) ||
              (getDirection() == left  && getX() >= icemanX))
          {
                getWorld()->shoutAtIceman();
                canShout = false;
                ticks_since_shout = 0;
          }
          return;
        }
        else if (!canShout && ticks_since_shout >= 15) {
            canShout = true;
        }
        else if (!canShout) { ++ticks_since_shout; }
        
        
        /// 5) if protester is in a straight horizontal or vertical line of sight to iceman
        Direction dir = none;
        if (getWorld()->canSeeIceman(getX(), getY(), icemanX, icemanY, dir)) {
          setDirection(dir);
          switch (dir) {
            case up :
              if (abs(getY()-icemanY) > 4) { moveTo(getX(), getY()+1); }
              break;
            case down :
              if (abs(getY()-icemanY) > 4) { moveTo(getX(), getY()-1); }
              break;
            case right :
              if (abs(getX()-icemanX) > 4) { moveTo(getX()+1, getY()); }
              break;
            case left :
              if (abs(getX()-icemanX) > 4) { moveTo(getX()-1, getY()); }
              break;
            default: throw;
          }
          move_straight_distance = 0;
          return;
        }
        
        else { /// 6) if protester can't directly see the iceman
          if (move_straight_distance <= 0) {
            while (true) {
              int dir = rand() % 4 + 1; /// pick a random direction(1-4)
              if (getWorld()->isIcyOrBouldery(getX(), getY(), (Direction)dir)) continue;
              setDirection((Direction)dir);
              pickMoveStraightDistance();
              break; /// continue with step 8
            }
          }
          
          else { /// 7) if MoveStraightDistance > 0
            if (canTurn && getWorld()->isAtJunction(getX(), getY(), getDirection())) {
                canTurn = false;
                ticks_since_turn = 0;
              
                switch (getDirection()) {
                  case up :
                  case down :
                    if (rand() % 2 == 0) {
                      (getWorld()->isIcyOrBouldery(getX(), getY(), right)) ?
                        setDirection(left) : setDirection(right);
                    }
                    else {
                      (getWorld()->isIcyOrBouldery(getX(), getY(), left)) ?
                        setDirection(right) : setDirection(left);
                    }
                    break;
                    
                  case right :
                  case left :
                    if (rand() % 2 == 0) {
                      (getWorld()->isIcyOrBouldery(getX(), getY(), up)) ?
                        setDirection(down) : setDirection(up);
                    }
                    else {
                      (getWorld()->isIcyOrBouldery(getX(), getY(), down)) ?
                        setDirection(up) : setDirection(down);
                    }
                    break;
                    
                  default: throw;
                }
                pickMoveStraightDistance();
            }
            else if (!canTurn && ticks_since_turn >= 200) {
                canTurn = true;
            }
            else if (!canTurn) { ++ticks_since_turn; }
          }
          
          if (getWorld()->isIcyOrBouldery(getX(), getY(), getDirection())) {
            move_straight_distance = 0;
            return;
          }
            
          switch (getDirection()) {
            case up :
              moveTo(getX(), getY()+1); break;
            case down :
              moveTo(getX(), getY()-1); break;
            case right :
              moveTo(getX()+1, getY()); break;
            case left :
              moveTo(getX()-1, getY()); break;
            default: throw; // THIS IS FOR DEBUG
          }
          --move_straight_distance;
        }
        break; /// end of STAY state
    }
  }
  else { countRestingTicks(); return; }
}

/*================ HARD PROTESTER ================*/
void HardProtester::doSomething() {
  if (!isAlive()) return;
  
  if (isTimeToMove()) { /// when resting tick finished counting OR if it's the very first tick the protester being created
    switch (getState()) {
      case WAIT : /// when shot by water or found gold
        if (stalled_ticks == 0) { /// when finished being stalled
          setStalledTicks();
          setState(STAY);
          recountRestingTicks();
        }
        else {
          --stalled_ticks;
        }
        break;
        
        
      case LEAVE : /// fully get annoyed and leave the oil field
        setRestingTicks();
        
        if (getX() == 60 && getY() == 60) { setDead(); break; }
        
        if (!didFindPath) {
          findShortestPath(getX(), getY(), 60, 60);
        }
        if (didFindPath) {
          if (step - 1 == stepArray[getX()][getY()+1]) {
            setDirection(up);
            moveTo(getX(), getY()+1);
            --step;
          }
          else if (step - 1 == stepArray[getX()][getY()-1]) {
            setDirection(down);
            moveTo(getX(), getY()-1);
            --step;
          }
          else if (step - 1 == stepArray[getX()+1][getY()]) {
            setDirection(right);
            moveTo(getX()+1, getY());
            --step;
          }
          else if (step - 1 == stepArray[getX()-1][getY()]) {
            setDirection(left);
            moveTo(getX()-1, getY());
            --step;
          }

        }
        break;
        
        
      case STAY : /// non-leave-the-oil-field state
        setRestingTicks();
        
        int icemanX = getWorld()->getIce_man()->getX();
        int icemanY = getWorld()->getIce_man()->getY();
        
        /// 4) if iceman is close enough and protester is facing him
        if (canShout && getWorld()->isInRange(getX(), getY(), icemanX, icemanY, 4.0f)) {
          if ((getDirection() == up && getY() <= icemanY) ||
              (getDirection() == down && getY() >= icemanY) ||
              (getDirection() == right && getX() <= icemanX) ||
              (getDirection() == left && getX() >= icemanX))
          {
                getWorld()->shoutAtIceman();
                canShout = false;
                ticks_since_shout = 0;
          }
          return;
        }
        else if (!canShout && ticks_since_shout >= 15) {
            canShout = true;
        }
        else if (!canShout) { ++ticks_since_shout; }
        
        
        /// 6) if protester is in a straight horizontal or vertical line of sight to iceman
        Direction dir = none;
        if (getWorld()->canSeeIceman(getX(), getY(), icemanX, icemanY, dir)) {
          setDirection(dir);
          switch (dir) {
            case up :
              if (abs(getY()-icemanY) > 4) { moveTo(getX(), getY()+1); }
              break;
            case down :
              if (abs(getY()-icemanY) > 4) { moveTo(getX(), getY()-1); }
              break;
            case right :
              if (abs(getX()-icemanX) > 4) { moveTo(getX()+1, getY()); }
              break;
            case left :
              if (abs(getX()-icemanX) > 4) { moveTo(getX()-1, getY()); }
              break;
            default: throw;
          }
          move_straight_distance = 0;
          return;
        }
        
        else { /// if protester can't directly see the iceman
          
          /// 5) if protester is less than or equal to a total of M legal horizontal or vertical moves away from the curr iceman location
          int_ M = 16 + getWorld()->getLevel() * 2;
          int_ manX = getWorld()->getIce_man()->getX();
          int_ manY =  getWorld()->getIce_man()->getY();
          
          if (getWorld()->isInRange(getX(), getY(), manX, manY, M)) {
            findShortestPath(getX(), getY(), manX, manY);
            bool didHearIceman = false;
            if (step <= M) {
              didHearIceman = true;
              if (step - 1 == stepArray[getX()][getY()+1]) {
                setDirection(up);
                moveTo(getX(), getY()+1);
                --step;
              }
              else if (step - 1 == stepArray[getX()][getY()-1]) {
                setDirection(down);
                moveTo(getX(), getY()-1);
                --step;
              }
              else if (step - 1 == stepArray[getX()+1][getY()]) {
                setDirection(right);
                moveTo(getX()+1, getY());
                --step;
              }
              else if (step - 1 == stepArray[getX()-1][getY()]) {
                setDirection(left);
                moveTo(getX()-1, getY());
                --step;
              }
            }
            setStepArray(); /// reset stepArray to 9999 to recalculate the shortest path on the next tick
            if (didHearIceman) { return; }
          }
          
          /// 7) if MoveStraightDistance <=  0
          if (move_straight_distance <= 0) {
            while (true) {
              int dir = rand() % 4 + 1; /// pick a random direction(1-4)
              if (getWorld()->isIcyOrBouldery(getX(), getY(), (Direction)dir)) continue;
              setDirection((Direction)dir);
              pickMoveStraightDistance();
              break;
            }
          }
          
          else { /// 8) if MoveStraightDistance > 0
            if (canTurn && getWorld()->isAtJunction(getX(), getY(), getDirection())) {
                canTurn = false;
                ticks_since_turn = 0;
              
                switch (getDirection()) {
                  case up :
                  case down :
                    if (rand() % 2 == 0) {
                      (getWorld()->isIcyOrBouldery(getX(), getY(), right)) ?
                        setDirection(left) : setDirection(right);
                    }
                    else {
                      (getWorld()->isIcyOrBouldery(getX(), getY(), left)) ?
                        setDirection(right) : setDirection(left);
                    }
                    break;
                    
                  case right :
                  case left :
                    if (rand() % 2 == 0) {
                      (getWorld()->isIcyOrBouldery(getX(), getY(), up)) ?
                        setDirection(down) : setDirection(up);
                    }
                    else {
                      (getWorld()->isIcyOrBouldery(getX(), getY(), down)) ?
                        setDirection(up) : setDirection(down);
                    }
                    break;
                    
                  default: throw;
                }
                pickMoveStraightDistance();
            }
            else if (!canTurn && ticks_since_turn >= 200) {
                canTurn = true;
            }
            else if (!canTurn) { ++ticks_since_turn; }
          }
          
          if (getWorld()->isIcyOrBouldery(getX(), getY(), getDirection())) {
            move_straight_distance = 0;
            return;
          }
            
          switch (getDirection()) {
            case up :
              moveTo(getX(), getY()+1); break;
            case down :
              moveTo(getX(), getY()-1); break;
            case right :
              moveTo(getX()+1, getY()); break;
            case left :
              moveTo(getX()-1, getY()); break;
            default: throw;
          }
          --move_straight_distance;
        }
        break; /// end of STAY state
    }
  }
  else { countRestingTicks(); return; }
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
    /// if iceman and oil is in range 3.0, get that oil
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
      if (getWorld()->bribeProtester(getX(), getY())) {
        setDead();
        return;
      }
      else {
        if (life_time == 100) { setDead(); return; }
        else { ++life_time; }
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
  
  if (life_time >= std::max(100, 300 - 10 * (int)getWorld()->getLevel())) { // need improvement
    setDead();
  }
  else { ++life_time; }
}

/*================ WATER ================*/
void Water::doSomething() {
  if (!isAlive()) return;
  
  if (getWorld()->isInRange(getX(), getY(), getWorld()->getIce_man()->getX(), getWorld()->getIce_man()->getY(), 3.0f)) {
    getWorld()->foundWater();
    setDead();
    return;
  }
  
  if (life_time >= std::max(100, 300 - 10 * (int)getWorld()->getLevel())) { // need improvement
    setDead();
  }
  else { ++life_time; }
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
      else { ++time_wait; }
      break;
      
    case FALL :
      /// when it hit the ground OR another boulder OR ice
      if (getWorld()->isIcyOrBouldery(getX(), getY(), down)) { setDead(); }
      
      /// when it hit the iceman
      if (getWorld()->bonkIceman(getX(), getY())) { return; }
      /// when it hit the protester
      getWorld()->bonkProtester(getX(), getY());
      
      moveTo(getX(), getY()-1);
      break;
  }
}

/*================ SQUIRT ================*/
void Squirt::doSomething() {
  if (!isAlive()) return;
  
  if (getWorld()->shootProtester(getX(), getY())) { setDead(); return; }
  
  if (life_time > 4) { setDead(); return; }
  else ++life_time;
  
  /// when it hit the ice or boulder set it dead
  if (getWorld()->isIcyOrBouldery(getX(), getY(), getDirection())) {
    setDead();
    return;
  }
  else {
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
      default: throw; // THIS IS FOR DEBUG
    }
  }
}

