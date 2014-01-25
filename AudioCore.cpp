// Аудио ядро.
// Автор: Приказчиков Стеепан
// Для Game Jam

#include "AudioCore.h"
#pragma comment(lib,"bass.lib")// есть под linux



HCHANNEL channel;

bool InitAudio(){
	return BASS_Init(-1,48000,NULL,0,NULL);
}

void FreeAudio(){
	BASS_Free();
}

Audio::Audio(char* path){
	sample=BASS_SampleLoad(false,path,0,0,2,BASS_MUSIC_STOPBACK);
	Audio::path=path;
}


Audio::~Audio(){
	BASS_SampleFree(sample);
}


void Audio::Play(){
	channel=BASS_SampleGetChannel(sample,false);
	BASS_ChannelPlay(channel,false);

}