#include "ResourceManager.h"

#include <map>
#include <stdio.h>


typedef std::map<std::string, void*> ResourceMap;
static ResourceMap resourceMap;

// some boilerplate code
#define GETTER_IMPLEMENTATION(CLASS, PREFIX, SUFFIX) \
	CLASS *ResourceManager::get ## CLASS(std::string name) { \
		std::string filename = PREFIX + name + SUFFIX; \
		ResourceMap::iterator it = resourceMap.find(filename); \
		if (it != resourceMap.end()) { \
			return (CLASS*)it->second; \
		} \
		resourceMap[filename] = new CLASS(filename); \
		return (CLASS*)resourceMap[filename]; \
	}

//FIXME: use GETTER_IMPLEMENTATION for this too, when there is "new Sound()" style instead of "loadSound"
Sound* ResourceManager::getSound(std::string name) {
	std::string filename = "sounds/" + name + ".wav";
	ResourceMap::iterator it = resourceMap.find(filename);
	if (it != resourceMap.end()) {
		return (Sound*)it->second;
	}

	resourceMap[filename] = Sound::loadSound(filename);
	return (Sound*)resourceMap[filename];
}

GETTER_IMPLEMENTATION(Texture, "textures/", ".png");
GETTER_IMPLEMENTATION(GameScene, "scenes/", ".svg");
