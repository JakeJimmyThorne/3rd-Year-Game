#ifndef _FPS_H_
#define _FPS_H_

#pragma comment(lib, "winmm.lib")


#include <windows.h>
#include <mmsystem.h>

class Fps
{
public:
	Fps();
	~Fps();

	void Initialize();
	void Frame();
	int GetFps();
	float GetDeltaTime();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};

#endif