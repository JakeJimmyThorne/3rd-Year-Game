#ifndef _SOUNDOBJECT_H_
#define _SOUNDOBJECT_H_

#include <d3d11.h>
#include <d3dx10math.h>

#include "gameobject.h"

class SoundObject : public GameObject
{
public:
	SoundObject();
	~SoundObject();

	bool Initialise(const char*,HWND*);
	void Shutdown();

	bool PlayOnce() { return GetSound()->PlayOnce(); }
	bool PlayInLoop() { return GetSound()->PlayInLoop(); }

	bool Stop() { return GetSound()->Stop(); }
};

#endif