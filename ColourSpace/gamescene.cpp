#include "gamescene.h"

#include <cstring>


GameScene::GameScene()
{
	srand(time(NULL));

	m_Arena = 0;
	m_CalculatingScore = 0;
	m_calculateScore = false;
	m_setScore = false;
	m_calculateTimer = 0;
	m_Scoreboard = 0;
	m_Checkers = 0;
	//m_FluidGrid = 0;

	m_Continue = 0;
	m_YesNo = 0;

	m_CupOne = 0;
	m_CupTwo = 0;
	m_CupThree = 0;
	m_CupFour = 0;
	m_showScore = false;
	m_arenaPushedBack = false;
	arenaPushBack = 0;
	m_startPathTimer = 0;

	m_flip = false;
	flipTimer = 0;

	m_firstAISpawned = false;
	m_aiSpawnTimer = 0;
	m_aiActiveSize = 0;
	m_playingChaseSound = false;
	m_aiPlayingSound = 0;
}

GameScene::~GameScene()
{
}


bool GameScene::Initialise(Renderer* renderer,ID3D11DeviceContext* deviceContext, ID3D11Device* device, HWND* hwnd,
	D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj, int screenWidth, int screenHeight)
{
	bool result;

	m_Renderer = renderer;

	result = Score::LoadNumberTextures(device);
	if (!result)
	{
		MessageBox(*hwnd, L"Failed to load a score texture.", L"Error", MB_OK);
		return false;
	}
	

	

	m_DecimalPoints = new Object2D[4];
	m_PercentageSigns = new Object2D[4];
	m_Canvas = new Object2D;

	m_Player1 = new Player;
	m_Player2 = new Player;
	m_Player3 = new Player;
	m_Player4 = new Player;

	m_Continue = new Object2D;
	m_YesNo = new Object2D;

	m_CupOne = new Object2D;
	m_CupTwo = new Object2D;
	m_CupThree = new Object2D;
	m_CupFour = new Object2D;

	for (int i = 0; i < 4; i++)
	{
		m_isPlaying[i] = false;
	}

	result = LoadSoundData();
	if (!result)
	{
		MessageBox(*hwnd, L"Failed to load sound.", L"Error", MB_OK);
		return false;
	}

	LoadInitialiseData();
	int playerIndex = 0;

	for (unsigned int i = 0; i < m_InitialiseData.size(); i++)
	{
		switch (m_InitialiseData[i].dataType)
		{
		case DataType::SCORE:
		{
			Score* scorePtr = static_cast<Score*>(m_InitialiseData[i].dataPtr);


			bool result = scorePtr->Initialise(device, deviceContext, screenWidth, screenHeight,
			
											   m_InitialiseData[i].pos.x, m_InitialiseData[i].pos.y,
			
											   m_InitialiseData[i].pos.x + m_InitialiseData[i].size.x, //end x
											   m_InitialiseData[i].pos.y + m_InitialiseData[i].size.y,  //end y
#if _DEBUG
											   m_InitialiseData[i].dataFilepath_DEBUG);
#else
											   m_InitialiseData[i].dataFilepath);
#endif
			if (!result)
			{
				MessageBox(*hwnd, L"Failed to initialise a score object.", L"Error", MB_OK);
				return false;
			}

			scorePtr = 0;
		}
		break;
		case DataType::OBJECT2D:
		{
			Object2D* objectPtr = static_cast<Object2D*>(m_InitialiseData[i].dataPtr);


			bool result = objectPtr->Initialise(0, 0, //Used for arena to specify drawable space

											    device, deviceContext, screenWidth, screenHeight,
			
											    m_InitialiseData[i].pos.x, m_InitialiseData[i].pos.y,
			
											    m_InitialiseData[i].pos.x + m_InitialiseData[i].size.x, //end x
											    m_InitialiseData[i].pos.y + m_InitialiseData[i].size.y,  //end y
#if _DEBUG
											    m_InitialiseData[i].dataFilepath_DEBUG);
#else
											    m_InitialiseData[i].dataFilepath);
#endif
			if (!result)
			{
				MessageBox(*hwnd, L"Failed to initialise a 2d object.", L"Error", MB_OK);
				return false;
			}

			objectPtr = 0;
		}
		break;
		case DataType::PLAYER:
		{
			Player* playerPtr = static_cast<Player*>(m_InitialiseData[i].dataPtr);


			bool result = playerPtr->Initialise(device, deviceContext, *hwnd, 
#if _DEBUG
											    m_InitialiseData[i].dataFilepath_DEBUG,
#else
											    m_InitialiseData[i].dataFilepath,
#endif			
											    playerIndex,
											    screenWidth, screenHeight,
			
											    m_InitialiseData[i].pos,
											    m_InitialiseData[i].size);
			if (!result)
			{
				MessageBox(*hwnd, L"Failed to initialise a player object.", L"Error", MB_OK);
				return false;
			}

			D3DXVECTOR4 color = D3DXVECTOR4(1, 0, 0, 1);

			if (playerIndex == 0)
				color = D3DXVECTOR4(1, 0, 0, 1);
			else if (playerIndex == 1)
				color = D3DXVECTOR4(1, 0.4f, 0.7f, 1);
			else if (playerIndex == 2)
				color = D3DXVECTOR4(0, 1, 1, 1);
			else if (playerIndex == 3)
				color = D3DXVECTOR4(0, 0.7f, 0, 1);

			playerPtr->SetPlayerColour(color);

			playerPtr = 0;
			playerIndex++;
		}
		break;

		default:
		{
			MessageBox(*hwnd, L"An object didn't have a dataType specified.", L"Error", MB_OK);
			return false;
		}
		break;
		}
		
	}

	//clear the initialisation data
	for (unsigned int i = 0; i < m_InitialiseData.size(); i++)
	{
		m_InitialiseData[i].Clear();
	}

	m_InitialiseData.clear();

	for (int i = 0; i < MAX_AI; i++)
	{
		m_aiBucketList.push_back(BucketAI());
		m_aiBucketList[i].Initialise(device, deviceContext, hwnd, screenWidth, screenHeight);
	}

	//push em back to the update lists
	/*m_objectList.push_back(m_Canvas);*/
	//m_objectList.push_back(m_aiBucket.bucket);

	m_playerList.push_back(m_Player1);
	m_playerList.push_back(m_Player2);
	m_playerList.push_back(m_Player3);
	m_playerList.push_back(m_Player4);

	for (int i = 0; i < 4; i++)
	{
		BucketAI::LinkPlayerPosition(i, m_playerList[i]->GetQuad()->GetPosition());
	}

	return true;
}

