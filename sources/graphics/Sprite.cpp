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
#include "Textures.hpp"

#include <GLFW/glfw3.h>

Sprite::Sprite(const std::string filename, int frames) {
	texture = loadTexture(filename);
	this->frames = frames;
}

void Sprite::draw(double halfwidth, double halfheight, int frame) {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	double u0 = (double)(frame % frames) / frames;
	double u1 = (double)((frame + 1) % frames) / frames;
	glColor3f(1, 1, 1);
		glTexCoord2d(u0, 0); glVertex2d(-halfwidth, -halfheight);
		glTexCoord2d(u0, 1); glVertex2d(-halfwidth,  halfheight);
		glTexCoord2d(u1, 1); glVertex2d( halfwidth,  halfheight);
		glTexCoord2d(u1, 0); glVertex2d( halfwidth, -halfheight);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
