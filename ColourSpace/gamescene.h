#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include <d3dx11.h>
#include <d3dx10math.h>
#include <vector>
#include <math.h>

#include "settings.h"
#include "d3dclass.h"

#include "Rendering\renderer.h"
#include "score.h"

#include "object2d.h"
#include "player.h"
#include "uiobject.h"

#include "fluidgrid.h"
#include "bucketai.h"

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

//in seconds
const bool AI_ENABLED = true;

const D3DXVECTOR2 BUCKET_TIP = D3DXVECTOR2(28, -17);

const int START_PATH_INTERVAL = 3;
const int AI_CREATE_INTERVAL = 30;
const int AI_CREATE_FIRST = 7;

const int MAX_AI = 12;


const float SHOW_SCORE_TIME = 6;

const D3DXVECTOR2 P1_POS = D3DXVECTOR2(400, 200),
				  P2_POS = D3DXVECTOR2(1200, 200),
				  P3_POS = D3DXVECTOR2(400, 640),
				  P4_POS = D3DXVECTOR2(1200, 640);

class GameScene
{
public:

	enum DataType
	{
		OBJECT2D, SCORE, PLAYER, SOUNDOBJECT
	};

	struct InitialiseData
	{
		void Clear() 
		{
			dataPtr = 0;
			dataFilepath = 0;
			dataFilepath_DEBUG = 0;
		}

		DataType dataType;
		void* dataPtr;

		WCHAR* dataFilepath;
		WCHAR* dataFilepath_DEBUG;

		D3DXVECTOR2 pos, size;
	};

public:
	GameScene();
	~GameScene();

	bool Initialise(Renderer*,ID3D11DeviceContext*, ID3D11Device*, HWND*, D3DXMATRIX*, D3DXMATRIX*, D3DXMATRIX*, int, int);
	bool LoadArena(ID3D11DeviceContext*, ID3D11Device*,int,int,int);
	void Shutdown();
	bool Frame(float, ID3D11DeviceContext*);

	void StopMusic() { m_Music.stop(); }

	void PlayerCollision(ID3D11DeviceContext*, int, int, D3DXVECTOR4);
	void CheckCollisionPosition(int, int, int, int);
	bool enableUp;
	bool enableDown;
	bool enableLeft;
	bool enableRight;

	D3DXVECTOR3 GetPlayerPosition(int);
	
	//render the 2d scene
	void RenderScene2D(ID3D11DeviceContext*, D3DXMATRIX*, D3DXMATRIX*);

private:
	void UpdateAI(float);
	void PlayerPaintLogic(int, float, ID3D11DeviceContext*);

	void LoadInitialiseData();

	void LoadScoreData();
	void LoadPlayerData();
	void LoadScoreScreenData();
	bool LoadSoundData();

private:
	Renderer* m_Renderer;
//	SoundManager* m_SoundManager;

	std::vector<BucketAI> m_aiBucketList;
	int m_aiActiveSize;
	float m_startPathTimer;

	float m_aiSpawnTimer;
	bool m_firstAISpawned;

	std::vector<InitialiseData> m_InitialiseData;

	//list objects here normally
	Object2D* m_Arena;
	float m_levelTimer;

	Object2D* m_Canvas;
	Object2D* m_Checkers;

	Object2D* m_CalculatingScore;
	Object2D* m_Scoreboard;
	Score m_Scores[4];
	Object2D* m_DecimalPoints;
	Object2D* m_PercentageSigns;

	//score screen 
	Object2D m_PlayerColors[4];
	Object2D* m_Continue;
	Object2D* m_YesNo;

	Object2D *m_CupOne, *m_CupTwo, *m_CupThree, *m_CupFour;
	bool m_showScore;
	float m_showScoreTimer;
	void SortScores();
	D3DXVECTOR2 m_SortedScores[4];

	sf::Music m_Music;

	sf::SoundBuffer m_DrawingSoundBuffer;
	sf::Sound m_DrawingSound;

	sf::SoundBuffer m_chaseSoundBuffer;
	sf::Sound m_chaseSound;
	bool m_playingChaseSound;
	int m_aiPlayingSound; //ref to the ai that started the sound

	sf::SoundBuffer m_dieSoundBuffer;
	sf::Sound m_dieSound;

	sf::Music m_victorySound;

	bool m_calculateScore;
	float m_calculateTimer;
	int m_roundScore[4];
	bool m_setScore;

	int minX;
	int maxX;
	int minY;
	int maxY;

	//object list, add objects to it at the end of initialise
	std::vector<Object2D*> m_objectList;
	std::vector<Object2D*> m_scoreList;

	//players
	Player* m_Player1;
	Player* m_Player2;
	Player* m_Player3;
	Player* m_Player4;

	bool m_isPlaying[4];

	std::vector<Player*> m_playerList;

	FluidGrid m_FluidGrid;

	bool m_arenaPushedBack;
	int arenaPushBack;

	int deadCount;

	bool m_flip;
	float flipTimer;
};

#endif