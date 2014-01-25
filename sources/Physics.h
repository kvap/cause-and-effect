#ifndef PHYSICS_H
#define PHYSICS_H

#include <Box2D/Box2D.h>

#include "Point.h"
#include "GameObject.h"

class Physics
{
public:
    enum Type {
        STATIC = 0,
        DYNAMIC = 1
    };

    Physics(GameObject* gameObject, Physics::Type type);
    ~Physics();

private:
    Type type;

    b2BodyDef* objectBodyDef;
    b2PolygonShape* objectPolygonShape;
    b2Body* objectBody;
    b2FixtureDef* objectFixtureDef;
public:
    void attach(b2World* physicsWorld);

    Point getPosition();
    float32 getAngle();
};

#endif // PHYSICS_H