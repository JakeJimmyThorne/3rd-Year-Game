#ifndef _MENUSCENE_H_
#define _MENUSCENE_H_

#include <vector>
#include "settings.h"

#include "Rendering\renderer.h"

#include "d3dclass.h"
#include "inputmanager.h"
#include "Camera.h"
#include "gamescene.h"

#include "uiobject.h"
#include "imagecrawl.h"

#include "fps.h"
#include <SFML/Audio.hpp>

class MenuScene
{
public:

	static bool IMAGES_DOWNLOADED;

public:

	enum State 
	{ 
		SPLASH,

		MENU,
		MENU_IMAGE_SELECTION,
		GAME,
		SHUTDOWN
	};

public:
	MenuScene();
	~MenuScene();

	bool Initialise(HINSTANCE*, HWND*, int, int);
	bool Frame();

	void Shutdown();

private:

	bool Render(); //Calls to the relevant render function
	bool RenderSplash();
	bool RenderMenu();
	bool RenderGame();

	//main update loops
	bool UpdateState();
	bool UpdateUI();

	void SetState(State);

	//test to see if the function failed
	void Try(bool);

private:
	D3DXMATRIX m_worldMatrix, m_viewMatrix, m_projectionMatrix, m_orthoMatrix;
	D3DXMATRIX m_uiViewMatrix;

	Renderer m_Renderer;

	Fps m_frameRate;
	Camera* m_Camera;
	D3DClass* m_Device;

	State* m_state;

	GameScene* m_GameScene;

	HWND* m_hwnd;

	ImageCrawl m_ImageCrawl;
	

	//sounds
	
	sf::SoundBuffer m_introSoundBuffer;
	sf::SoundBuffer m_startupSoundBuffer;
	sf::Sound m_IntroSound;
	sf::Sound m_StartupBeep;

	//Splash screen
	float m_introTimer;

	#pragma region UI VARIABLES

	std::vector<UIObject*> m_uiList;
	float m_screenWidth, m_screenHeight;

	//Mouse variables for the UI
	InputManager* m_Input;
	int m_mouseX = 0, m_mouseY = 0;
	bool m_mouseSelect;
	bool m_menuStarted;
	bool m_exit;

	//list ui objects normally
	UIObject* m_Background;
	UIObject* m_DropdownFile;
	UIObject* m_ExplorerWindow;
	UIObject* m_InfoScreen;
	UIObject* m_Tutorial;
	UIObject* m_Controls;
	UIObject* m_SplashScreen;

	UIObject *m_LoadingImages, *m_LoadedImages;

	UIObject* m_ImageSelect;
	int m_imageSelected;

	UIObject* m_TopBar;

	ID3D11ShaderResourceView* m_menuOriginal;
	ID3D11ShaderResourceView* m_selectOpen;
	ID3D11ShaderResourceView* m_selectInfo;
	ID3D11ShaderResourceView* m_selectEject;
	bool m_gameReturn;

	#pragma endregion 
};

#endif