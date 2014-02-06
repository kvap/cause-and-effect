#include "Character.h"

#include <stdio.h>
#include <GLFW/glfw3.h>

#include "Input.hpp"
#include "ResourceManager.h"

Character::Character(Point position, Point size)
    : GameObject(position, size)
{
	sprite = new Sprite("char", 7);
	look_right = true;
	
	this->jumpSound = ResourceManager::getSound("jump");
	this->stepSound = ResourceManager::getSound("step_dirt");
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

	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 1, 1);
	sprite->draw(size.x * 1.2, size.y * 1.2, frame);
	glPopAttrib();
	glPopMatrix();
}

#define ANIM_SPEED 5
void Character::update(const GameTime* gameTime) {
	if (Input::isJustPressed(ACTION_JUMP)) {
		b2Vec2 velocity = this->getPhysics()->getBody()->GetLinearVelocity();
		this->getPhysics()->getBody()->SetLinearVelocity(b2Vec2(velocity.x, 5));
	}

	if (Input::isJustPressed(ACTION_LEFT) || Input::isJustPressed(ACTION_RIGHT)) {
		this->getPhysics()->setFriction(0.0f);
	}

	if (Input::isPressed(ACTION_LEFT)) {
		look_right = false;
		frame = (int)(gameTime->totalGameTime * ANIM_SPEED) % 4;
		b2Vec2 velocity = this->getPhysics()->getBody()->GetLinearVelocity();
		this->getPhysics()->getBody()->SetLinearVelocity(b2Vec2(-5, velocity.y));
	} else if (Input::isPressed(ACTION_RIGHT)) {
		look_right = true;
		frame = (int)(gameTime->totalGameTime * ANIM_SPEED) % 4;
		b2Vec2 velocity = this->getPhysics()->getBody()->GetLinearVelocity();
		this->getPhysics()->getBody()->SetLinearVelocity(b2Vec2(5, velocity.y));
	} else {
		frame = 4;
		if (this->getPhysics()->getFriction() < 20.0f) {
			this->getPhysics()->setFriction(20.0f);
		}
	}
}