bool GameScene::LoadArena(ID3D11DeviceContext* dc, ID3D11Device* device, int width, int height, int imageToLoad = 0)
{
	bool result;
	WCHAR* filename = 0;

#if _DEBUG
	switch(imageToLoad)
	{
	case 0:
		filename = L"../ColourSpace/data/Images/blank.jpg";
		break;
	case 1:
		filename = L"../ColourSpace/data/Images/2.jpg";
		break;
	case 2:
		filename = L"../ColourSpace/data/Images/3.jpg";
		break;
	case 3:
		filename = L"../ColourSpace/data/Images/4.jpg";
		break;
	case 4:
		filename = L"../ColourSpace/data/Images/5.jpg";
		break;
	case 5:
		filename = L"../ColourSpace/data/Images/6.jpg";
		break;
	} 
#else
	switch (imageToLoad)
	{
	case 0:
		filename = L"data/Images/0.jpg";
		break;
	case 1:
		filename = L"data/Images/1.jpg";
		break;
	case 2:
		filename = L"data/Images/2.jpg";
		break;
	case 3:
		filename = L"data/Images/3.jpg";
		break;
	case 4:
		filename = L"data/Images/4.jpg";
		break;
	case 5:
		filename = L"data/Images/5.jpg";
		break;
	}
#endif

	

	if (!m_Checkers)
	{
		m_Checkers = new Object2D;

		result = m_Checkers->Initialise(1600, //size x
										900, //size y

										device, dc, width, height,

										0, 0, //start pos

										1600, //end x
										900,  //end y
#if _DEBUG
										L"../ColourSpace/data/GFX/theVoid.jpg");
#else
										L"data/GFX/theVoid.jpg");
#endif

		

		if (!result)
			return false;
	}

	if (!m_Arena)
	{
		m_Arena = new Object2D;
		*m_Arena->GetSize() = D3DXVECTOR2(1600, 855);
		*m_Arena->GetPosition() = D3DXVECTOR2(0, 45);

	

		result = m_Arena->Initialise(m_Arena->GetSize()->x, //size x
										m_Arena->GetSize()->y, //size y

										device, dc, width, height,

										m_Arena->GetPosition()->x, m_Arena->GetPosition()->y, //start pos

										m_Arena->GetPosition()->x + m_Arena->GetSize()->x, //end x
										m_Arena->GetPosition()->y + m_Arena->GetSize()->y,  //end y
										filename);

		if (!result)
			return false;
	}
	else
	{
		result = m_Arena->LoadTexture(device, filename);
		if (!result)
			return false;
	}

	m_FluidGrid.Initialise(m_Arena->GetSize()->x, m_Arena->GetSize()->y, device, dc, filename);
	m_FluidGrid.Apply(dc);

	if (filename)
	{
		filename = 0;
	}

	//only want to push back the arena once and then just change the 
	//pointer upon resetting the level
	if (arenaPushBack == 0)
	{
		m_objectList.push_back(m_Checkers);
		m_objectList.push_back(m_Canvas);
		m_objectList.push_back(m_Arena);
		
		arenaPushBack = m_objectList.size() - 1;
	}
	else
	{
		m_objectList[arenaPushBack] = m_Arena;
	}

	//put the players back to their original position
	*m_playerList[0]->GetQuad()->GetPosition() = P1_POS;
	*m_playerList[1]->GetQuad()->GetPosition() = P2_POS;
	*m_playerList[2]->GetQuad()->GetPosition() = P3_POS;
	*m_playerList[3]->GetQuad()->GetPosition() = P4_POS;

	//make them all alive again
	for (int i = 0; i < 4; i++)
	{
		m_playerList[i]->alive = true;
		m_playerList[i]->velocity = 0;
	}

	//push the scores back once
	if (!m_arenaPushedBack)
	{
		for (int i = 0; i < 4; i++)
		{
			m_scoreList.push_back(&m_PlayerColors[i]);
		}

		m_scoreList.push_back(m_CupOne);
		m_scoreList.push_back(m_CupTwo);
		m_scoreList.push_back(m_CupThree);
		m_scoreList.push_back(m_CupFour);


		m_arenaPushedBack = true;
	}

	m_Arena->AccessTextureClass()->SetTexture(m_FluidGrid.GetTexture());

	m_showScoreTimer = 0;
	m_showScore = false;

	deadCount = 0;
	m_levelTimer = 0;

	m_flip = false;
	flipTimer = 0;

	//reset the AI
	BucketAI::ResetBaseSpeed();

	m_aiSpawnTimer = 0;
	m_startPathTimer = 0;
	m_aiActiveSize = 0;
	m_firstAISpawned = false;

	for (int i = 0; i < MAX_AI; i++)
	{
		m_aiBucketList[i].SetInactive();
	}
	m_Music.stop();
	m_Music.play();

	return true;
}


