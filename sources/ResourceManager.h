#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>

#include "Sound.h"
#include "GameScene.h"
#include "graphics/Textures.hpp"

class ResourceManager
{
public:
	static Sound* getSound(std::string soundName);
	static Texture *getTexture(std::string textureName);
	static GameScene *getGameScene(std::string sceneName);
};

#endif // RESOURCE_MANAGER_H
