#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>

#include "Sound.h"

class ResourceManager
{
public:
	static Sound* getSound(std::string soundName);
};

#endif // RESOURCE_MANAGER_H