void GameScene::Shutdown()
{
	for (unsigned int i = 0; i < m_objectList.size(); i++)
	{
		if (m_objectList[i])
		{
			m_objectList[i]->Shutdown();
			m_objectList[i] = 0;
		}
	}
	m_objectList.clear();
	
	for (unsigned int i = 0; i < m_playerList.size(); i++)
	{
		m_playerList[i]->Shutdown();
	}
	m_playerList.clear();

	if (m_Arena)
	{
		m_Arena->Shutdown();
		delete m_Arena;
		m_Arena = 0;
	}

	if (m_Canvas)
	{
		m_Canvas->Shutdown();
		delete m_Canvas;
		m_Canvas = 0;
	}

	if (m_CalculatingScore)
	{
		m_CalculatingScore->Shutdown();
		delete m_CalculatingScore;
		m_CalculatingScore = 0;
	}

	if (m_Scoreboard)
	{
		m_Scoreboard->Shutdown();
		delete m_Scoreboard;
		m_Scoreboard = 0;
	}

	if (m_DecimalPoints)
	{
		for (int i = 0; i < 4; i++)
		{
			m_DecimalPoints[i].Shutdown();
		}

		delete[] m_DecimalPoints;
		m_DecimalPoints = 0;
	}

	if (m_PercentageSigns)
	{
		for (int i = 0; i < 4; i++)
		{
			m_PercentageSigns[i].Shutdown();
		}

		delete[] m_PercentageSigns;
		m_PercentageSigns = 0;
	}

	if (m_Continue)
	{
		m_Continue->Shutdown();
		delete m_Continue;
		m_Continue = 0;
	}

	if (m_YesNo)
	{
		m_YesNo->Shutdown();
		delete m_YesNo;
		m_YesNo = 0;
	}

	if (m_CupOne)
	{
		m_CupOne->Shutdown();
		delete m_CupOne;
		m_CupOne = 0;
	}

	if (m_CupTwo)
	{
		m_CupTwo->Shutdown();
		delete m_CupTwo;
		m_CupTwo = 0;
	}

	if (m_CupThree)
	{
		m_CupThree->Shutdown();
		delete m_CupThree;
		m_CupThree = 0;
	}

	if (m_CupFour)
	{
		m_CupFour->Shutdown();
		delete m_CupFour;
		m_CupFour = 0;
	}

	if (m_Player1)
	{
		m_Player1->Shutdown();
		delete m_Player1;
		m_Player1 = 0;
	}

	if (m_Player2)
	{
		m_Player2->Shutdown();
		delete m_Player2;
		m_Player2 = 0;
	}

	if (m_Player3)
	{
		m_Player3->Shutdown();
		delete m_Player3;
		m_Player3 = 0;
	}

	if (m_Player4)
	{
		m_Player4->Shutdown();
		delete m_Player4;
		m_Player4 = 0;
	}


	return;
}

bool GameScene::Frame(float frameTime, ID3D11DeviceContext* dc)
{
	//don't count the low FPS at the beginning of loading into the level timer
	if (frameTime < 0.1f)
	{
		m_levelTimer += frameTime;
		m_startPathTimer += frameTime;

		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		//	m_aiSpawnTimer += 1;
		
		m_aiSpawnTimer += frameTime;

		flipTimer += frameTime;

		if (flipTimer > 3)
		{
			m_flip = !m_flip;
			flipTimer = 0;
		}

		if (!m_flip)
		{
			m_Checkers->GetQuad()->GetPosition()->y += frameTime * 5;
		}
		else
		{
			m_Checkers->GetQuad()->GetPosition()->y -= frameTime * 5;
		}

	}

	if (m_showScore)
		m_showScoreTimer += frameTime;

	if (m_showScoreTimer > SHOW_SCORE_TIME)
	{
		return false;
	}

#pragma region gameplay

	if (GAMEPLAY_ENABLED)
	{
		if (!m_showScore)
		{
			m_FluidGrid.Update(frameTime, dc);

			for (int i = 0; i < 4; ++i)
			{
				m_Scores[i].SetScore((int)m_FluidGrid.playerScores[i], i);
			}

			if (AI_ENABLED)
			{
				//start path for the black buckets
				if (m_startPathTimer > START_PATH_INTERVAL)
				{
					bool init = false;
					for (int i = 0; i < m_aiActiveSize; i++)
					{
						if (i < MAX_AI)
						{
							if (!m_aiBucketList[i].IsActive() && !init)
							{
								m_aiBucketList[i].SpawnBucket();

								//init = true;
								//break;
							}
						}
					}

					m_startPathTimer = 0;
				}

				//first ai spawn is faster than the other ones
				if (!m_firstAISpawned)
				{
					if (m_aiSpawnTimer > AI_CREATE_FIRST)
					{
						m_firstAISpawned = true;

						m_aiActiveSize = 1;
						m_aiSpawnTimer = 0;
					}
				}

				//add more AI to the game
				if (m_aiSpawnTimer > AI_CREATE_INTERVAL)
				{
					if (m_aiActiveSize < MAX_AI)
					{
						m_aiActiveSize++;
					}

					m_aiSpawnTimer = 0;
				}

				//update them
				if (frameTime < 0.1f)
				{
					BucketAI::IncrementBaseSpeed(frameTime);

					for (int i = 0; i < m_aiActiveSize; i++)
					{
						if (i < MAX_AI)
						{
							if (m_aiBucketList[i].IsActive())
							{
								m_aiBucketList[i].Update(frameTime, m_FluidGrid);

								if (m_aiBucketList[i].UpdateFlipout(frameTime))
								{
									if (!m_playingChaseSound)
									{
										m_chaseSound.play();
										m_aiPlayingSound = i;
										m_playingChaseSound = true;
									}
								}
								else
								{
									if (m_playingChaseSound && i == m_aiPlayingSound)
									{
										m_chaseSound.stop();
										m_playingChaseSound = false;
									}
								}
							}
						}
					}
				}
			}

			for (int i = 0; i < 4; i++)
			{
				if (m_playerList[i]->alive)
				{
					//Stops player going off the canvas or screen
					
					D3DXVECTOR4 colorOnBlack = D3DXVECTOR4(1, 1, 1, 1);

					colorOnBlack = m_FluidGrid.QueryColor(m_playerList[i]->GetQuad()->GetPosition()->x + BUCKET_TIP.x,
														  m_playerList[i]->GetQuad()->GetPosition()->y + BUCKET_TIP.y);
			

					//Stop updating player position and taking input if the player has been caught by the void
					if (colorOnBlack != D3DXVECTOR4(1, 0, 1, 1) && m_playerList[i]->alive)
					{
						m_playerList[i]->Frame(frameTime);
					}
					else
					{
						//Kills the player
						if (m_playerList[i]->alive)
						{
							m_playerList[i]->alive = false;
							m_isPlaying[i] = false;
							m_FluidGrid.playerScores[i] *= 0.75f;
							m_dieSound.play();
							deadCount++;
						}

						//Counts how many players are dead
						if (deadCount > 2)
						{
							SortScores();
							m_showScore = true;
						}

						m_isPlaying[i] = false;
					}


					PlayerPaintLogic(i, frameTime, dc);

					PlayerCollision(dc, m_playerList[i]->GetQuad()->GetPosition()->x + BUCKET_TIP.x, 
										m_playerList[i]->GetQuad()->GetPosition()->y + BUCKET_TIP.y, 
										m_playerList[i]->GetPlayerColour());

					//Set the variables that allow the player to move in the specific direction 
					m_playerList[i]->enableRight = enableRight;
					m_playerList[i]->enableLeft = enableLeft;
					m_playerList[i]->enableUp = enableUp;
					m_playerList[i]->enableDown = enableDown;
				}
				//Updates player position to give a death animation
				else
				{
					m_playerList[i]->KillPlayer();
					m_isPlaying[i] = false;
				}
			}

			m_FluidGrid.Apply(dc);		
		}
	}
#pragma endregion 

	return true;
}

