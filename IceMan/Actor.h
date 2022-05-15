#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
  Actor(int imageID, int startX, int startY, StudentWorld* stud_world, Direction dir = right, double size = 1.0, unsigned int depth = 0)
    : GraphObject(imageID, startX, startY, dir, size, depth), stud_world(stud_world) {}
  virtual ~Actor() {}
  virtual void doSomething() = 0;
  StudentWorld* getWorld() { return stud_world; }
private:
  StudentWorld* stud_world;
};


class Ice : public Actor {
public:
  Ice(int startX, int startY, StudentWorld* stud_world)
    : Actor(IID_ICE, startX, startY, stud_world, right, 0.25, 3) {}
  virtual ~Ice() {}
  virtual void doSomething() override {}
};


class Iceman : public Actor {
public:
  Iceman(StudentWorld* stud_world) : Actor(IID_PLAYER, 30, 60, stud_world, right, 1.0, 0) {}
  virtual ~Iceman() {}
  virtual void doSomething() override;
private:
  
};


#endif // ACTOR_H_
