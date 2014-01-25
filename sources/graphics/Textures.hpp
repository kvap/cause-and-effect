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

// Initializes texture loading module. Call this function after
// initializing graphics, but before any texture loading
// functions of this module.
void initTextures();

// Loads texture from file named "filename" and returns OpenGL
// handler for it. If you try to load from the same file again,
// it will just return existing handler, without creating yet
// another texture.
unsigned int loadTexture(std::string filename, int *wid, int *hei);
unsigned int loadTexture(std::string filename);

// Unloads OpenGL texture, that was loaded from file "filename" 
void unloadTexture(std::string filename);

// Unloads all textures, loaded by LoadTex.
void wipeTextures();

#endif

