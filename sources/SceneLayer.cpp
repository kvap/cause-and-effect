#include "SceneLayer.h"

using namespace std;

SceneLayer::SceneLayer()
{
    this->gravity = new b2Vec2(0.0f, -9.8f);
    this->physicsWorld = new b2World(*this->gravity);
}

SceneLayer::~SceneLayer()
{
    delete this->gravity;
    delete this->physicsWorld;
}

void SceneLayer::add(GameObject* gameObject, Physics::Type type)
{
    Physics* physics = new Physics(gameObject, type);
    physics->attach(this->physicsWorld);
	gameObject->setPhysics(physics);

    this->gameObjects.push_back(gameObject);
    this->gameObjectPhysics.push_back(physics);
}

void SceneLayer::draw(const GameTime* gameTime)
{
    for (vector<GameObject*>::iterator it = this->gameObjects.begin(); it != this->gameObjects.end(); ++it)
    {
        (*it)->draw(gameTime);
    }
}

void SceneLayer::update(const GameTime* gameTime)
{
    for (vector<GameObject*>::iterator it = this->gameObjects.begin(); it != this->gameObjects.end(); ++it)
    {
        (*it)->update(gameTime);
    }
}

void SceneLayer::updatePhysics()
{
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    this->physicsWorld->Step(timeStep, velocityIterations, positionIterations);
    for (int i = 0; i < this->gameObjects.size(); ++i)
    {
        this->gameObjects[i]->setPosition(this->gameObjectPhysics[i]->getPosition());
        this->gameObjects[i]->setAngle(this->gameObjectPhysics[i]->getAngle());
    }
}
