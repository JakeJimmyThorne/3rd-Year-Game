#include "bucketai.h"

std::vector<D3DXVECTOR2*> BucketAI::s_playerPositions;
float BucketAI::s_baseSpeed = 1;
float BucketAI::s_branchIncrease = 0;
float BucketAI::s_radiusIncrease = 0;

void BucketAI::IncrementBaseSpeed(float _dt)
{
	if (s_baseSpeed < 5)
		s_baseSpeed += _dt * BASE_SPEED_INCREASE;

	if (s_branchIncrease < 50)
		s_branchIncrease += _dt * BASE_SPEED_INCREASE;

	if (s_radiusIncrease < 300)
	{
		s_radiusIncrease += _dt;
	}
}

void BucketAI::ResetBaseSpeed()
{
	s_baseSpeed = 1;
	s_branchIncrease = 0;
	s_radiusIncrease = 0;
}

void BucketAI::LinkPlayerPosition(int _index, D3DXVECTOR2* _pos)
{
	//If the vector hasn't been set, set it
	if (s_playerPositions.size() != 4)
	{
		s_playerPositions.resize(4);
	}

	s_playerPositions[_index] = _pos;
}

BucketAI::BucketAI() {}
BucketAI::~BucketAI(){}

void BucketAI::Initialise(ID3D11Device* _dev, ID3D11DeviceContext* _dc, HWND* _hwnd, int _w, int _h)
{
	m_bucket = new Player;
	m_position = D3DXVECTOR2(-100, 400);
	m_direction = D3DXVECTOR2(1, 0);

	m_screenWidth = _w;
	m_screenHeight = _h;

	m_isActive = false;
	m_isFlippingOut = false;
	m_additionalSpeed = 0;
	m_additionalBranchChance = 0;
	m_flipoutDuration = 0;
	m_chaseTime = 0;
	m_branchInterval = 1;
	m_patrolTimer = 0;

	m_bucket->Initialise(_dev, _dc, *_hwnd,
#if _DEBUG
						 L"../ColourSpace/data/GFX/blackBucket.png",
#else
						 L"data/GFX/blackBucket.png",
#endif	
						 -1, //AI index
						 _w, _h,

						 m_position,
						 D3DXVECTOR2(34, 32));

	m_bucket->SetPlayerColour(D3DXVECTOR4(0, 0, 0, 1));
}

void BucketAI::InitialiseFlipout()
{
	if (!m_isFlippingOut)
	{
		m_flipoutTarget = FindNearestPlayer();

		float diff = 0;
		diff += Utility::Difference(m_position.x, m_flipoutTarget.x);
		diff += Utility::Difference(m_position.y, m_flipoutTarget.y);

		if (diff > AGGRESSION_RADIUS + s_radiusIncrease)
			return;

		m_additionalSpeed = 2;
		m_additionalBranchChance = 10 + s_baseSpeed / 2;
		m_flipoutTimer = 0;

		m_branchTimer = 0;

		m_flipoutDuration = Utility::RandomFloat(MIN_FLIPOUT_TIME, MAX_FLIPOUT_TIME);

		m_isFlippingOut = true;
	}
}

void BucketAI::SpawnBucket()
{
	if (!m_isActive)
	{
		m_additionalSpeed = 0;
		m_chaseTime = 0;
		m_additionalBranchChance = 0;
		m_flipoutChanceTimer = 0;

		m_branchInterval = 1.5f;
		
		m_isActive = true;

		//0 = top, 1 = right, 2 = bottom, 3 = left
		int randomSide = rand() % 4;
		int randomX = Utility::RandomFloat(50, m_screenWidth - 50);
		int randomY = Utility::RandomFloat(50, m_screenHeight - 50);

		//spawn the black bucket at a random side
		switch (randomSide)
		{
		case 0:
		{
			m_position = D3DXVECTOR2(randomX, -50);
			m_direction = D3DXVECTOR2(0, 1);
			break;
		}
		case 1:
		{
			m_position = D3DXVECTOR2(1650, randomY);
			m_direction = D3DXVECTOR2(-1, 0);
			break;
		}
		case 2:
		{
			m_position = D3DXVECTOR2(randomX, 900);
			m_direction = D3DXVECTOR2(0, -1);
			break;
		}
		case 3:
		{
			m_position = D3DXVECTOR2(-50, randomY);
			m_direction = D3DXVECTOR2(1, 0);
			break;
		}
		}
	}
}

