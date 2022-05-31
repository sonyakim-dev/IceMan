#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld; // incomplete type declaration
enum State { TEMP, PERM, WAIT, FALL, STABLE, STAY, LEAVE };


class Actor : public GraphObject {
private:
  bool active = true;
  State state;
  StudentWorld* stud_world;
public:
  Actor(int imageID, int startX, int startY, StudentWorld* stud_world, Direction dir = right, double size = 1.0, unsigned int depth = 0)
    : GraphObject(imageID, startX, startY, dir, size, depth), stud_world(stud_world) {}
  virtual ~Actor() {}
  virtual void doSomething() = 0;
  virtual StudentWorld* getWorld() const final { return stud_world; }
  virtual bool isAlive() const final { return active; }
  virtual void setDead() final { active = false; setVisible(false); }
  virtual int getState() const final { return state; }
  virtual void setState(State condition) final { state = condition; }
};


/*================ ICE ================*/
class Ice : public Actor {
public:
  Ice(int startX, int startY, StudentWorld* stud_world)
    : Actor(IID_ICE, startX, startY, stud_world, none, 0.25, 3) { setVisible(true); }
  virtual ~Ice() {}
  virtual void doSomething() override {}
};


/*================ CHARACTER ================*/
class Character : public Actor {
private:
  int hit_points;
protected:
  virtual void setHP(unsigned int hp) final { hit_points = hp; }
  virtual void dropHP(unsigned int damage) final { hit_points -= damage; }
public:
  Character(int imageID, int startX, int startY, StudentWorld* stud_world, Direction dir = right, double size = 1.0, unsigned int depth = 0)
    : Actor(imageID, startX, startY, stud_world, dir, size, depth) {}
  virtual ~Character() {}
  virtual void doSomething() override = 0;
  virtual void getAnnoyed(unsigned int damage) = 0; /// both iceman and protester get annoyed
  virtual int getHP() const final { return hit_points; }
};

class Iceman : public Character {
private:
  unsigned int i_golds {0};
  unsigned int i_waters {5};
  unsigned int i_sonars {1};
public:
  Iceman(StudentWorld* stud_world)
    : Character(IID_PLAYER, 30, 60, stud_world, right, 1.0, 0) { setVisible(true); setHP(10); }
  virtual ~Iceman() {}
  virtual void doSomething() override;
  virtual void getAnnoyed(unsigned int damage) override;
  virtual void addGold() { ++i_golds; }
  virtual void addWater() { i_waters += 5; }
  virtual void addSonar() { i_sonars += 2; }
  virtual void useGold() { --i_golds; }
  virtual void useWater() { --i_waters; }
  virtual void useSonar() { --i_sonars; }
  virtual int getGold() const { return i_golds; }
  virtual int getWater() const { return i_waters; }
  virtual int getSonar() const { return i_sonars; }
};


class Protester : public Character {
private:
  unsigned int move_straight_distance; /// numSquresToMoveInCurrentDirection
  unsigned int resting_ticks;
  unsigned int stalled_ticks;
  void setStalledTicks();
  bool can_move = true;
protected:
  unsigned int non_resting_ticks {0};
  unsigned int non_resting_turn {0}; //TEST
  bool canShout = true;
  bool canTurn = true;
  
  virtual bool canMoveStraight() const { return move_straight_distance != 0; }
  virtual void setMoveStraightDistance() final { move_straight_distance = rand() % 53 + 8; } /// 8 <= rand <= 60
  virtual void setMoveStraightDistance_0() final { move_straight_distance = 0; }
  virtual void countMoveStraightDistance() final { --move_straight_distance; }
  
  virtual bool canMove() final;
  virtual void setRestingTicks() final;
  virtual void countRestingTicks() final { --resting_ticks; }
  