void GameScene::PlayerPaintLogic(int index, float dt, ID3D11DeviceContext* dc)
{
	int i = index;
	
	//The color that the player is currently on
	D3DXVECTOR4 colorOn = D3DXVECTOR4(1, 1, 1, 1);

	//if the player is within bounds of the canvas
	if (m_playerList[i]->GetQuad()->GetPosition()->x + BUCKET_TIP.x > 0 && m_playerList[i]->GetQuad()->GetPosition()->x + BUCKET_TIP.x < 1600 &&
		m_playerList[i]->GetQuad()->GetPosition()->y + BUCKET_TIP.y > 0 && m_playerList[i]->GetQuad()->GetPosition()->y + BUCKET_TIP.y < 855)
	{
		//set colorOn to the color that the player is currently over
		colorOn = m_FluidGrid.QueryColor(m_playerList[i]->GetQuad()->GetPosition()->x + BUCKET_TIP.x,
			m_playerList[i]->GetQuad()->GetPosition()->y + BUCKET_TIP.y);
	}

	if (m_playerList[i]->playerIsPainting && 
		(colorOn != D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1)))
	{
		//and if the player is not currently dragging black or someone elses color
		if (!m_playerList[i]->IsDraggingBlack() && !m_playerList[i]->IsDraggingColor())
		{
			//if the player is stuck allow them to bypass paint restrictions so they can get themselves out
			if (!m_playerList[i]->enableRight && !m_playerList[i]->enableLeft &&
				!m_playerList[i]->enableUp    && !m_playerList[i]->enableDown)
			{
				m_FluidGrid.LayPaint(m_playerList[i]->GetQuad()->GetPosition()->x + BUCKET_TIP.x,
					m_playerList[i]->GetQuad()->GetPosition()->y + BUCKET_TIP.y,
					m_playerList[i]->GetPlayerColour(), true);
			}
			else
			{
				//The player draws their own paint
				m_FluidGrid.LayPaint(m_playerList[i]->GetQuad()->GetPosition()->x + BUCKET_TIP.x,
					m_playerList[i]->GetQuad()->GetPosition()->y + BUCKET_TIP.y,
					m_playerList[i]->GetPlayerColour());
			}

			if (!m_isPlaying[0] && !m_isPlaying[1] && !m_isPlaying[2] && !m_isPlaying[3])
			{
				//m_DrawingSounds[0].PlayInLoop();
				m_DrawingSound.play();
				m_isPlaying[i] = true;
			}

			//slow the player speed
			m_playerList[i]->PlayerPaintSpeed();
		}
	}
	else if (m_playerList[i]->playerIsPainting &&
		(colorOn == D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1)))
	{
		if (!m_playerList[i]->enableRight && !m_playerList[i]->enableLeft &&
			!m_playerList[i]->enableUp    && !m_playerList[i]->enableDown)
		{
			m_FluidGrid.LayPaint(m_playerList[i]->GetQuad()->GetPosition()->x + BUCKET_TIP.x,
					m_playerList[i]->GetQuad()->GetPosition()->y + BUCKET_TIP.y,
					m_playerList[i]->GetPlayerColour(), true);
		}
	}
	else
	{
		m_isPlaying[i] = false;

		if (!m_isPlaying[0] && !m_isPlaying[1] && !m_isPlaying[2] && !m_isPlaying[3])
		{
			//m_DrawingSounds[i].Stop();
			m_DrawingSound.stop();
			m_isPlaying[i] = false;
		}
	}

	//slow down on black
	if (colorOn == D3DXVECTOR4(0.5, 0.5, 0.5, 1))
	{
		m_playerList[i]->PlayerGreySpeed();
	}
	else if (colorOn == D3DXVECTOR4(0.0, 0.0, 0.0, 1))
	{
		m_playerList[i]->PlayerBlackSpeed();
	}

	return;
}

D3DXVECTOR3 GameScene::GetPlayerPosition(int player)
{
	return m_playerList[player-1]->GetTransform()->GetPosition();
}

