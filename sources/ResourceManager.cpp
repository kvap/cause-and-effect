#include "ResourceManager.h"

#include <map>
#include <stdio.h>

typedef unsigned int DWORD;
static Sound* loadSound(std::string soundFile)
{
	FILE* file = fopen(soundFile.c_str(), "rb");
	if (!file) {
		printf("Can't open file `%s`.", soundFile.c_str());
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
		printf("No RIFF\n");
		return NULL;
	}

	fread(&size, sizeof(DWORD), 1, file);
	fread(type, sizeof(char), 4, file);
	if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E') {
		printf("No WAVE\n");
		return NULL;
	}

	fread(type, sizeof(char), 4, file);
	if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ') {
		printf("No fmt \n");
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
		printf("No data\n");
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

typedef std::map<std::string, Sound*> SoundMap;
static SoundMap soundMap;

Sound* ResourceManager::getSound(std::string soundName)
{
	std::string soundFile = "sounds/" + soundName + ".wav";
	SoundMap::iterator it = soundMap.find(soundFile);
	if (it != soundMap.end())
		return it->second;

	soundMap[soundFile] = loadSound(soundFile);
	return soundMap[soundFile];
}
