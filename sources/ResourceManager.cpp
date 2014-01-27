#include "ResourceManager.h"

#include <map>
#include <stdio.h>


typedef std::map<std::string, void*> ResourceMap;
static ResourceMap resourceMap;

Sound* ResourceManager::getSound(std::string name) {
	std::string filename = "sounds/" + name + ".wav";
	ResourceMap::iterator it = resourceMap.find(filename);
	if (it != resourceMap.end()) {
		return (Sound*)it->second;
	}

	resourceMap[filename] = Sound::loadSound(filename);
	return (Sound*)resourceMap[filename];
}

Texture *ResourceManager::getTexture(std::string name) {
	std::string filename = "textures/" + name + ".png";
	ResourceMap::iterator it = resourceMap.find(filename);
	if (it != resourceMap.end()) {
		return (Texture*)it->second;
	}

	resourceMap[filename] = new Texture(filename);
	return (Texture*)resourceMap[filename];
}
