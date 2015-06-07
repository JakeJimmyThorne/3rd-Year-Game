#include "menuscene.h"
#include <iostream>
#include <time.h>

bool MenuScene::IMAGES_DOWNLOADED = false;

MenuScene::MenuScene()
{
	//Set to null
	m_state = 0;

	m_Device = 0;
	m_Input = 0;

	m_Camera = 0;

	m_GameScene = 0;

	m_DropdownFile = 0;
	m_ExplorerWindow = 0;
	m_introTimer = 0;

	m_TopBar = 0;
	m_InfoScreen = 0;

	m_gameReturn = true;
}

MenuScene::~MenuScene()
{
}


bool MenuScene::Initialise(HINSTANCE* hinstance, HWND* hwnd, int screenWidth, int screenHeight)
{
	bool result;
	srand(time(NULL));

	m_hwnd = hwnd;

	if (!INTRO_SKIP)
	{
#if _DEBUG
		if (!m_introSoundBuffer.loadFromFile(
			"../ColourSpace/data/SFX/128k-StartupOGG.ogg")) {
			return false;
		}

		if (!m_startupSoundBuffer.loadFromFile(
			"../ColourSpace/data/BGM/startUpSoundOGG.ogg")) {
			return false;
		}
#else
		if (!m_introSoundBuffer.loadFromFile(
			"data/SFX/128k-StartupOGG.ogg")) {
			return false;
		}

		if (!m_startupSoundBuffer.loadFromFile(
			"data/BGM/startUpSoundOGG.ogg")) {
			return false;
		}
#endif

		m_IntroSound.setBuffer(m_introSoundBuffer);
		m_StartupBeep.setBuffer(m_startupSoundBuffer);

		m_IntroSound.play();
		m_StartupBeep.play();
	}

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_frameRate.Initialize();

	m_exit = false;
	m_mouseSelect = false;
	m_menuStarted = false;

	GameObject::s_hinstance = hinstance;
	GameObject::s_hwnd = m_hwnd;

	#if _DEBUG
	std::cout << "Initialising menuscene..\n\n";
	#endif

	//Initialise states
	m_state = new State;

	SetState(SPLASH);

	#pragma region Create & initialise the Direct3D object.

	#if _DEBUG
	std::cout << "Creating D3D...";
	#endif

	m_Device = new D3DClass;

	if(!m_Device)
		return false;

	result = m_Device->Initialise(screenWidth, screenHeight, VSYNC_ENABLED, *m_hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	if(!result)
	{
		MessageBox(*m_hwnd, L"Could not initialise Direct3D.", L"Error", MB_OK);
		return false;
	}

	m_Renderer.Initialise(m_Device->GetDeviceContext(), m_Device->GetDevice(), *m_hwnd, &m_worldMatrix, &m_viewMatrix, &m_projectionMatrix);

	#if _DEBUG
	std::cout << "     Success!\n";
	#endif

	#pragma endregion

	#pragma region Create & initialise the input object.
	m_Input = new InputManager;

	if(!m_Input)
		return false;

	result = m_Input->Initialise(*hinstance, *m_hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(*m_hwnd, L"Could not initialise the input object.", L"Error", MB_OK);
		return false;
	}

	#pragma endregion

	#pragma region Create & initialise the camera object.

	#if _DEBUG
	std::cout << "Creating Camera...";
	#endif

	m_Camera = new Camera;
	if(!m_Camera)
		return false;

	result = m_Camera->Initialise();
	if(!result)
		return false;

	float camX, camY, camZ;
	camX = 0;
	camY = 55;
	camZ = -5;

	//set a view matrix for the UI elements
	m_Camera->SetPosition(camX, 0, camZ);
	m_Camera->Render();

	m_uiViewMatrix = m_Camera->GetViewMatrix();
	
	m_Camera->SetComputerPosition(D3DXVECTOR3(0, 0, 0));


	m_Camera->SetPosition(camX, camY, camZ);
	m_Camera->SetRotation(85,0,0);

	#if _DEBUG
	std::cout << "  Success!\n";
	#endif

	#pragma endregion

	#pragma region UI objects

#if _DEBUG
	std::cout << "Creating UI...";
#endif

#if _DEBUG
	WCHAR* filepath = L"../ColourSpace/data/GFX/menuBar.png";
#else
	WCHAR* filepath = L"data/GFX/menuBar.png";
#endif

	m_TopBar = new UIObject;
	result = m_TopBar->Initialise(m_Device->GetDevice(), m_Device->GetDeviceContext(), m_screenWidth, m_screenHeight,
		0.0f, 0.0f,
		(float)m_screenWidth, 50,
		filepath);
	if (!result)
		return false;

#if _DEBUG
	filepath = L"../ColourSpace/data/GFX/background.png";
#else
	filepath = L"data/GFX/background.png";
#endif

	m_Background = new UIObject;
	result = m_Background->Initialise(m_Device->GetDevice(), m_Device->GetDeviceContext(), m_screenWidth, m_screenHeight,
		0.0f, 0.0f,
		1600,
		900,
		filepath);
	if (!result)
		return false;

#if _DEBUG
	filepath = L"../ColourSpace/data/GFX/splashscreen.jpg";
#else
	filepath = L"data/GFX/splashscreen.jpg";
#endif

	m_SplashScreen = new UIObject;
	result = m_SplashScreen->Initialise(m_Device->GetDevice(), m_Device->GetDeviceContext(), m_screenWidth, m_screenHeight,
		0.0f, 0.0f,
		1600,
		900,
		filepath);
	if (!result)
		return false;

	
	m_SplashScreen->SetActive(!INTRO_SKIP);

#if _DEBUG
	filepath = L"../ColourSpace/data/GFX/dropdownMenu.png";
#else
	filepath = L"data/GFX/dropdownMenu.png";
#endif

	m_DropdownFile = new UIObject;
	result = m_DropdownFile->Initialise(m_Device->GetDevice(), m_Device->GetDeviceContext(), m_screenWidth, m_screenHeight,
		76, 0,
		303, 374,
		filepath);
	if (!result)
		return false;

	m_DropdownFile->SetActive(false);

#if _DEBUG
	filepath = L"../ColourSpace/data/GFX/infoScreen.png";
#else
	filepath = L"data/GFX/infoScreen.png";
#endif

	m_InfoScreen = new UIObject;
	result = m_InfoScreen->Initialise(m_Device->GetDevice(), m_Device->GetDeviceContext(), m_screenWidth, m_screenHeight,
		800, 60,
		1574, 534,
		filepath);
	if (!result)
		return false;

	m_InfoScreen->SetActive(false);

#if _DEBUG
	filepath = L"../ColourSpace/data/GFX/pickImage.png";
#else
	filepath = L"data/GFX/pickImage.png";
#endif

	m_ImageSelect = new UIObject;
	result = m_ImageSelect->Initialise(m_Device->GetDevice(), m_Device->GetDeviceContext(), m_screenWidth, m_screenHeight,
		(m_screenWidth / 2) - 364, (m_screenHeight / 2) - 152,
		(m_screenWidth / 2) + 364, (m_screenHeight / 2) + 152,
		filepath);
	if (!result)
		return false;

	m_ImageSelect->SetActive(false);

#if _DEBUG
	filepath = L"../ColourSpace/data/GFX/tutorial.png";
#else
	filepath = L"data/GFX/tutorial.png";
#endif

	m_Tutorial = new UIObject;
	result = m_Tutorial->Initialise(m_Device->GetDevice(), m_Device->GetDeviceContext(), m_screenWidth, m_screenHeight,
		440, 490,
		886, 984,
		filepath);
	if (!result)
		return false;

	m_Tutorial->SetActive(false);

#if _DEBUG
	filepath = L"../ColourSpace/data/GFX/controls.png";
#else
	filepath = L"data/GFX/controls.png";
#endif

	m_Controls = new UIObject;
	result = m_Controls->Initialise(m_Device->GetDevice(), m_Device->GetDeviceContext(), m_screenWidth, m_screenHeight,
		20, 460,
		466, 954,
		filepath);
	if (!result)
		return false;

	m_Controls->SetActive(false);

#if _DEBUG
		filepath = L"../ColourSpace/data/GFX/explorer.png";
#else
	filepath = L"data/GFX/explorer.png";
#endif

	m_ExplorerWindow = new UIObject;
	result = m_ExplorerWindow->Initialise(m_Device->GetDevice(), m_Device->GetDeviceContext(), m_screenWidth, m_screenHeight,
		30, 70,
		756, 456,
		filepath);
	if (!result)
		return false;

	m_ExplorerWindow->SetActive(false);

#if _DEBUG
	filepath = L"../ColourSpace/data/GFX/loadingImages.png";
#else
	filepath = L"data/GFX/loadingImages.png";
#endif

	m_LoadingImages = new UIObject;
	result = m_LoadingImages->Initialise(m_Device->GetDevice(), m_Device->GetDeviceContext(), m_screenWidth, m_screenHeight,
		650, 500,
		650 + 318, 500 + 42,
		filepath);
	if (!result)
		return false;

	m_LoadingImages->SetActive(false);

#if _DEBUG
	filepath = L"../ColourSpace/data/GFX/loadedImages.png";
#else
	filepath = L"data/GFX/loadedImages.png";
#endif

	m_LoadedImages = new UIObject;
	result = m_LoadedImages->Initialise(m_Device->GetDevice(), m_Device->GetDeviceContext(), m_screenWidth, m_screenHeight,
		700, 500,
		700 + 196, 500 + 38,
		filepath);
	if (!result)
		return false;

	m_LoadedImages->SetActive(false);

#if _DEBUG
	filepath = L"../ColourSpace/data/GFX/dropdownMenu.png";
#else
	filepath = L"data/GFX/dropdownMenu.png";
#endif

	result = D3DX11CreateShaderResourceViewFromFile(m_Device->GetDevice(), filepath, NULL, NULL, &m_menuOriginal, NULL);
	if (FAILED(result))
		return false;

#if _DEBUG
	filepath = L"../ColourSpace/data/GFX/dropdownMenuOpen.png";
#else
	filepath = L"data/GFX/dropdownMenuOpen.png";
#endif

	result = D3DX11CreateShaderResourceViewFromFile(m_Device->GetDevice(), filepath, NULL, NULL, &m_selectOpen, NULL);
	if (FAILED(result))
		return false;

#if _DEBUG
	filepath = L"../ColourSpace/data/GFX/dropdownMenuGetInfo.png";
#else
	filepath = L"data/GFX/dropdownMenuGetInfo.png";
#endif

	result = D3DX11CreateShaderResourceViewFromFile(m_Device->GetDevice(), filepath, NULL, NULL, &m_selectInfo, NULL);
	if (FAILED(result))
		return false;

#if _DEBUG
	filepath = L"../ColourSpace/data/GFX/dropdownMenuEject.png";
#else
	filepath = L"data/GFX/dropdownMenuEject.png";
#endif

	result = D3DX11CreateShaderResourceViewFromFile(m_Device->GetDevice(), filepath, NULL, NULL, &m_selectEject, NULL);
	if (FAILED(result))
		return false;


#if _DEBUG
	filepath = L"../ColourSpace/data/sidebar.png";
#else
	filepath = L"data/sidebar.png";
#endif


	m_uiList.push_back(m_Background);
	m_uiList.push_back(m_TopBar);
	
	m_uiList.push_back(m_InfoScreen);
	m_uiList.push_back(m_ExplorerWindow);
	m_uiList.push_back(m_Controls);
	m_uiList.push_back(m_Tutorial);
	m_uiList.push_back(m_ImageSelect);
	m_uiList.push_back(m_DropdownFile);
	m_uiList.push_back(m_SplashScreen);

	m_uiList.push_back(m_LoadingImages);
	m_uiList.push_back(m_LoadedImages);

#if _DEBUG
	std::cout << "      Success!\n";
#endif

#pragma endregion 

	#pragma region Create scene

	#if _DEBUG
	std::cout << "Creating Scene...";
	#endif

	m_GameScene = new GameScene;
	if(!m_GameScene)
		return false;

	result = m_GameScene->Initialise(&m_Renderer, m_Device->GetDeviceContext(), m_Device->GetDevice(), m_hwnd, &m_worldMatrix, &m_viewMatrix, &m_projectionMatrix, screenWidth, screenHeight);
	if(!result)
		return false;

	#if _DEBUG
	std::cout << "   Success!\n";
	#endif

	#pragma endregion


	#if _DEBUG
	std::cout << "\nDone!\n\n";
	#endif

	return true;
}

bool MenuScene::Frame()
{
	m_frameRate.Frame();
	Try(UpdateState());

	if (m_exit)
		Shutdown();

	return !m_exit;
}

bool MenuScene::Render()
{
	// Clear the buffers to begin the scene.
	m_Device->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Frame(m_frameRate.GetDeltaTime());
	m_Camera->Render();

	m_viewMatrix = m_Camera->GetViewMatrix();
	m_Device->GetWorldMatrix(m_worldMatrix);
	m_Device->GetProjectionMatrix(m_projectionMatrix);
	m_Device->GetOrthoMatrix(m_orthoMatrix);

	switch (*m_state)
	{
	case SPLASH:
		//Try(RenderSplash());
		Try(RenderMenu());
		break;
	case MENU:
		Try(RenderMenu());
		break;
	case MENU_IMAGE_SELECTION:
		Try(RenderMenu());
		break;
	case GAME:
		Try(RenderGame());
		break;
	case SHUTDOWN:

		break;
	}

	m_Device->EndScene();

	return true;
}

bool MenuScene::RenderSplash()
{

	return true;
}

bool MenuScene::RenderMenu()
{
	//begin UI rendering
	m_Device->TurnZBufferOff();

	m_Device->TurnOnAlphaBlending();


	//render the UI
	for (unsigned int i = 0; i < m_uiList.size(); i++)
	{
		if (m_uiList[i]->IsActive())
		{
			//update the ui in case of movement
			m_uiList[i]->Update(m_Device->GetDeviceContext());

			m_Renderer.Render(m_Device->GetDeviceContext(), m_uiList[i]->GetIndexCount(), m_uiList[i]->GetVertexBuffer(), m_uiList[i]->GetIndexBuffer(), m_uiList[i]->GetSize());
			m_Renderer.RenderTexture(m_Device->GetDeviceContext(), m_uiList[i]->GetIndexCount(), m_uiList[i]->GetUITexture(), &m_uiViewMatrix, &m_orthoMatrix, 0);
		}
	}

	m_Device->TurnOffAlphaBlending();

	m_Device->TurnZBufferOn();

	m_Device->SetBackBufferRenderTarget();
	m_Device->ResetViewport();

	return true;
}

bool MenuScene::RenderGame()
{
	m_Device->TurnZBufferOff();
	m_Device->TurnOnAlphaBlending();
	
	//render the UI
	m_GameScene->RenderScene2D(m_Device->GetDeviceContext(), &m_uiViewMatrix, &m_orthoMatrix);

	m_Device->TurnOffAlphaBlending();
	m_Device->TurnZBufferOn();

	//m_Device->SetBackBufferRenderTarget();
	m_Device->ResetViewport();

	return true;
}


bool MenuScene::UpdateState()
{
	//read user input
	Try(m_Input->Frame());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F4))
		{
			return false;
		}
	}

	//Escape key goes back to the menu
	if (m_Input->IsEscapePressed())
	{
		if (*m_state == GAME)
		{
			SetState(MENU);
			m_GameScene->StopMusic();
		}
	}

	#pragma region DEBUG SKIPPING

	if (INTRO_SKIP)
	{
		if(*m_state == SPLASH)
			SetState(MENU);
	}

	if (MENU_SKIP)
	{
		if (*m_state == MENU)
		{
			m_GameScene->LoadArena(m_Device->GetDeviceContext(), m_Device->GetDevice(), m_screenWidth, m_screenHeight, 0);
			m_ImageSelect->SetActive(false);
			m_LoadingImages->SetActive(false);
			m_LoadedImages->SetActive(false);

			SetState(GAME);
		}
	}

	#pragma endregion 

	switch(*m_state)
	{
	case SPLASH:

		if (m_introTimer < INTRO_LENGTH && m_frameRate.GetDeltaTime() < 0.1f)
		{
			m_introTimer += m_frameRate.GetDeltaTime();
		}
		else if (m_introTimer >= INTRO_LENGTH)
		{
			SetState(MENU);
			m_SplashScreen->SetActive(false);
		}

		break;
	case MENU:
		Try(UpdateUI());

		break;
	case GAME:
		m_gameReturn = m_GameScene->Frame(m_frameRate.GetDeltaTime(), m_Device->GetDeviceContext());

		if(!m_gameReturn)
			SetState(MENU);

		break;
	case SHUTDOWN:
		return false;

		break;
	default:
		return false;
	}

	Try(Render());

	return true;
}

