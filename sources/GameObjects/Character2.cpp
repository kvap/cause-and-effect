#include "Character2.h"

#include <stdio.h>
#include <GLFW/glfw3.h>

#include "../input/Keyboard.hpp"
#include "../Input.hpp"

Character2::Character2(Point position, Point size)
    : GameObject(position, size)
{
	sprite = new Sprite("char", 7);
	look_right = true;
}

Character2::~Character2()
{ }

void Character2::draw(const GameTime* gameTime)
{
	glPushMatrix();
	glTranslatef(this->position.x, this->position.y, 0);
	if (!look_right) {
		glRotatef(180, 0, 1, 0);
	}

	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0, 0, 0);
	sprite->draw(size.x * 1.2, size.y * 1.2, frame);
	glPopAttrib();
	glPopMatrix();
}

#define ANIM_SPEED 5
void Character2::update(const GameTime* gameTime)
{
	if (Input::isJustPressed(JUMP))
	{
		b2Vec2 velocity = this->getPhysics()->getBody()->GetLinearVelocity();
		this->getPhysics()->getBody()->SetLinearVelocity(b2Vec2(velocity.x, 5));
	}/*
    if (Keyboard::keyIsFirstPressed(GLFW_KEY_SPACE))
    {
		b2Vec2 velocity = this->getPhysics()->getBody()->GetLinearVelocity();
		this->getPhysics()->getBody()->SetLinearVelocity(b2Vec2(velocity.x, 5));
	}*/

	if (Keyboard::keyIsJustPressed(GLFW_KEY_D) || Keyboard::keyIsJustPressed(GLFW_KEY_A))
		this->getPhysics()->setFriction(0.0f);

	if (Keyboard::keyIsPressed(GLFW_KEY_D))
	{
		look_right = false;
		frame = (int)(gameTime->totalGameTime * ANIM_SPEED) % 4;
		b2Vec2 velocity = this->getPhysics()->getBody()->GetLinearVelocity();
		this->getPhysics()->getBody()->SetLinearVelocity(b2Vec2(-5, velocity.y));
	}
	else if (Keyboard::keyIsPressed(GLFW_KEY_A))
	{
		look_right = true;
		frame = (int)(gameTime->totalGameTime * ANIM_SPEED) % 4;
		b2Vec2 velocity = this->getPhysics()->getBody()->GetLinearVelocity();
		this->getPhysics()->getBody()->SetLinearVelocity(b2Vec2(5, velocity.y));
	}
	else
	{
		frame = 4;
		if (this->getPhysics()->getFriction() < 20.0f)
			this->getPhysics()->setFriction(20.0f);
	}
}