void BucketAI::Update(float _dt, FluidGrid& _grid)
{
	if (m_isActive)
	{
		m_chaseTime += _dt;

		m_position += D3DXVECTOR2((s_baseSpeed + m_additionalSpeed) * m_direction.x,
							      (s_baseSpeed + m_additionalSpeed) * m_direction.y);

		*m_bucket->GetQuad()->GetPosition() = m_position;

		//if within the canvas we want to paint
		if (m_position.x > -26 && m_position.x < m_screenWidth &&
			m_position.y > 0 && m_position.y < m_screenHeight)
		{
			if (m_isFlippingOut)
			{
				_grid.LayBlack(m_bucket->GetQuad()->GetPosition()->x + 26,
							   m_bucket->GetQuad()->GetPosition()->y - 12,
							   m_bucket->GetPlayerColour());
			}
			else
			{
				m_flipoutChanceTimer += _dt;
				m_patrolTimer += _dt;

				Patrol();

				if (m_flipoutChanceTimer > FLIPOUT_CHANCE_INTERVAL)
				{
					if (Utility::RandomFloat(0.0f, 100.0f) < FLIPOUT_CHANCE + s_baseSpeed + s_branchIncrease)
					{
						InitialiseFlipout();
					}

					m_flipoutChanceTimer = 0;
				}
				
			}
		}
		else
		{
			if (m_isActive && m_chaseTime > 10)
			{
				//delete
				m_isActive = false;
			}
		}
	}

}

bool BucketAI::UpdateFlipout(float _dt)
{
	if (m_isFlippingOut)
	{
		m_flipoutTimer += _dt;

		if (m_flipoutTimer < m_flipoutDuration)
		{
			if (Utility::RandomFloat(0.0f, 100.0f) < PATH_BRANCH_CHANCE + m_additionalBranchChance)
			{
				bool newPath = false;
				//if the ai is going left or right, we only want to branch upwards or downwards
				if (m_direction == D3DXVECTOR2(1, 0) || m_direction == D3DXVECTOR2(-1, 0))
				{
					if (m_position.y < m_flipoutTarget.y)
					{
						if (!newPath)
						{
							m_direction = D3DXVECTOR2(0, 1);
							newPath = true;
						}
					}
					else if (m_position.y > m_flipoutTarget.y)
					{
						if (!newPath)
						{
							m_direction = D3DXVECTOR2(0, -1);
							newPath = true;
						}
					}
				}
				//else up/down
				if (m_direction == D3DXVECTOR2(0, 1) || m_direction == D3DXVECTOR2(0, -1))
				{
					if (m_position.x < m_flipoutTarget.x)
					{
						if (!newPath)
						{
							m_direction = D3DXVECTOR2(1, 0);
							newPath = true;
						}
					}
					else if (m_position.x > m_flipoutTarget.x)
					{
						if (!newPath)
						{
							m_direction = D3DXVECTOR2(-1, 0);
							newPath = true;
						}
					}
				}
			}
		}
		else
		{
			m_additionalSpeed = 0;
			m_additionalBranchChance = 0;
			m_isFlippingOut = false;
		}

		return true;
	}

	return false;
}

void BucketAI::Patrol()
{
	if (m_patrolTimer > PATROL_PATH_CHANGE)
	{
		if (Utility::RandomFloat(0.0f, 100.0f) < PATH_BRANCH_CHANCE)
		{
			bool newPath = false;
			//if the ai is going left or right, we only want to branch upwards or downwards
			if (m_direction == D3DXVECTOR2(1, 0) || m_direction == D3DXVECTOR2(-1, 0))
			{
				if (m_position.y < m_flipoutTarget.y)
				{
					if (!newPath)
					{
						m_direction = D3DXVECTOR2(0, 1);
						newPath = true;
					}
				}
				else if (m_position.y > m_flipoutTarget.y)
				{
					if (!newPath)
					{
						m_direction = D3DXVECTOR2(0, -1);
						newPath = true;
					}
				}
			}
			//else up/down
			if (m_direction == D3DXVECTOR2(0, 1) || m_direction == D3DXVECTOR2(0, -1))
			{
				if (m_position.x < m_flipoutTarget.x)
				{
					if (!newPath)
					{
						m_direction = D3DXVECTOR2(1, 0);
						newPath = true;
					}
				}
				else if (m_position.x > m_flipoutTarget.x)
				{
					if (!newPath)
					{
						m_direction = D3DXVECTOR2(-1, 0);
						newPath = true;
					}
				}
			}
		}

		m_patrolTimer = 0;
	}
}

D3DXVECTOR2 BucketAI::FindNearestPlayer()
{
	float distances[4] = { 0, 0, 0, 0 };

	//make it bigger than the canvas just to make sure that it is
	//bigger than the biggest distance 
	float smallestDistance = m_screenHeight + m_screenWidth;

	for (int i = 0; i < 4; i++)
	{
		distances[i] += Utility::Difference(m_position.x, s_playerPositions[i]->x);
		distances[i] += Utility::Difference(m_position.y, s_playerPositions[i]->y);

		if (distances[i] < smallestDistance)
			smallestDistance = distances[i];
	}

	for (int i = 0; i < 4; i++)
	{
		if (distances[i] == smallestDistance)
		{
			return *s_playerPositions[i];
		}
	}

	//direct them to the middle if for some reason the return isn't hit in the 
	//above for loop
	return D3DXVECTOR2(800, 400);
}


