#include "SoundManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <al.h>
#include <alc.h>
#include <alut.h>

#include <map>
typedef std::map<std::string, Sound*> SoundMap;
SoundMap sounds;

#define SOUND_BUFFERS	10

static int soundId = -1;
static ALuint buffers[SOUND_BUFFERS];
static ALuint sources[SOUND_BUFFERS];

bool SoundManager::initSound()
{
	alutInit(0, NULL);
	alGetError();
	
	printf("Gen buffers\n");
	alGenBuffers(SOUND_BUFFERS, buffers);
	ALint error;
	if ((error = alGetError()) != AL_NO_ERROR) {
		printf("Can't gen buffers: %d\n", error);
		return false;
	}
/*
	printf("Gen sources\n");
	alGenSources(SOUND_BUFFERS, sources);
	if ((error = alGetError()) != AL_NO_ERROR) {
		printf("Can't gen source (%d): %s\n", error, alGetString(error));
		return false;
	}
*/
	return true;
}

Sound* loadSoundFromFile(std::string soundFilePath)
{
	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;

	printf("Loading sound file %d\n", ++soundId);
	alutLoadWAVFile((ALbyte*)(soundFilePath.c_str()), &format, &data, &size, &freq, &loop);
	alBufferData(buffers[soundId], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	ALint error;
	if ((error = alutGetError()) != ALUT_ERROR_NO_ERROR )
	{
		printf("Can't create buffer for sound file %d: %d\n", soundId, error);
		soundId--;
		return NULL;
	}

	// Bind buffer with a source.
	alGenSources(1, &sources[soundId]);
	if ((error = alGetError()) != AL_NO_ERROR) {
		printf("Can't gen source (%d): %s\n", error, alGetString(error));
		return NULL;
	}

	// Position of the source sound.
	ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
	// Velocity of the source sound.
	ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };

	alSourcei(sources[soundId], AL_BUFFER, buffers[soundId]);
	alSourcef(sources[soundId], AL_PITCH, 1.0f);
	alSourcef(sources[soundId], AL_GAIN, 1.0f);
	alSourcefv(sources[soundId], AL_POSITION, SourcePos);
	alSourcefv(sources[soundId], AL_VELOCITY, SourceVel);
	
	return new Sound(buffers[soundId], sources[soundId]);
}

Sound* SoundManager::loadSound(std::string soundName)
{
	std::string soundFilePath = "sounds/" + soundName;
	SoundMap::iterator it = sounds.find(soundFilePath);
	if (it != sounds.end())
		return it->second;
	printf("Loading sound %s\n", soundFilePath.c_str());
	sounds[soundFilePath] = loadSoundFromFile(soundFilePath);
	return  sounds[soundFilePath];
}
