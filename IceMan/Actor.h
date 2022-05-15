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
public:
  Iceman(StudentWorld* stud_world) : Actor(IID_PLAYER, 30, 60, right, 1.0, 0), stud_world(stud_world) {}
  virtual ~Iceman() { delete stud_world; }
  virtual void doSomething() override;
  bool isAlive() const;
  StudentWorld* getWorld() const { return stud_world; }
private:
  StudentWorld* stud_world;
};


#endif // ACTOR_H_
