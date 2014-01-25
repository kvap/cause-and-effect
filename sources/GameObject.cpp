#include "GameObject.h"

GameObject::GameObject(Point position, Point size)
    : position(position), size(size), angle(0.0f)
{ }

GameObject::GameObject(Point position, Point size, float angle)
    : position(position), size(size), angle(angle)
{ }

GameObject::~GameObject()
{
}

float GameObject::getAngle()
{
    return this->angle;
}

void GameObject::setAngle(float angle)
{
    this->angle = angle;
}

Point GameObject::getPosition()
{
    return this->position;
}

void GameObject::setPosition(Point position)
{
    this->position = position;
}

Point GameObject::getSize()
{
    return this->size;
}

void GameObject::setSize(Point size)
{
    this->size = size;
}