  virtual bool canGoOn() const { return stalled_ticks == 0; }
  virtual void countStalledTicks() final { --stalled_ticks; }
public:
  Protester(int imageID, StudentWorld* stud_world)
  : Character(imageID, 60, 60, stud_world, left, 1, 0) { setMoveStraightDistance(); setRestingTicks(); setStalledTicks(); }
  virtual ~Protester() {}
  virtual void doSomething() override = 0;
  virtual void getAnnoyed(unsigned int damage) override;
  virtual bool findShortestPath(int startX, int startY, int finalX, int finalY, Direction& dir, int& steps) const;
};

class RegProtester : public Protester {
public:
  RegProtester(StudentWorld* stud_world)
    : Protester(IID_PROTESTER, stud_world) { setVisible(true); setHP(5); setState(STAY); }
  virtual ~RegProtester() {}
  virtual void doSomething() override;
};

class HardProtester : public Protester {
public:
  HardProtester(StudentWorld* stud_world)
    : Protester(IID_HARD_CORE_PROTESTER, stud_world) { setVisible(true); setHP(20); setState(STAY); }
  virtual ~HardProtester() {}
  virtual void doSomething() override;
private:
};


/*================ GOODIES ================*/
class Goodies : public Actor {
public:
    Goodies(int imageID, int startX, int startY, StudentWorld* stud_world, Direction dir = right, double size = 1.0, unsigned int depth = 0)
      : Actor(imageID, startX, startY, stud_world, dir, size, depth) {}
    virtual ~Goodies() {}
    virtual void doSomething() override = 0;
};

class Oil : public Goodies {
public:
    Oil(int startX, int startY, StudentWorld* stud_world)
      : Goodies(IID_BARREL, startX, startY, stud_world, right, 1, 2) { setVisible(true); }
    virtual ~Oil() {}
    virtual void doSomething() override;
};

class Gold : public Goodies {
public:
    Gold(int startX, int startY, StudentWorld* stud_world, State condition = PERM)
      : Goodies(IID_GOLD, startX, startY, stud_world, right, 1, 2) { setState(condition); (condition == TEMP) ? setVisible(true) : setVisible(false);}
    virtual ~Gold() {}
    virtual void doSomething() override;
private:
  unsigned int life_time{0}; /// gold could stay on the field for only for a certain time when it's TEMP
};

class Sonar : public Goodies {
public:
    Sonar(StudentWorld* stud_world)
      : Goodies(IID_SONAR, 0, 60, stud_world, right, 1, 2) { setVisible(true); setState(TEMP); }
    virtual ~Sonar() {}
    virtual void doSomething() override;
private:
    unsigned int life_time{0}; /// sonar could stay on the field for only for a certain time
};

class Water : public Goodies {
public:
    Water(int startX, int startY, StudentWorld* stud_world)
      : Goodies(IID_WATER_POOL, startX, startY, stud_world, right, 1, 2) { setVisible(true); setState(TEMP); }
    virtual ~Water() {}
    virtual void doSomething() override;
private:
    unsigned int life_time{0}; /// water could stay on the field for only for a certain time
};


/*================ ATTACK ================*/
class Attack : public Actor {
public:
  Attack(int imageID, int startX, int startY, StudentWorld* stud_world, Direction dir = right, double size = 1.0, unsigned int depth = 0)
    : Actor(imageID, startX, startY, stud_world, dir, size, depth){}
  virtual ~Attack() {}
  virtual void doSomething() override = 0;
};

class Boulder : public Attack {
public:
  Boulder(int startX, int startY, StudentWorld* stud_world)
    : Attack(IID_BOULDER, startX, startY, stud_world, down, 1, 1) { setVisible(true); setState(STABLE); }
  virtual ~Boulder() {}
  virtual void doSomething() override;
private:
  unsigned int time_wait {0};
};

class Squirt : public Attack {
public:
  Squirt(int startX, int startY, StudentWorld* stud_world, Direction dir = right)
    : Attack(IID_WATER_SPURT, startX, startY, stud_world, dir, 1, 1) { setVisible(true); }
  virtual ~Squirt() {}
  virtual void doSomething() override;
private:
  unsigned int life_time{0};
};

#endif // ACTOR_H_