void GameScene::RenderScene2D(ID3D11DeviceContext* deviceContext, D3DXMATRIX* uiView, D3DXMATRIX* ortho)
{
	for (unsigned int i = 0; i < m_objectList.size(); i++)
	{
		m_objectList[i]->Update(deviceContext);

		m_Renderer->Render(deviceContext, m_objectList[i]->GetIndexCount(), 
										  m_objectList[i]->GetVertexBuffer(), 
										  m_objectList[i]->GetIndexBuffer(), 
										  m_objectList[i]->GetSizeOf());

		m_Renderer->RenderTexture(deviceContext, m_objectList[i]->GetIndexCount(), 
												 m_objectList[i]->AccessTexture(), 
												 uiView, ortho, 1);
	}

	

	if (!m_showScore)
	{
		for (unsigned int i = 0; i < m_playerList.size(); i++)
		{
			m_playerList[i]->Update(deviceContext);

			m_Renderer->Render(deviceContext, m_playerList[i]->GetIndexCount(), 
											  m_playerList[i]->GetVertexBuffer(), 
											  m_playerList[i]->GetIndexBuffer(), 
											  m_playerList[i]->GetSizeOf());

			m_Renderer->RenderTexture(deviceContext, m_playerList[i]->GetIndexCount(), 
													 m_playerList[i]->AccessTextureClass()->GetTexture(), 
													 uiView, ortho, 0);
		}

		for (unsigned int i = 0; i < m_aiBucketList.size(); i++)
		{
			if (m_aiBucketList[i].IsActive())
			{
				m_aiBucketList[i].GetBucket()->Update(deviceContext);

				m_Renderer->Render(deviceContext, m_aiBucketList[i].GetBucket()->GetIndexCount(),
												  m_aiBucketList[i].GetBucket()->GetVertexBuffer(), 
												  m_aiBucketList[i].GetBucket()->GetIndexBuffer(), 
												  m_aiBucketList[i].GetBucket()->GetSizeOf());

				m_Renderer->RenderTexture(deviceContext, m_aiBucketList[i].GetBucket()->GetIndexCount(),
														 m_aiBucketList[i].GetBucket()->AccessTextureClass()->GetTexture(), 
														 uiView, ortho, 0);
			}
		}
	}



	if (!m_showScore)
	{
		for (int i = 0; i < 4; i++)
		{
			m_Scores[i].Render(deviceContext, uiView, ortho, m_Renderer);
		}
	}


	//Draw percentage signs and decimal points
	if (!m_showScore)
	{
		for (int i = 0; i < 4; i++)
		{
			m_DecimalPoints[i].Update(deviceContext);
			m_Renderer->Render(deviceContext, m_DecimalPoints[i].GetIndexCount(), 
											  m_DecimalPoints[i].GetVertexBuffer(), 
											  m_DecimalPoints[i].GetIndexBuffer(), 
											  m_DecimalPoints[i].GetSizeOf());

			m_Renderer->RenderTexture(deviceContext, m_DecimalPoints[i].GetIndexCount(),
													 m_DecimalPoints[i].AccessTextureClass()->GetTexture(), 
													 uiView, ortho, 0);

			m_PercentageSigns[i].Update(deviceContext);
			m_Renderer->Render(deviceContext, m_PercentageSigns[i].GetIndexCount(), 
											  m_PercentageSigns[i].GetVertexBuffer(), 
											  m_PercentageSigns[i].GetIndexBuffer(), 
											  m_PercentageSigns[i].GetSizeOf());

			m_Renderer->RenderTexture(deviceContext, m_PercentageSigns[i].GetIndexCount(),
													 m_PercentageSigns[i].AccessTextureClass()->GetTexture(), 
													 uiView, ortho, 0);
		}
	}

	if (m_showScore)
	{
		for (unsigned int i = 0; i < m_scoreList.size(); i++)
		{
			m_scoreList[i]->Update(deviceContext);

			m_Renderer->Render(deviceContext, m_scoreList[i]->GetIndexCount(), m_scoreList[i]->GetVertexBuffer(), m_scoreList[i]->GetIndexBuffer(), m_scoreList[i]->GetSizeOf());
			m_Renderer->RenderTexture(deviceContext, m_scoreList[i]->GetIndexCount(), m_scoreList[i]->AccessTexture(), uiView, ortho, 1);
		}
	}
}

void GameScene::PlayerCollision(ID3D11DeviceContext* deviceContext, int playerX, int playerY, D3DXVECTOR4 playerColour)
{
	enableRight = true;
	enableLeft = true;
	enableUp = true;
	enableDown = true;

	if (playerX <= 7) enableLeft = false;
				 else enableLeft = true;

	if (playerX >= 1588) enableRight = false;
				    else enableRight = true;
	

	if (playerY <= 7) enableUp = false;
				 else enableUp = true;
	

	if (playerY >= 844) enableDown = false;
				   else enableDown = true;
	

	for (int y = playerY - 3; y < playerY + 4; ++y)
	{
		for (int x = playerX - 4; x < playerX + 4; ++x)
		{
			if (x > 0 && x < m_Arena->GetSize()->x &&
				y > 0 && y < m_Arena->GetSize()->y)
			{
				//Makes sure not colliding with player colour
				if (m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].r != playerColour.x ||
					m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].g != playerColour.y ||
					m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].b != playerColour.z)
				{
					//TEMPORARY HARD CODED NUMBERS
					if (m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].r == m_playerList[0]->GetPlayerColour().x &&
						m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].g == m_playerList[0]->GetPlayerColour().y &&
						m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].b == m_playerList[0]->GetPlayerColour().z ||

						m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].r == m_playerList[1]->GetPlayerColour().x &&
						m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].g == m_playerList[1]->GetPlayerColour().y &&
						m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].b == m_playerList[1]->GetPlayerColour().z ||

						m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].r == m_playerList[2]->GetPlayerColour().x &&
						m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].g == m_playerList[2]->GetPlayerColour().y &&
						m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].b == m_playerList[2]->GetPlayerColour().z ||

						m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].r == m_playerList[3]->GetPlayerColour().x &&
						m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].g == m_playerList[3]->GetPlayerColour().y &&
						m_FluidGrid.GetGrid()[(int)(y*m_Arena->GetSize()->x + x)].b == m_playerList[3]->GetPlayerColour().z)
					{
						//Easy way of chaecking collision, only checks pixels at player x or y value
						if (x == playerX || y == playerY)
							CheckCollisionPosition(playerX, playerY, x, y);
					}
				}
			}
		}
	}
}

