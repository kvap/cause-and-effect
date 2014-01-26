#include "Character.h"

#include <stdio.h>
#include <GLFW/glfw3.h>

#include "../Keyboard.h"
#include "../SoundManager.h"

Character::Character(Point position, Point size)
    : GameObject(position, size)
{
	this->jumpSound = SoundManager::loadSound("BounceYoFrankie.wav");
}

Character::~Character()
{ }

void Character::draw(const GameTime* gameTime)
{
    glPushMatrix();
    glTranslatef(this->position.x, this->position.y, 0);
    glRotatef(this->getAngle(), 0, 0, 1);
    glBegin(GL_QUADS);
    {
        glColor3f(1, 1, 1);
        glVertex2f(this->size.x, this->size.y);
        glVertex2f(-this->size.x, this->size.y);
        glVertex2f(-this->size.x, -this->size.y);
        glVertex2f(this->size.x, -this->size.y);
    }
    glEnd();
    glPopMatrix();
}

void Character::update(const GameTime* gameTime)
{
    if (Keyboard::keyIsFirstPressed(GLFW_KEY_SPACE))
    {
		b2Vec2 velocity = this->getPhysics()->getBody()->GetLinearVelocity();
		this->getPhysics()->getBody()->SetLinearVelocity(b2Vec2(velocity.x, 65));
		this->jumpSound->play();
	}
	if (Keyboard::keyIsPressed(GLFW_KEY_D))
	{
		b2Vec2 velocity = this->getPhysics()->getBody()->GetLinearVelocity();
		this->getPhysics()->getBody()->SetLinearVelocity(b2Vec2(50, velocity.y));
	}
	if (Keyboard::keyIsPressed(GLFW_KEY_A))
	{
		b2Vec2 velocity = this->getPhysics()->getBody()->GetLinearVelocity();
		this->getPhysics()->getBody()->SetLinearVelocity(b2Vec2(-50, velocity.y));
	}
}
