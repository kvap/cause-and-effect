#include "Character.h"

#include <stdio.h>
#include <GLFW/glfw3.h>

#include "../Keyboard.h"

Character::Character(Point position, Point size)
    : GameObject(position, size)
{
	sprite = new Sprite("char.png", 7);
	look_right = true;
}

Character::~Character()
{ }

void Character::draw(const GameTime* gameTime)
{
	glPushMatrix();
	glTranslatef(this->position.x, this->position.y, 0);
	if (!look_right) {
		glRotatef(180, 0, 1, 0);
	}

	sprite->draw(size.x * 1.2, size.y * 1.2, frame);
	glPopMatrix();
}

#define ANIM_SPEED 5
void Character::update(const GameTime* gameTime)
{
    if (Keyboard::keyIsFirstPressed(GLFW_KEY_SPACE))
    {
		b2Vec2 velocity = this->getPhysics()->getBody()->GetLinearVelocity();
		this->getPhysics()->getBody()->SetLinearVelocity(b2Vec2(velocity.x, 5));
	}
	if (Keyboard::keyIsPressed(GLFW_KEY_D))
	{
		look_right = true;
		this->getPhysics()->setFriction(0.0f);
		frame = (int)(gameTime->totalGameTime * ANIM_SPEED) % 4;
		b2Vec2 velocity = this->getPhysics()->getBody()->GetLinearVelocity();
		this->getPhysics()->getBody()->SetLinearVelocity(b2Vec2(5, velocity.y));
	}
	else if (Keyboard::keyIsPressed(GLFW_KEY_A))
	{
		look_right = false;
		this->getPhysics()->setFriction(0.0f);
		frame = (int)(gameTime->totalGameTime * ANIM_SPEED) % 4;
		b2Vec2 velocity = this->getPhysics()->getBody()->GetLinearVelocity();
		this->getPhysics()->getBody()->SetLinearVelocity(b2Vec2(-5, velocity.y));
	}
	else
	{
		frame = 4;
		this->getPhysics()->setFriction(10.0f);
	}
}
