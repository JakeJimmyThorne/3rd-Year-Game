#include "soundobject.h"

SoundObject::SoundObject()
{

}

SoundObject::~SoundObject()
{

}

bool SoundObject::Initialise(const char* filename, HWND* hwnd)
{
	bool result;

	result = AddSound(filename, hwnd);
	if(!result)
		return false;

	return true;
}

void SoundObject::Shutdown()
{
	GameObject::Shutdown();
}
