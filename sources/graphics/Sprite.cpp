/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#include "Sprite.hpp"
#include "ResourceManager.h"

#include <GLFW/glfw3.h>

Sprite::Sprite(const std::string textureName, int frames) {
	texture = ResourceManager::getTexture(textureName);
	this->frames = frames;
}

void Sprite::draw(double halfwidth, double halfheight, int frame) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	texture->apply();
	glBegin(GL_QUADS);
	double u0 = (double)(frame % frames) / frames;
	double u1 = (double)((frame + 1) % frames) / frames;
		glTexCoord2d(u0, 0); glVertex2d(-halfwidth, -halfheight);
		glTexCoord2d(u0, 1); glVertex2d(-halfwidth,  halfheight);
		glTexCoord2d(u1, 1); glVertex2d( halfwidth,  halfheight);
		glTexCoord2d(u1, 0); glVertex2d( halfwidth, -halfheight);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
