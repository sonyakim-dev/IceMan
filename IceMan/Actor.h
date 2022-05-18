#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <cmath>

class StudentWorld; // dsdfsdf

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
  Actor(int imageID, int startX, int startY, StudentWorld* stud_world, Direction dir = right, double size = 1.0, unsigned int depth = 0)
    : stud_world(stud_world), GraphObject(imageID, startX, startY, dir, size, depth) {}
  virtual ~Actor() {}
  virtual void doSomething() = 0;
  virtual StudentWorld* getWorld() const { return stud_world; }

private:
    StudentWorld* stud_world;
};


class Ice : public Actor {
public:
  Ice(int startX, int startY, StudentWorld* stud_world)
    : Actor(IID_ICE, startX, startY, stud_world, none, 0.25, 3) {}
  virtual ~Ice() {}
  virtual void doSomething() override {}
};


class Iceman : public Actor {
private:
    StudentWorld* stud_world; // to get ice_man
    int i_hitPoints;
    int i_waters;
    int i_golds;
    int i_sonars;
public:
  Iceman(StudentWorld* stud_world) : i_hitPoints(100), i_waters(5), i_golds(0), i_sonars(1),
      Actor(IID_PLAYER, 30, 60, stud_world, right, 1.0, 0), stud_world(stud_world) {}
  virtual ~Iceman() { }
  virtual void doSomething() override;
  bool isAlive() const;
  void addGold() { i_golds += 1; }
  int getHP(int hitpoints) { return i_hitPoints = hitpoints; }
  int getWater() { return i_waters; }
  int getGold() { return i_golds; }
  int getSonars() { return i_sonars; }
};

class Goodies : public Actor {
public:
    Goodies(int imageID, int startX, int startY, StudentWorld* stud_world,  Direction dir = right, double size = 1.0, unsigned int depth = 0)
        : Actor(imageID, startX, startY, stud_world, dir, size, depth){}
    virtual ~Goodies() {}
    virtual void doSomething() override {}
    virtual bool isInRange(const unsigned int& x, const unsigned int& y) = 0;
private:
};

class Gold : public Goodies {
public:
    Gold(int startX, int startY, StudentWorld* stud_world)
        : Goodies(IID_GOLD, startX, startY, stud_world, right, 1, 2) {}
    virtual ~Gold() {}
    virtual void doSomething() override;

    //later in future fix it once we do the protestors
    bool isInRange(const unsigned int& x, const unsigned int& y) override;
};


#endif // ACTOR_H_
