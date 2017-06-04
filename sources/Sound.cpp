#include "Sound.h"

#include <stdio.h>

static ALCdevice* alcDevice;
static ALCcontext* alcContext;

void Sound::initialize()
{
	alcDevice = alcOpenDevice(NULL);
	if (!alcDevice) {
		LOG_ERROR("No sound device found");
		return;
	}

	alcContext = alcCreateContext(alcDevice, NULL);
	alcMakeContextCurrent(alcContext);
	if (!alcContext) {
		LOG_ERROR("No sound context");
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

typedef unsigned int DWORD;
Sound *Sound::loadSound(std::string filename) {
	FILE* file = fopen(filename.c_str(), "rb");
	if (!file) {
		LOG_ERROR("Can't open file '" + filename + "'");
		return NULL;
	}

	char type[4];
	DWORD size, chunkSize;
	short formatType, channels;
	DWORD sampleRate, avgBytesPerSecond;
	short bytesPerSample, bitsPerSample;
	DWORD dataSize;

	fread(type, sizeof(char), 4, file);
	if (type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F') {
		LOG_ERROR("No RIFF");
		return NULL;
	}

	fread(&size, sizeof(DWORD), 1, file);
	fread(type, sizeof(char), 4, file);
	if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E') {
		LOG_ERROR("No WAVE");
		return NULL;
	}

	fread(type, sizeof(char), 4, file);
	if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ') {
		LOG_ERROR("No fmt");
		return NULL;
	}

	fread(&chunkSize, sizeof(DWORD), 1, file);
	fread(&formatType, sizeof(short), 1, file);
	fread(&channels, sizeof(short), 1, file);
	fread(&sampleRate, sizeof(DWORD), 1, file);
	fread(&avgBytesPerSecond, sizeof(DWORD), 1, file);
	fread(&bytesPerSample, sizeof(short), 1, file);
	fread(&bitsPerSample, sizeof(short), 1, file);

	fread(type, sizeof(char), 4, file);
	if (type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a') {
		LOG_ERROR("No data");
		return NULL;
	}

	fread(&dataSize, sizeof(DWORD), 1, file);
	unsigned char* buffer = new unsigned char[dataSize];
	fread(buffer, sizeof(unsigned char), dataSize, file);

	Sound* s = new Sound(sampleRate, bitsPerSample, channels, buffer, dataSize);

	fclose(file);
	delete [] buffer;

	return s;
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
