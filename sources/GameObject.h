#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class Physics;

#include "Physics.h"
#include "GameTime.h"
#include "Point.h"

class GameObject
{
public:
    GameObject(Point position, Point size);
    GameObject(Point position, Point size, float angle);
    ~GameObject();
private:
    Physics* physics;
protected:
    float angle;
    Point position;
    Point size;
public:
    float getAngle();
    void setAngle(float angle);
    Point getPosition();
    void setPosition(Point point);
    Point getSize();
    void setSize(Point size);
    Physics* getPhysics();
    void setPhysics(Physics* p);

    virtual void draw(const GameTime* gameTime) = 0;
    virtual void update(const GameTime* gameTime) = 0;
};

#endif // GAME_OBJECT_H
