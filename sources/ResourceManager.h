#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>

#include "Sound.h"
#include "graphics/Textures.hpp"

class ResourceManager
{
public:
	static Sound* getSound(std::string soundName);
	static Texture *getTexture(std::string textureName);
};

#endif // RESOURCE_MANAGER_H
