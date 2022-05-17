#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
  Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
    : GraphObject(imageID, startX, startY, dir, size, depth) {}
  virtual ~Actor() {}
  virtual void doSomething() = 0;
private:
};


class Ice : public Actor {
public:
  Ice(int startX, int startY)
    : Actor(IID_ICE, startX, startY, none, 0.25, 3) {}
  virtual ~Ice() {}
  virtual void doSomething() override {}
};


class Iceman : public Actor {
private:
    StudentWorld* stud_world;
    int i_hitPoints;
    int i_waters;
    int i_golds;
    int i_sonars;
public:

    Iceman(StudentWorld* stud_world) : i_hitPoints(100), i_waters(5), i_golds(0), i_sonars(1),
      Actor(IID_PLAYER, 30, 60, right, 1.0, 0), stud_world(stud_world) {}
  virtual ~Iceman() { delete stud_world; }
  virtual void doSomething() override;
  bool isAlive() const;
  StudentWorld* getWorld() const { return stud_world; }
  int getHP(int hitpoints) { return i_hitPoints = hitpoints; }
  int getWater() { return i_waters; }
  int getGold() { return i_golds; }
  int getSonars() { return i_sonars; }
};

class Goodies : public Actor {
public:
    Goodies(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) 
        : Actor(imageID, startX, startY, dir, size, depth){}
    virtual ~Goodies() {}
    virtual void doSomething() {};
private:
};

class Gold : public Goodies {
public:
    Gold(int startX, int startY) 
        : Goodies(IID_GOLD, startX, startY, none, 1, 0) {}
    ~Gold() {}
    void doSomething() override {};
};


#endif // ACTOR_H_
