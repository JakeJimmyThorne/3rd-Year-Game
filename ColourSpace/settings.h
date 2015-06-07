#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <string>
#include <windows.h>


// --- CHANGABLE BY USER SETTINGS --- //

const static bool FULL_SCREEN = false;
const static bool VSYNC_ENABLED = true;

const static int WINDOWED_WIDTH = 1600;
const static int WINDOWED_HEIGHT = 900;



//values between 0 - 100
const static int MASTER_VOLUME = 0;




// --- NOT CHANGABLE BY USER SETTINGS --- //

const static LPCWSTR WINDOW_NAME = L"Colour Space";

const static float SCREEN_DEPTH = 1000.0f;
const static float SCREEN_NEAR = 0.1f;


//gameplay
const static bool GAMEPLAY_ENABLED = true;

//intro
const static int INTRO_LENGTH = 3;
const static bool INTRO_SKIP = false;

const static bool MENU_SKIP = false;

#if _DEBUG
static std::string DATA_PATH = "../ColourSpace/data/";
#else
static std::string DATA_PATH = "data/";
#endif

#endif