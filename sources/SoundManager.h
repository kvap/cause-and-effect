#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <string>
#include "Sound.h"

class SoundManager
{
private:
	SoundManager();
public:
	static bool initSound();
	static Sound* loadSound(std::string soundFile);
};

#endif // SOUND_MANAGER_H