void GameScene::CheckCollisionPosition(int playerX, int playerY, int xCollided, int yCollided)
{
	//Enable each direction according to position of the player and the collided pixel
	if (xCollided > playerX)
	{
		enableRight = false;
	}
	else if (xCollided < playerX)
	{
		enableLeft = false;
	}

	if (yCollided < playerY)
	{
		enableUp = false;
	}
	else if (yCollided > playerY)
	{
		enableDown = false;
	}
}


void GameScene::SortScores()
{
	D3DXVECTOR2 temp;
	m_victorySound.play();
	m_Music.stop();
	m_chaseSound.stop();
	m_dieSound.stop();
	m_DrawingSound.stop();

	m_FluidGrid.ScoreCanvasCover();

	//Gets original data and stores it in an array that will keep track of the colour
	//m_SortedScores = new D3DXVECTOR2[4];

	for (int k = 0; k < 4; ++k)
	{
		m_SortedScores[k] = D3DXVECTOR2(m_FluidGrid.playerScores[k], k);
	}

	//Simple bubble sort
	for (int i = 0; i < 4; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			if (m_SortedScores[j].x < m_SortedScores[i].x)
			{
				temp.x = m_SortedScores[i].x;
				temp.y = m_SortedScores[i].y;

				m_SortedScores[i].x = m_SortedScores[j].x;
				m_SortedScores[i].y = m_SortedScores[j].y;

				m_SortedScores[j].x = temp.x;
				m_SortedScores[j].y = temp.y;
			}
		}
	}

	//Sets position of score text to display
	int posY = 210;

	for (int n = 3; n >= 0; --n)
	{
		*m_PlayerColors[(int)m_SortedScores[n].y].GetQuad()->GetPosition() = D3DXVECTOR2(200, posY);
		posY += 100;
	}
}




void GameScene::LoadInitialiseData()
{
	LoadScoreData();
	LoadPlayerData();
	LoadScoreScreenData();

	#pragma region Canvas

	InitialiseData canvasUI;
	canvasUI.Clear();

	canvasUI.dataFilepath = L"data/GFX/canvas.png";
	canvasUI.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/canvas.png";

	canvasUI.dataType = DataType::OBJECT2D;
	canvasUI.dataPtr = m_Canvas;

	canvasUI.pos = D3DXVECTOR2(0, 0);
	canvasUI.size = D3DXVECTOR2(1600, 46);

	m_InitialiseData.push_back(canvasUI);
	canvasUI.Clear();

	#pragma endregion 
}

