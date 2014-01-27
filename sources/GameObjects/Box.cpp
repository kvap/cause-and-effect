#include "Box.h"

#include <stdio.h>
#include <GLFW/glfw3.h>

#include "Keyboard.h"
#include "ResourceManager.h"

Box::Box(Point position, Point size)
    : GameObject(position, size)
{ }

Box::~Box()
{ }

void Box::draw(const GameTime* gameTime)
{
	glPushMatrix();
	glTranslatef(this->position.x, this->position.y, 0);
	glRotatef(this->getAngle(), 0, 0, 1);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Texture *groundtex = ResourceManager::getTexture("ground");
	groundtex->apply();


	#define GROUND_SCALE 0.2
	double u0 = GROUND_SCALE * (position.x - size.x);
	double v0 = GROUND_SCALE * (position.y - size.y);
	double u1 = GROUND_SCALE * (position.x + size.x);
	double v1 = GROUND_SCALE * (position.y + size.y);
	glBegin(GL_QUADS);
	glTexCoord2d(u0, v0); glVertex2d(-size.x, -size.y);
	glTexCoord2d(u0, v1); glVertex2d(-size.x,  size.y);
	glTexCoord2d(u1, v1); glVertex2d( size.x,  size.y);
	glTexCoord2d(u1, v0); glVertex2d( size.x, -size.y);
	glEnd();
	glPopMatrix();
}

void Box::update(const GameTime* gameTime)
{
}
