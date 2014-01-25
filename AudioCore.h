//AudioCore.h
#ifndef AudioCore_h
#define AudioCore_h
#include "bass.h"// есть под linux

bool InitAudio(); // инициализация аудио
void FreeAudio(); 
class Audio{
public:
	Audio(char* path); // путь к файлу. В винде с двойными слэшами
	void Play(); // проиграть
	~Audio();
	char* path;
private:
	HSAMPLE sample;
};
#endif