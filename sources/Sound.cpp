#include "Sound.h"

Sound::Sound(ALuint buffer, ALuint source)
	: buffer(buffer), source(source)
{
	// Position of the listener.
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
	// Velocity of the listener.
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
	// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

	alListenerfv(AL_POSITION,    ListenerPos);
	alListenerfv(AL_VELOCITY,    ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}

Sound::~Sound()
{
}

void Sound::play()
{
	alSourcePlay(this->source);
}

void Sound::pause()
{
	alSourcePause(this->source);
}

void Sound::stop()
{
	alSourceStop(this->source);
}
