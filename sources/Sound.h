#ifndef SOUND_H
#define SOUND_H

#include <string>
#include <AL/al.h>
#include <AL/alc.h>

#include "util/Logger.hpp"

class Sound
{
public:
	Sound(unsigned int sampleRate, short bitsPerSample, short channels, unsigned char* data, unsigned int dataSize);
	static Sound *loadSound(std::string filename);
	~Sound();
private:
	ALuint source;
	ALuint buffer;
	ALuint frequency;
	ALenum format;
protected:

public:
	static void initialize();
	static void terminate();

	void setLooping(bool looping);

	void play() const;
	void pause() const;
	void stop() const;
};

#endif // SOUND_H
