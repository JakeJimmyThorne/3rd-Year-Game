#ifndef _SYSTEM_H_
#define _SYSTEM_H_

//Strip windows.h down to get rid of junk
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <process.h>
#include "settings.h"
#include "menuscene.h"
#include "imagecrawl.h"


class System
{
public:
	System();
	System(const System&);
	~System();

	bool Initialise();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitialiseWindows(int&, int&);
	void ShutdownWindows();

	void QueryGoogle();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	ImageCrawl m_ImageCrawl;
	void* m_crawlPtr;

	MenuScene* m_Application;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static System* ApplicationHandle = 0;


#endif