bool MenuScene::UpdateUI()
{
	if (m_ImageSelect->IsActive())
	{
		if (IMAGES_DOWNLOADED)
		{
			m_LoadingImages->SetActive(false);
			m_LoadedImages->SetActive(true);
		}
		else
		{
			m_LoadingImages->SetActive(true);
			m_LoadedImages->SetActive(false);
		}
	}

	//bring down menu
	if (m_Input->GetMousePressed())
	{
		//Get mouse location, map it to mouseX and mouseY
		m_Input->GetMouseLocation(m_mouseX, m_mouseY);

		//std::cout << "X: " << m_mouseX << "| Y: " << m_mouseY << std::endl;
		
		#pragma region DROP DOWN MENU
		if (m_mouseX > 90 && m_mouseX < 144 &&
			m_mouseY > 0 && m_mouseY < 43)
		{
			m_DropdownFile->SetActive(true);
			m_ImageSelect->SetActive(false);
			m_LoadingImages->SetActive(false);
			m_LoadedImages->SetActive(false);

			m_mouseSelect = true;
			m_menuStarted = true;
		}
		else
		{
			//keeps the drop down after moving the mouse below the menu button
			if (m_menuStarted)
			{
				if (m_DropdownFile->IsActive())
				{
					if (m_mouseX > 73 && m_mouseX < 304 &&
						m_mouseY > 15 && m_mouseY < 370)
					{
						m_DropdownFile->SetActive(true);
						m_mouseSelect = true;
					}
					else
					{
						m_DropdownFile->SetActive(false);
						m_mouseSelect = false;
					}
				}
			}
		}
		#pragma endregion

		#pragma region HOVER TEXTURE UPDATE
		if (m_DropdownFile->IsActive() || m_menuStarted)
		{
			if (m_mouseX > 78 && m_mouseX < 298 &&
				m_mouseY > 49 && m_mouseY < 81)
			{
				m_DropdownFile->SetUITexture(m_selectOpen);
			}
			else if (m_mouseX > 78 && m_mouseX < 298 &&
				m_mouseY > 109 && m_mouseY < 142)
			{
				m_DropdownFile->SetUITexture(m_selectInfo);
			}
			else if (m_mouseX > 78 && m_mouseX < 298 &&
				m_mouseY > 332 && m_mouseY < 360)
			{
				m_DropdownFile->SetUITexture(m_selectEject);
			}
			else m_DropdownFile->SetUITexture(m_menuOriginal);
		}
		#pragma endregion 

		if (!m_DropdownFile->IsActive())
		{
			#pragma region EXPLORER WINDOW LOGIC
			if (m_ExplorerWindow->IsActive())
			{
				//Close the window
				if (m_mouseX > 49 && m_mouseX < 67 &&
					m_mouseY > 79 && m_mouseY < 96)
				{
					m_ExplorerWindow->SetActive(false);
				}

				//Select tutorial
				if (m_mouseX > 74 && m_mouseX < 140 &&
					m_mouseY > 281 && m_mouseY < 369)
				{
					m_Tutorial->SetActive(true);
				}

				//Select controls
				if (m_mouseX > 189 && m_mouseX < 253 &&
					m_mouseY > 290 && m_mouseY < 364)
				{
					m_Controls->SetActive(true);
				}

				//Select to launch the game
				if (m_mouseX > 63 && m_mouseX < 156 &&
					m_mouseY > 168 && m_mouseY < 248)
				{
					m_ExplorerWindow->SetActive(false);
					m_Tutorial->SetActive(false);
					m_Controls->SetActive(false);
					m_InfoScreen->SetActive(false);
					m_ImageSelect->SetActive(true);
				}
			}
			#pragma endregion 

			#pragma region CHOOSE IMAGE TO LOAD
			if (m_ImageSelect->IsActive())
			{
				m_Tutorial->SetActive(true);

				if (IMAGES_DOWNLOADED)
				{
					m_LoadingImages->SetActive(false);
					m_LoadedImages->SetActive(true);
				}
				else
				{
					m_LoadingImages->SetActive(true);
					m_LoadedImages->SetActive(false);
				}

				//Load image 1
				if (m_mouseX > 535 && m_mouseX < 566 &&
					m_mouseY > 420 && m_mouseY < 472)
				{
					if (MenuScene::IMAGES_DOWNLOADED)
					{
						m_GameScene->LoadArena(m_Device->GetDeviceContext(), m_Device->GetDevice(), m_screenWidth, m_screenHeight, 0);
						m_ImageSelect->SetActive(false);
						SetState(GAME);
					}
				}

				//Load image 2
				if (m_mouseX > 625 && m_mouseX < 669 &&
					m_mouseY > 417 && m_mouseY < 472)
				{
					if (MenuScene::IMAGES_DOWNLOADED)
					{
						m_GameScene->LoadArena(m_Device->GetDeviceContext(), m_Device->GetDevice(), m_screenWidth, m_screenHeight, 1);
						m_ImageSelect->SetActive(false);
						m_LoadingImages->SetActive(false);
						m_LoadedImages->SetActive(false);
						m_Tutorial->SetActive(false);

						SetState(GAME);
					}
				}

				//Load image 3
				if (m_mouseX > 721 && m_mouseX < 762 &&
					m_mouseY > 417 && m_mouseY < 472)
				{
					if (MenuScene::IMAGES_DOWNLOADED)
					{
						m_GameScene->LoadArena(m_Device->GetDeviceContext(), m_Device->GetDevice(), m_screenWidth, m_screenHeight, 2);
						m_ImageSelect->SetActive(false);
						m_LoadingImages->SetActive(false);
						m_LoadedImages->SetActive(false);
						m_Tutorial->SetActive(false);

						SetState(GAME);
					}
				}

				//Load image 4
				if (m_mouseX > 818 && m_mouseX < 863 &&
					m_mouseY > 416 && m_mouseY < 470)
				{
					if (MenuScene::IMAGES_DOWNLOADED)
					{
						m_GameScene->LoadArena(m_Device->GetDeviceContext(), m_Device->GetDevice(), m_screenWidth, m_screenHeight, 3);
						m_ImageSelect->SetActive(false);
						m_LoadingImages->SetActive(false);
						m_LoadedImages->SetActive(false);
						m_Tutorial->SetActive(false);

						SetState(GAME);
					}
				}

				//Load image 5
				if (m_mouseX > 911 && m_mouseX < 953 &&
					m_mouseY > 416 && m_mouseY < 472)
				{
					if (MenuScene::IMAGES_DOWNLOADED)
					{
						m_GameScene->LoadArena(m_Device->GetDeviceContext(), m_Device->GetDevice(), m_screenWidth, m_screenHeight, 4);
						m_ImageSelect->SetActive(false);
						m_LoadingImages->SetActive(false);
						m_LoadedImages->SetActive(false);
						m_Tutorial->SetActive(false);

						SetState(GAME);
					}
				}

				//Load image 6
				if (m_mouseX > 1007 && m_mouseX < 1049 &&
					m_mouseY > 416 && m_mouseY < 472)
				{
					if (MenuScene::IMAGES_DOWNLOADED)
					{
						m_GameScene->LoadArena(m_Device->GetDeviceContext(), m_Device->GetDevice(), m_screenWidth, m_screenHeight, 5);
						m_ImageSelect->SetActive(false);
						m_LoadingImages->SetActive(false);
						m_LoadedImages->SetActive(false);
						m_Tutorial->SetActive(false);

						SetState(GAME);
					}
				}
			}
			#pragma endregion 	

			#pragma region CLOSE TUTORIAL
			if (m_Tutorial->IsActive())
			{
				if (m_mouseX > 456 && m_mouseX < 475 &&
					m_mouseY > 492 && m_mouseY < 509)
				{
					m_Tutorial->SetActive(false);
				}
			}
			#pragma endregion 

			#pragma region CLOSE CONTROLS
			if (m_Controls->IsActive())
			{
				if (m_mouseX > 39 && m_mouseX < 57 &&
					m_mouseY > 461 && m_mouseY < 479)
				{
					m_Controls->SetActive(false);
				}
			}
			#pragma endregion 

			#pragma region CLOSE INFO SCREEN
			if (m_InfoScreen->IsActive())
			{
				if (m_mouseX > 812 && m_mouseX < 832 &&
					m_mouseY > 67 && m_mouseY < 87)
				{
					m_InfoScreen->SetActive(false);
				}
			}
			#pragma endregion 
		}
	}
	else m_DropdownFile->SetActive(false);

	#pragma region DROP DOWN MENU SELECTION
	if (m_mouseSelect && !m_Input->GetMousePressed())
	{
		//load image screen
		if (m_mouseX > 78 && m_mouseX < 298 &&
			m_mouseY > 49 && m_mouseY < 81)
		{
			m_ExplorerWindow->SetActive(true);
		}

		//load info screen
		if (m_mouseX > 78 && m_mouseX < 298 &&
			m_mouseY > 109 && m_mouseY < 142)
		{
			m_InfoScreen->SetActive(true);
		}

		//exit the program
		if (m_mouseX > 78 && m_mouseX < 298 &&
			m_mouseY > 332 && m_mouseY < 360)
		{
			SetState(SHUTDOWN);
		}

		m_mouseSelect = false;
		m_menuStarted = false;
	}
	#pragma endregion 

	return true;
}

void MenuScene::SetState(State state)
{
	*m_state = state;
}

void MenuScene::Try(bool toTest)
{
	if (toTest)
	{
		return;
	}
	else if (!toTest)
	{
		m_exit = true;
	}
}

void MenuScene::Shutdown()
{
	if (m_Camera)
	{
		m_Camera->Shutdown();
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_state)
	{
		delete m_state;
		m_state = 0;
	}

	if (m_GameScene)
	{
		m_GameScene->Shutdown();
		delete m_GameScene;
		m_GameScene = 0;
	}

	for (int i = 0; i < m_uiList.size(); i++)
	{
		m_uiList[i]->Shutdown();
		delete m_uiList[i];
		m_uiList[i] = 0;
	}

	m_uiList.clear();

	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	if (m_menuOriginal)
	{
		m_menuOriginal = 0;
	}

	if (m_selectOpen)
	{
		m_selectOpen = 0;
	}

	if (m_selectInfo)
	{
		m_selectInfo = 0;
	}

	if (m_selectEject)
	{
		m_selectEject = 0;
	}

	return;
}
