#ifndef SOUND_H
#define SOUND_H

#include <AL/al.h>
#include <AL/alc.h>

class Sound
{
public:
	Sound(unsigned int sampleRate, short bitsPerSample, short channels, unsigned char* data, unsigned int dataSize);
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
