#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <cmath>

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
  Actor(int imageID, int startX, int startY, StudentWorld* stud_world, Direction dir = right, double size = 1.0, unsigned int depth = 0)
    : GraphObject(imageID, startX, startY, dir, size, depth), stud_world(stud_world) {}
  virtual ~Actor() {}
  virtual void doSomething() = 0;
  virtual StudentWorld* getWorld() const { return stud_world; }
  bool isActive() const { return active; }
  void setUnactive() { active = false; }
  void setActive() { active = true; }
private:
  bool active = true;
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
  unsigned int i_hitPoints {100};
  unsigned int i_waters {5};
  unsigned int i_golds {0};
  unsigned int i_sonars {1};
public:
  Iceman(StudentWorld* stud_world) :
    Actor(IID_PLAYER, 30, 60, stud_world, right, 1.0, 0), stud_world(stud_world) {}
  virtual ~Iceman() { }
  virtual void doSomething() override;
  void addGold() { ++i_golds; }
  void addWater() { i_waters += 5; }
  void addSonar() { ++i_sonars; }
  int getHP(int hitpoints) { return i_hitPoints = hitpoints; }
  int getGold() const { return i_golds; }
  int getWater() const { return i_waters; }
  int getSonars() const { return i_sonars; }
};

class Goodies : public Actor {
public:
    Goodies(int imageID, int startX, int startY, StudentWorld* stud_world,  Direction dir = right, double size = 1.0, unsigned int depth = 0)
        : Actor(imageID, startX, startY, stud_world, dir, size, depth){}
    virtual ~Goodies() {}
    virtual void doSomething() override {}
    virtual bool isInRange(const unsigned int& x, const unsigned int& y, const float& radius);
private:
};

class Oil : public Goodies {
public:
    Oil(int startX, int startY, StudentWorld* stud_world)
        : Goodies(IID_BARREL, startX, startY, stud_world, right, 1, 2) {}
    virtual ~Oil() {}
    virtual void doSomething() override;
};

class Gold : public Goodies {
public:
    Gold(int startX, int startY, StudentWorld* stud_world)
        : Goodies(IID_GOLD, startX, startY, stud_world, right, 1, 2) {}
    virtual ~Gold() {}
    virtual void doSomething() override;
    //later in future fix it once we do the protestors
//    virtual bool isInRange(const unsigned int x, const unsigned int y, const float& radius) override;
};

class Sonar : public Goodies {
public:
    Sonar(int startX, int startY, StudentWorld* stud_world)
        : Goodies(IID_SONAR, startX, startY, stud_world, right, 1, 2) {}
    virtual ~Sonar() {}
    virtual void doSomething() override;
};

class Boulder : public Goodies {
private:
public:
    Boulder(int startX, int startY, StudentWorld* stud_world)
        : Goodies(IID_BOULDER, startX, startY, stud_world, down, 1, 1) {}
    virtual ~Boulder() {}
    virtual void doSomething() override;
    bool isStable();
};

#endif // ACTOR_H_
