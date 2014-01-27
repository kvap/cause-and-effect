/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

/* 
 * Provides functions to load/unload OpenGL textures from PNG It memorizes what
 * textures you load and does not load the same file again.
 */

#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include <GLFW/glfw3.h>
#include <string>

class Texture {
	private:
		unsigned int handle; // OpenGL handle
		int width, height;
	public:
		int getWidth();
		int getHeight();
		// FIXME: get rid of "getHandle"?
		unsigned int getHandle(); // returns the OpenGL handle
		void apply();
		Texture(std::string filename);
		~Texture();
};

#endif

