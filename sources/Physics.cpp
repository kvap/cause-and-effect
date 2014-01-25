#include "Physics.h"

Physics::Physics(GameObject* gameObject, Physics::Type type)
    : type(type)
{
    this->objectBodyDef = new b2BodyDef();
    this->objectBodyDef->position.Set(gameObject->getPosition().x, gameObject->getPosition().y);

    this->objectPolygonShape = new b2PolygonShape();
    this->objectPolygonShape->SetAsBox(gameObject->getSize().x, gameObject->getSize().y);

    if (type == Physics::DYNAMIC)
    {
        this->objectBodyDef->type = b2_dynamicBody;

        this->objectFixtureDef = new b2FixtureDef();
        this->objectFixtureDef->shape = this->objectPolygonShape;
        this->objectFixtureDef->density = 10.5f;
        this->objectFixtureDef->restitution = 0.0f;
        this->objectFixtureDef->friction = 15.0f;
    }
}

Physics::~Physics()
{
    delete this->objectBodyDef;
    delete this->objectPolygonShape;
    if (this->type == Physics::DYNAMIC)
        delete this->objectFixtureDef;
}

void Physics::attach(b2World* physicsWorld)
{
    this->objectBody = physicsWorld->CreateBody(this->objectBodyDef);
    if (this->type == Physics::STATIC)
    {
        this->objectBody->CreateFixture(this->objectPolygonShape, 10.0f);
    }
    else // dynamic
    {
        this->objectBody->CreateFixture(this->objectFixtureDef);
    }
}

Point Physics::getPosition()
{
    b2Vec2 bodyPosition = this->objectBody->GetPosition();
    Point position(bodyPosition.x, bodyPosition.y);
    return position;
}

float32 Physics::getAngle()
{
    return this->objectBody->GetAngle();
}

b2Body* Physics::getBody()
{
	return this->objectBody;
}
