#ifndef SCENE_LAYER_H
#define SCENE_LAYER_H

#include <vector>
#include <Box2D/Box2D.h>

#include "GameTime.h"
#include "GameObject.h"
#include "Physics.h"

class SceneLayer
{
public:
    SceneLayer();
    ~SceneLayer();
private:
    b2Vec2* gravity;
    b2World* physicsWorld;
protected:
    std::vector<GameObject*> gameObjects;
    std::vector<Physics*> gameObjectPhysics;
public:
    void add(GameObject* gameObject, Physics::Type type);
    //void remove(GameObject* gameObject);

    void draw(const GameTime* gameTime);
    void update(const GameTime* gameTime);
    void updatePhysics();
};

#endif // SCENE_LAYER_H
