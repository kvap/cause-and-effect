#include "Sound.h"

#include <stdio.h>
#include <AL/al.h>
#include <AL/alc.h>

static ALCdevice* alcDevice;
static ALCcontext* alcContext;

void Sound::initialize()
{
	alcDevice = alcOpenDevice(NULL);
	if (!alcDevice) {
		printf("No sound device found\n");
		return;
	}

	alcContext = alcCreateContext(alcDevice, NULL);
	alcMakeContextCurrent(alcContext);
	if (!alcContext) {
		printf("No sound context\n");
	}
}

void Sound::terminate()
{
	alcMakeContextCurrent(NULL);
	alcDestroyContext(alcContext);
	alcCloseDevice(alcDevice);
}

Sound::Sound(unsigned int sampleRate, short bitsPerSample, short channels, unsigned char* data, unsigned int dataSize)
	: frequency(sampleRate), format(0)
{
	alGenBuffers(1, &this->buffer);
	alGenSources(1, &this->source);

	if (bitsPerSample == 8)
		this->format = (channels == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8);
	else if (bitsPerSample == 16)
		this->format = (channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);
	
	alBufferData(this->buffer, this->format, data, dataSize, this->frequency);

	ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
	ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };

	alListenerfv(AL_POSITION, ListenerPos);
	alListenerfv(AL_VELOCITY, ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);

	alSourcei(this->source, AL_BUFFER, this->buffer);
	alSourcef(this->source, AL_PITCH, 1.0f);
	alSourcef(this->source, AL_GAIN, 1.0f);
	alSourcefv(this->source, AL_POSITION, SourcePos);
	alSourcefv(this->source, AL_VELOCITY, SourceVel);
	alSourcei(this->source, AL_LOOPING, AL_FALSE);
}

Sound::~Sound()
{
	alDeleteSources(1, &this->source);
	alDeleteBuffers(1, &this->buffer);
}

void Sound::setLooping(bool looping)
{
	alSourcei(this->source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
}

void Sound::play() const
{
	alSourcePlay(this->source);
}

void Sound::pause() const
{
	alSourcePause(this->source);
}

void Sound::stop() const
{
	alSourceStop(this->source);
}
