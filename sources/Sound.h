#ifndef SOUND_H
#define SOUND_H

#include <al.h>

class Sound
{
public:
	Sound(ALuint buffer, ALuint source);
	~Sound();
private:
	// Buffers hold sound data.
	ALuint buffer;
	// Sources are points emitting sound.
	ALuint source;
protected:

public:
	void play();
	void pause();
	void stop();
};

#endif // SOUND_H
