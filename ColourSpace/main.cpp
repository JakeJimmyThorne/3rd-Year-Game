#include "system.h"

#if _DEBUG
int main()
{
	System* system;
	bool result;

	system = new System;
	if(!system)
	{
		return 0;
	}

	result = system->Initialise();
	if(result)
	{
		system->Run();
	}

	system->Shutdown();
	delete system;
	system = 0;

	return 0;
}
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	System* system;
	bool result;

	system = new System;
	if(!system)
	{
		return 0;
	}

	result = system->Initialise();
	if(result)
	{
		system->Run();
	}

	system->Shutdown();
	delete system;
	system = 0;

	return 0;
}
#endif