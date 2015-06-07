#ifndef _SOUND_H_
#define _SOUND_H_

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//#include DATA_PATH + "settings.h"

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <string>

class Sound
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	Sound();
	~Sound();

	bool Initialize(const char*,HWND,int);
	void Shutdown();

	bool PlayOnce();
	bool PlayInLoop();

	std::string GetName() { return m_name; }
	void SetName(std::string name) { m_name = name; }

	bool Stop();

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(const char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

	bool PlayWaveFile();
	bool PlayLoop();

private:
	std::string m_name;
	IDirectSound8* m_DirectSound;

	//The primary buffer main buffer for the sound card, headset etc
	IDirectSoundBuffer* m_primaryBuffer;

	//load sounds into the secondary buffer
	IDirectSoundBuffer8* m_secondaryBuffer1;

	int m_volume;
};

#endif