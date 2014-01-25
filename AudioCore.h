//AudioCore.h
#ifndef AudioCore_h
#define AudioCore_h
#include "bass.h"// ���� ��� linux

bool InitAudio(); // ������������� �����
void FreeAudio(); 
class Audio{
public:
	Audio(char* path); // ���� � �����. � ����� � �������� �������
	void Play(); // ���������
	~Audio();
	char* path;
private:
	HSAMPLE sample;
};
#endif