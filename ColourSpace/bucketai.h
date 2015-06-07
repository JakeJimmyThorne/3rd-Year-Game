#ifndef _BUCKETAI_H_
#define _BUCKETAI_H_

#include "player.h"
#include "fluidgrid.h"
#include "utility.h"

//amount per second of increase
const float BASE_SPEED_INCREASE = 0.05f;

const int MIN_BRANCH_INTERVAL = 10;
const int MAX_BRANCH_INTERVAL = 15;

const float PATH_BRANCH_CHANCE = 10;

const float MIN_FLIPOUT_TIME = 0.3f;
const float MAX_FLIPOUT_TIME = 1.1f;

const float AGGRESSION_RADIUS = 200;
const float FLIPOUT_CHANCE = 20;
const float FLIPOUT_CHANCE_INTERVAL = 1;

const float PATROL_PATH_CHANGE = 0.4f;

class BucketAI
{
public:
	static void IncrementBaseSpeed(float);
	static void ResetBaseSpeed();
	static void LinkPlayerPosition(int, D3DXVECTOR2*);

public:
	BucketAI();
	~BucketAI();

	void Initialise(ID3D11Device*, ID3D11DeviceContext*, HWND*, int, int);
	void InitialiseFlipout();

	void SpawnBucket();

	void Shutdown();

	void Update(float, FluidGrid&);
	bool UpdateFlipout(float);

	void Patrol();

	bool IsActive() { return m_isActive; }
	void SetInactive() { m_isActive = false; }

	Player* GetBucket() { return m_bucket; }

private:

	D3DXVECTOR2 FindNearestPlayer();

private:
	static float s_baseSpeed;
	static float s_branchIncrease;
	static float s_radiusIncrease;

	static std::vector<D3DXVECTOR2*> s_playerPositions;

	int m_screenWidth, 
		m_screenHeight;

	Player* m_bucket;

	D3DXVECTOR2 m_position;
	D3DXVECTOR2 m_direction;

	float m_additionalSpeed,
		  m_additionalBranchChance;

	float m_chaseTime;
	float m_branchInterval;
	float m_branchTimer;

	float m_flipoutDuration;
	float m_flipoutTimer;
	float m_flipoutChanceTimer;
	D3DXVECTOR2 m_flipoutTarget;

	bool m_isActive;
	bool m_isFlippingOut;


	float m_patrolTimer;
};

#endif