void GameScene::LoadScoreData()
{
	#pragma region Numbers
	
	InitialiseData redScore, pinkScore, blueScore, greenScore;

	redScore.Clear();
	pinkScore.Clear();
	blueScore.Clear();
	greenScore.Clear();

	//Red
	redScore.dataFilepath = L"data/GFX/Scores/Red/0.png";
	redScore.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/Red/0.png";

	redScore.dataType = DataType::SCORE;
	redScore.dataPtr = &m_Scores[0];

	redScore.pos = D3DXVECTOR2(5, 45);
	redScore.size = D3DXVECTOR2(41, 54);

	//Pink
	pinkScore.dataFilepath = L"data/GFX/Scores/Pink/0.png";
	pinkScore.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/Pink/0.png";

	pinkScore.dataType = DataType::SCORE;
	pinkScore.dataPtr = &m_Scores[1];

	pinkScore.pos = D3DXVECTOR2(1378, 45);
	pinkScore.size = D3DXVECTOR2(41, 54);

	//Blue
	blueScore.dataFilepath = L"data/GFX/Scores/Blue/0.png";
	blueScore.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/Blue/0.png";

	blueScore.dataType = DataType::SCORE;
	blueScore.dataPtr = &m_Scores[2];

	blueScore.pos = D3DXVECTOR2(5, 842);
	blueScore.size = D3DXVECTOR2(41, 54);

	//Green
	greenScore.dataFilepath = L"data/GFX/Scores/Green/0.png";
	greenScore.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/Green/0.png";

	greenScore.dataType = DataType::SCORE;
	greenScore.dataPtr = &m_Scores[3];

	greenScore.pos = D3DXVECTOR2(1378, 842);
	greenScore.size = D3DXVECTOR2(41, 54);


	m_InitialiseData.push_back(redScore);
	m_InitialiseData.push_back(pinkScore);
	m_InitialiseData.push_back(blueScore);
	m_InitialiseData.push_back(greenScore);

	redScore.Clear();
	pinkScore.Clear();
	blueScore.Clear();
	greenScore.Clear();

	#pragma endregion 
	#pragma region decimal points

	InitialiseData redPoint, pinkPoint, bluePoint, greenPoint;

	redPoint.Clear();
	pinkPoint.Clear();
	bluePoint.Clear();
	greenPoint.Clear();

	//red
	redPoint.dataFilepath = L"data/GFX/Scores/Red/point.png";
	redPoint.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/Red/point.png";

	redPoint.dataType = DataType::OBJECT2D;
	redPoint.dataPtr = &m_DecimalPoints[0];

	redPoint.pos = D3DXVECTOR2(85, 78);
	redPoint.size = D3DXVECTOR2(16, 16);

	//pink
	pinkPoint.dataFilepath = L"data/GFX/Scores/Pink/point.png";
	pinkPoint.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/Pink/point.png";

	pinkPoint.dataType = DataType::OBJECT2D;
	pinkPoint.dataPtr = &m_DecimalPoints[1];

	pinkPoint.pos = D3DXVECTOR2(1458, 78);
	pinkPoint.size = D3DXVECTOR2(16, 16);

	//blue
	bluePoint.dataFilepath = L"data/GFX/Scores/Blue/point.png";
	bluePoint.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/Blue/point.png";

	bluePoint.dataType = DataType::OBJECT2D;
	bluePoint.dataPtr = &m_DecimalPoints[2];

	bluePoint.pos = D3DXVECTOR2(85, 875);
	bluePoint.size = D3DXVECTOR2(16, 16);

	//green
	greenPoint.dataFilepath = L"data/GFX/Scores/Green/point.png";
	greenPoint.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/Green/point.png";

	greenPoint.dataType = DataType::OBJECT2D;
	greenPoint.dataPtr = &m_DecimalPoints[3];

	greenPoint.pos = D3DXVECTOR2(1458, 875);
	greenPoint.size = D3DXVECTOR2(16, 16);

	m_InitialiseData.push_back(redPoint);
	m_InitialiseData.push_back(pinkPoint);
	m_InitialiseData.push_back(bluePoint);
	m_InitialiseData.push_back(greenPoint);

	redPoint.Clear();
	pinkPoint.Clear();
	bluePoint.Clear();
	greenPoint.Clear();

	#pragma endregion 
	#pragma region Percentage signs

	InitialiseData redSign, pinkSign, blueSign, greenSign;

	redSign.Clear();
	pinkSign.Clear();
	blueSign.Clear();
	greenSign.Clear();

	//red
	redSign.dataFilepath = L"data/GFX/Scores/Red/percentage.png";
	redSign.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/Red/percentage.png";

	redSign.dataType = DataType::OBJECT2D;
	redSign.dataPtr = &m_PercentageSigns[0];

	redSign.pos = D3DXVECTOR2(176, 48);
	redSign.size = D3DXVECTOR2(44, 48);

	//pink
	pinkSign.dataFilepath = L"data/GFX/Scores/Pink/percentage.png";
	pinkSign.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/Pink/percentage.png";

	pinkSign.dataType = DataType::OBJECT2D;
	pinkSign.dataPtr = &m_PercentageSigns[1];

	pinkSign.pos = D3DXVECTOR2(1549, 48);
	pinkSign.size = D3DXVECTOR2(44, 48);

	//Blue
	blueSign.dataFilepath = L"data/GFX/Scores/Blue/percentage.png";
	blueSign.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/Blue/percentage.png";

	blueSign.dataType = DataType::OBJECT2D;
	blueSign.dataPtr = &m_PercentageSigns[2];

	blueSign.pos = D3DXVECTOR2(176, 845);
	blueSign.size = D3DXVECTOR2(44, 48);

	//Green
	greenSign.dataFilepath = L"data/GFX/Scores/Green/percentage.png";
	greenSign.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/Green/percentage.png";

	greenSign.dataType = DataType::OBJECT2D;
	greenSign.dataPtr = &m_PercentageSigns[3];

	greenSign.pos = D3DXVECTOR2(1549, 845);
	greenSign.size = D3DXVECTOR2(44, 48);

	m_InitialiseData.push_back(redSign);
	m_InitialiseData.push_back(pinkSign);
	m_InitialiseData.push_back(blueSign);
	m_InitialiseData.push_back(greenSign);

	redSign.Clear();
	pinkSign.Clear();
	blueSign.Clear();
	greenSign.Clear();

	#pragma endregion 
}

void GameScene::LoadPlayerData()
{
	InitialiseData p1, p2, p3, p4;

	p1.Clear();
	p2.Clear();
	p3.Clear();
	p4.Clear();

	//Player 1
	p1.dataFilepath = L"data/GFX/redBucket.png";
	p1.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/redBucket.png";

	p1.dataType = DataType::PLAYER;
	p1.dataPtr = m_Player1;

	p1.pos = P1_POS;
	p1.size = D3DXVECTOR2(34, 32);

	//Player 2
	p2.dataFilepath = L"data/GFX/pinkBucket.png";
	p2.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/pinkBucket.png";

	p2.dataType = DataType::PLAYER;
	p2.dataPtr = m_Player2;

	p2.pos = P2_POS;
	p2.size = D3DXVECTOR2(34, 32);

	//Player 3
	p3.dataFilepath = L"data/GFX/blueBucket.png";
	p3.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/blueBucket.png";

	p3.dataType = DataType::PLAYER;
	p3.dataPtr = m_Player3;

	p3.pos = P3_POS;
	p3.size = D3DXVECTOR2(34, 32);

	//Player 4
	p4.dataFilepath = L"data/GFX/greenBucket.png";
	p4.dataFilepath_DEBUG = L"../ColourSpace/data/GFX/greenBucket.png";

	p4.dataType = DataType::PLAYER;
	p4.dataPtr = m_Player4;

	p4.pos = P4_POS;
	p4.size = D3DXVECTOR2(34, 32);

	m_InitialiseData.push_back(p1);
	m_InitialiseData.push_back(p2);
	m_InitialiseData.push_back(p3);
	m_InitialiseData.push_back(p4);

	p1.Clear();
	p2.Clear();
	p3.Clear();
	p4.Clear();
}

