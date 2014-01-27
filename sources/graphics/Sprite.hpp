/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <string>
#include <GLFW/glfw3.h>

#include "Textures.hpp"

class Sprite {
	private:
		int frames;
		Texture *texture; // sprite texture
	public:
		Sprite(const std::string textureName, int frames);
		void draw(double halfwidth, double halfheight, int frame);
};

#endif