void GameScene::LoadScoreScreenData()
{
	//player colour text
	InitialiseData textColors[4];
	InitialiseData trophys[4];
	InitialiseData continueGame;
	InitialiseData yesNo;

	for (int i = 0; i < 4; i++)
	{
		textColors[i].Clear();
		trophys[i].Clear();
	}

	continueGame.Clear();
	yesNo.Clear();

	
	#pragma region Text loading

	//red text
	textColors[0].dataFilepath = L"data/GFX/Scores/red.png";
	textColors[0].dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/red.png";

	textColors[0].dataType = DataType::OBJECT2D;
	textColors[0].dataPtr = &m_PlayerColors[0];

	textColors[0].pos = D3DXVECTOR2(200, 210);
	textColors[0].size = D3DXVECTOR2(96, 44);

	//pink text
	textColors[1].dataFilepath = L"data/GFX/Scores/pink.png";
	textColors[1].dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/pink.png";

	textColors[1].dataType = DataType::OBJECT2D;
	textColors[1].dataPtr = &m_PlayerColors[1];

	textColors[1].pos = D3DXVECTOR2(200, 310);
	textColors[1].size = D3DXVECTOR2(112, 44);

	//blue text
	textColors[2].dataFilepath = L"data/GFX/Scores/blue.png";
	textColors[2].dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/blue.png";

	textColors[2].dataType = DataType::OBJECT2D;
	textColors[2].dataPtr = &m_PlayerColors[2];

	textColors[2].pos = D3DXVECTOR2(200, 410);
	textColors[2].size = D3DXVECTOR2(112, 44);

	//green text
	textColors[3].dataFilepath = L"data/GFX/Scores/green.png";
	textColors[3].dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/green.png";

	textColors[3].dataType = DataType::OBJECT2D;
	textColors[3].dataPtr = &m_PlayerColors[3];

	textColors[3].pos = D3DXVECTOR2(200, 510);
	textColors[3].size = D3DXVECTOR2(152, 44);
	
	#pragma endregion 
	#pragma region Trophy loading

	//first place trophy
	trophys[0].dataFilepath = L"data/GFX/Scores/cup_1.png";
	trophys[0].dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/cup_1.png";

	trophys[0].dataType = DataType::OBJECT2D;
	trophys[0].dataPtr = m_CupOne;

	trophys[0].pos = D3DXVECTOR2(100, 200);
	trophys[0].size = D3DXVECTOR2(64, 56);

	//second place trophy
	trophys[1].dataFilepath = L"data/GFX/Scores/cup_2.png";
	trophys[1].dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/cup_2.png";

	trophys[1].dataType = DataType::OBJECT2D;
	trophys[1].dataPtr = m_CupTwo;

	trophys[1].pos = D3DXVECTOR2(100, 300);
	trophys[1].size = D3DXVECTOR2(64, 56);

	//third place trophy
	trophys[2].dataFilepath = L"data/GFX/Scores/cup_3.png";
	trophys[2].dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/cup_3.png";

	trophys[2].dataType = DataType::OBJECT2D;
	trophys[2].dataPtr = m_CupThree;

	trophys[2].pos = D3DXVECTOR2(100, 400);
	trophys[2].size = D3DXVECTOR2(64, 56);

	//fourth place trophy
	trophys[3].dataFilepath = L"data/GFX/Scores/cup_4.png";
	trophys[3].dataFilepath_DEBUG = L"../ColourSpace/data/GFX/Scores/cup_4.png";

	trophys[3].dataType = DataType::OBJECT2D;
	trophys[3].dataPtr = m_CupFour;

	trophys[3].pos = D3DXVECTOR2(100, 500);
	trophys[3].size = D3DXVECTOR2(64, 56);

	#pragma endregion 


	for (int i = 0; i < 4; i++)
	{
		m_InitialiseData.push_back(textColors[i]);
		m_InitialiseData.push_back(trophys[i]);
	}

	//m_InitialiseData.push_back(continueGame);
	//m_InitialiseData.push_back(yesNo);

	for (int i = 0; i < 4; i++)
	{
		textColors[i].Clear();
		trophys[i].Clear();
	}

	continueGame.Clear();
	yesNo.Clear();
}

bool GameScene::LoadSoundData()
{

#if _DEBUG
	if (!m_DrawingSoundBuffer.loadFromFile(
		"../ColourSpace/data/SFX/paintingOGG.ogg")) { return false; }

	if (!m_chaseSoundBuffer.loadFromFile(
		"../ColourSpace/data/SFX/AI_ChaseOGG.ogg")) { return false; }

	if (!m_dieSoundBuffer.loadFromFile(
		"../ColourSpace/data/SFX/playerDeathOGG.ogg")) { return false; }

	if (!m_Music.openFromFile(
		"../ColourSpace/data/BGM/AngelicStrainOGG.ogg")) { return false; }

	if (!m_victorySound.openFromFile(
		"../ColourSpace/data/BGM/victoryOGG.ogg")) { return false; }
#else
	if (!m_DrawingSoundBuffer.loadFromFile(
		"data/SFX/paintingOGG.ogg")) { return false; }

	if (!m_Music.openFromFile(
		"data/BGM/AngelicStrainOGG.ogg")) { return false; }

	if (!m_chaseSoundBuffer.loadFromFile(
		"data/SFX/AI_ChaseOGG.ogg")) { return false; }

	if (!m_dieSoundBuffer.loadFromFile(
		"data/SFX/playerDeathOGG.ogg")) { return false; }

	if (!m_victorySound.openFromFile(
		"data/BGM/victoryOGG.ogg")) { return false; }
#endif
	
	m_DrawingSound.setBuffer(m_DrawingSoundBuffer);
	m_chaseSound.setBuffer(m_chaseSoundBuffer);
	m_dieSound.setBuffer(m_dieSoundBuffer);

	m_DrawingSound.setLoop(true);
	m_chaseSound.setLoop(true);

	m_DrawingSound.setVolume(7);
	m_Music.setVolume(40);
	m_victorySound.setVolume(30);

	m_Music.setLoop(true);

	return true;
}
