#include "fluidgrid.h"

#define AtCoords(x,y) y*m_width + x

FluidGrid::FluidGrid()
{
	m_fluidTimer = 0;
	m_ColorGrid = 0;
	m_OriginalGrid = 0;
	m_ScoreCounts = 0;
}

FluidGrid::~FluidGrid()
{

}

bool FluidGrid::Initialise(int w, int h, ID3D11Device* device, ID3D11DeviceContext* dc, WCHAR* filename)
{
	m_width = w;
	m_height = h;

	if (!m_ColorGrid)
		m_ColorGrid = new GridUnit[m_width * m_height];

	if (!m_OriginalGrid)
		m_OriginalGrid = new GridUnit[m_width * m_height];

	if (!m_ScoreCounts)
		m_ScoreCounts = new ScorePositions[m_width * m_height];

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			m_ColorGrid[At(x, y)].r = 0;
			m_ColorGrid[At(x, y)].g = 0;
			m_ColorGrid[At(x, y)].b = 0;
			m_ColorGrid[At(x, y)].a = 1;

			m_ScoreCounts[At(x, y)].player1Colour = false;
			m_ScoreCounts[At(x, y)].player2Colour = false;
			m_ScoreCounts[At(x, y)].player3Colour = false;
			m_ScoreCounts[At(x, y)].player4Colour = false;
		}
	}

	m_GridStats.Initialise(w, h);

	//Create the paint textures
	m_arenaPaint.LoadBackgroundImage(m_width, m_height, device, filename);
	m_arenaPaint.CreateTexture(device, dc, m_ColorGrid);

	//store the original grid
	*m_OriginalGrid = *m_ColorGrid;

	//basically tells the fluid to spread every 0.15f seconds
	m_viscosity = 1.0f / UPDATE_FPS;
	m_viscosity = 0.02f;
	m_density = 40;

	updateCache.clear();
	activeCache.clear();
	flaggedForDeletion.clear();
	deletionCache.clear();

	updateCache.resize(20000);
	activeCache.resize(20000);
	flaggedForDeletion.resize(20000);
	deletionCache.resize(20000);
	deletionPos = 0;
	poolPos = 0;

	for (int i = 0; i < updateCache.size(); ++i)
	{
		updateCache[i].first.r = 0.0f;
		updateCache[i].first.g = 0.0f;
		updateCache[i].first.b = 0.0f;
		updateCache[i].first.a = 0.0f;
	}

	playerScores[0] = 0;
	playerScores[1] = 0;
	playerScores[2] = 0;
	playerScores[3] = 0;

	activeSize = 0;
	headPos = 0;
	deletionSize = 0;
	m_testCollisionTimer = 0;

	return true;
}

void FluidGrid::LayPaint(int x, int y, D3DXVECTOR4 color, bool playerStuck)
{
	//error checking and making sure the player can't paint over another players colour
	for (int i = y - 3; i < y + 4; ++i)
	{
		for (int j = x - 3; j < x + 4; ++j)
		{
			if (j <= 0 || j >= 1600)
				return;

			if (i <= 0 || i >= 855)
				return;

			if (!playerStuck)
			{
				if (HasOtherColor(m_ColorGrid[At(j, i)], color))
				{
					if (rand() % 100 < 15)
						return;
				}
			}
		}
	}

	for (int i = y - 2; i < y + 3; ++i)
	{
		for (int j = x - 2; j < x + 3; ++j)
		{
			if (CanMoveTo(m_ColorGrid[At(j, i)], m_ColorGrid[At(x, y)]))
			{
				m_ColorGrid[At(j, i)].r = color.x;
				m_ColorGrid[At(j, i)].g = color.y;
				m_ColorGrid[At(j, i)].b = color.z;
				m_ColorGrid[At(j, i)].a = color.w;

				m_GridStats.minX = x - 2;
				m_GridStats.maxX = x + 3;

				m_GridStats.minY = y - 2;
				m_GridStats.maxY = y + 3;

				m_GridStats.m_tileUnits[j][i] = m_density;

				if (ENABLE_CACHING)
				{
					SendToCache(m_ColorGrid[At(j, i)], D3DXVECTOR2(j, i));
				}
			}
		}
	}
}

void FluidGrid::LayBlack(int x, int y, D3DXVECTOR4 color)
{
	for (int i = y - 2; i < y + 3; ++i)
	{
		for (int j = x - 2; j < x + 3; ++j)
		{
			if (j <= 0 || j >= 1600)
				return;

			if (i <= 0 || i >= 855)
				return;

				m_ColorGrid[At(j, i)].r = color.x;
				m_ColorGrid[At(j, i)].g = color.y;
				m_ColorGrid[At(j, i)].b = color.z;
				m_ColorGrid[At(j, i)].a = color.w;

				m_GridStats.minX = x - 2;
				m_GridStats.maxX = x + 3;

				m_GridStats.minY = y - 2;
				m_GridStats.maxY = y + 3;

				m_GridStats.m_tileUnits[j][i] = 35;

				if (ENABLE_CACHING)
				{
					//SendToCache(&m_ColorGrid[At(j, i)], D3DXVECTOR2(j, i));
					SendToCache(m_ColorGrid[At(j, i)], D3DXVECTOR2(j, i));
				}
				
		}
	}
}

void FluidGrid::DrawPencil(int x, int y, D3DXVECTOR4 color)
{
	for (int i = y - 1; i < y + 1; ++i)
	{
		for (int j = x - 1; j < x + 1; ++j)
		{
			m_ColorGrid[At(j, i)].r = color.x;
			m_ColorGrid[At(j, i)].g = color.y;
			m_ColorGrid[At(j, i)].b = color.z;
			m_ColorGrid[At(j, i)].a = color.w;

			m_GridStats.minX = x - 2;
			m_GridStats.maxX = x + 3;

			m_GridStats.minY = y - 2;
			m_GridStats.maxY = y + 3;

			m_GridStats.m_tileUnits[j][i] = 1;
			//m_GridStats.m_isPencil[j][i] = true;

			if (ENABLE_CACHING)
			{
				//SendToCache(&m_ColorGrid[At(j, i)], D3DXVECTOR2(j, i));
				SendToCache(m_ColorGrid[At(j, i)], D3DXVECTOR2(j, i));
			}
		}
	}
}

void FluidGrid::Update(float frameTime, ID3D11DeviceContext* dc)
{
	m_fluidTimer += frameTime;
	m_testCollisionTimer += frameTime;

	if (m_fluidTimer > m_viscosity)
	{
		if (ENABLE_CACHING)
		{
			//std::cout << activeSize << std::endl;

			for (int i = 0; i < activeSize; i++)
			{
				if (HasColor(updateCache[activeCache[i]].first))
				{
					int x = (int)updateCache[activeCache[i]].second.x;
					int y = (int)updateCache[activeCache[i]].second.y;

					//Thinnest layer of fluid a pixel can have, this helps spread the fluid
					if (m_GridStats.m_tileUnits[x][y] <= 1)
					{
						RemoveFromCache(i, D3DXVECTOR2(x, y));
						continue;
					}

			
					//width check - only change if inside of the array area
					if (x > 1 && x < m_width - 2)
					{
						if (y > 1 && y < m_height - 2)
						{
							//Converts the position to grid space
							D3DXVECTOR2 scaledPos = ConvertToExpandedGrid(x, y);

							//remove from the cache if on black or rendering box
							if (!CanMoveTo(m_ColorGrid[At(x, y)], m_ColorGrid[At(x, y)]))
							{
								RemoveFromCache(i, D3DXVECTOR2(x, y));
								continue;
							}

							//test to see if it can spread to an empty space
							if (TestForEmpty((int)scaledPos.x, (int)scaledPos.y))
								continue;

							if (TestForCollision((int)scaledPos.x, (int)scaledPos.y))
									continue;

							//test to see if we can spread out paint units
							if (TestForSpread((int)scaledPos.x, (int)scaledPos.y))
								continue;

							//If the fluid isn't doing anything, remove it
							RemoveFromCache(i, D3DXVECTOR2(x, y));
						}
					}
				}
			}
		}

		if (m_testCollisionTimer > UPDATE_COLLISION)
			m_testCollisionTimer = 0;

		//Deals with cache of items that are to be deleted
		for (int k = 0; k < deletionSize; ++k)
		{
			flaggedForDeletion[deletionCache[k]].second -= frameTime;

			if (flaggedForDeletion[deletionCache[k]].second <= 0.0f)
			{
				for (int i = flaggedForDeletion[deletionCache[k]].first.y - 2; i < flaggedForDeletion[deletionCache[k]].first.y + 3; ++i)
				{
					for (int j = flaggedForDeletion[deletionCache[k]].first.x - 2; j < flaggedForDeletion[deletionCache[k]].first.x + 3; ++j)
					{
						if (j > 0 && j < 1600 && i > 0 && i < 853)
						{
							m_ColorGrid[At(j, i)].r = 1.0f;
							m_ColorGrid[At(j, i)].g = 0.0f;
							m_ColorGrid[At(j, i)].b = 1.0f;
						}
					}
				}
				int temp = deletionCache[deletionSize - 1];

				deletionCache[k] = temp;

				deletionSize--;

			}
		}

		m_fluidTimer = 0;
		m_GridStats.Reset(m_width, m_height);
	}
}

bool FluidGrid::TestForEmpty(int _x, int _y)
{
	//We randomise check order so that no direction is always checked first
	RandomiseCheck checkOrder;
	checkOrder.Randomise();

	for (int i = 0; i < 4; i++)
	{
		//Modifiers to know which tile to test
		int x = 2, y = 2;

		switch (checkOrder.m_order[i])
		{
		case 0: //UP
			x = 0;
			y = 1;
			break;
		case 1: //RIGHT
			x = 1;
			y = 0;
			break;
		case 2: //DOWN
			x = 0;
			y = -1;
			break;
		case 3: //LEFT
			x = -1;
			y = 0;
			break;
		default:
			break;
		}

		/*
		x and y are the direction to check
		_x and _y is the origin of the current point
		*/

		//Make sure they have been changed
		if (x != 2 && y != 2)
		{
			//If the pixel we're checking is empty
			if (!HasColor(m_ColorGrid[At(ConvBackX(_x + x), ConvBackY(_y + y))]))
			{
				//if not on black
				if (CanMoveTo(m_ColorGrid[At(ConvBackX(_x + x), ConvBackY(_y + y))], m_ColorGrid[At(ConvBackX(_x), ConvBackY(_y))]))
				{
					//and if it hasn't been changed
					if (!m_GridStats.m_tileChanged[At(ConvBackX(_x + x), ConvBackY(_y + y))])
					{
						//fill it with the same color
						m_ColorGrid[At(ConvBackX(_x + x), ConvBackY(_y + y))] = m_ColorGrid[At(ConvBackX(_x), ConvBackY(_y))];

						//spread the units
						m_GridStats.m_tileUnits[ConvBackX(_x + x)][ConvBackY(_y + y)]++;
						m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y)]--;

						//flag it as being changed
						m_GridStats.m_tileChanged[At(ConvBackX(_x + x), ConvBackY(_y + y))] = true;

						if (ENABLE_CACHING)
						{
							SendToCache(m_ColorGrid[At(ConvBackX(_x + x), ConvBackY(_y + y))], D3DXVECTOR2(ConvBackX(_x + x), ConvBackY(_y + y)));
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool FluidGrid::TestForCollision(int _x, int _y)
{
	//We randomise check order so that no direction is always checked first
	RandomiseCheck checkOrder;
	checkOrder.Randomise();

	//for (int i = 0; i < 4; i++)
	//{
		//Modifiers to know which tile to test
		int x = 2, y = 2;

		switch (checkOrder.m_order[0])
		{
		case 0: //UP
			x = 0;
			y = 1;
			break;
		case 1: //RIGHT
			x = 1;
			y = 0;
			break;
		case 2: //DOWN
			x = 0;
			y = -1;
			break;
		case 3: //LEFT
			x = -1;
			y = 0;
			break;
		default:
			break;
		}

		/*
		x and y are the direction to check
		_x and _y is the origin of the current point

		_x + x * 2 is the point behind the direction (this case it's two points to the right)
		*/

		//Make sure they have been changed
		if (x != 2 && y != 2)
		{
			//make sure that pixel hasn't been changed
			if (!m_GridStats.m_tileChanged[At(ConvBackX(_x + x), ConvBackY(_y + y))])
			{
				if (CanMoveTo(m_ColorGrid[At(ConvBackX(_x + x), ConvBackY(_y + y))], m_ColorGrid[At(ConvBackX(_x), ConvBackY(_y))]))
				{
					//If the pixel we're checking is not empty
					if (HasColor(m_ColorGrid[At(ConvBackX(_x + x), ConvBackY(_y + y))]))
					{
						//and if it is not the same color
						if (m_ColorGrid[At(ConvBackX(_x + x), ConvBackY(_y + y))] != m_ColorGrid[At(ConvBackX(_x),ConvBackY(_y))])
						{
							//if the other color isn't pencil
							if (!m_GridStats.m_isPencil[ConvBackX(_x + x)][ConvBackY(_y + y)])
							{
								//if the other tile has less units, only then do we spread
								if (m_GridStats.m_tileUnits[ConvBackX(_x + x)][ConvBackY(_y + y)] < m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackX(_y)])
								{
									//if the space behind it is within bounds
									if (InBounds(ConvBackX(_x + x * 2), ConvBackY(_y + y * 2)))
									{
										//push back half of the paint to the tile behind it, only if it isn't black
										if (!m_ColorGrid[At(ConvBackX(_x + x * 2), ConvBackY(_y + y * 2))].IsBlack())
										{
											//make sure that the color being pushed back is the same around it
											if (TestSameColor(ConvBackX(_x + x * 2), ConvBackY(_y + y * 2),
												ConvBackX(_x + x), ConvBackY(_y + y)))
											{
												m_GridStats.m_tileUnits[ConvBackX(_x + x * 2)][ConvBackY(_y + y * 2)] += m_GridStats.m_tileUnits[ConvBackX(_x + x)][ConvBackY(_y + y)] / 2;
												m_ColorGrid[At(ConvBackX(_x + x * 2), ConvBackY(_y + y * 2))] = m_ColorGrid[At(ConvBackX(_x + x), ConvBackY(_y + y))];
											}
										}
									}

									//fill it with the same color
									m_ColorGrid[At(ConvBackX(_x + x), ConvBackY(_y + y))] = m_ColorGrid[At(ConvBackX(_x), ConvBackY(_y))];

									//half and half the units
									m_GridStats.m_tileUnits[ConvBackX(_x + x)][ConvBackY(_y + y)] /= 2;
									m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y)] /= 2;

									//flag it as being changed
									m_GridStats.m_tileChanged[At(ConvBackX(_x + x), ConvBackY(_y + y))] = true;

									if (ENABLE_CACHING)
									{
										//Send it to the update cache
										SendToCache(m_ColorGrid[At(ConvBackX(_x + x),ConvBackY(_y + y))], D3DXVECTOR2(ConvBackX(_x + x),ConvBackY(_y + y)));
									}

									return true;
								}
							}
						}
					}
				}
			}
		}
	//}

	return false;
}

bool FluidGrid::TestForSpread(int _x, int _y)
{
	bool changed = false;

	for (int i = 0; i < SPREAD_SPEED; i++)
	{
		int x = 2, y = 2;

		D3DXVECTOR2 coords = TestForLeastUnits(_x, _y);
		x = coords.x;
		y = coords.y;

		/*
		x and y are the direction to check
		_x and _y is the origin of the current point
		*/

		//Make sure they have been changed
		if (x != 2 && y != 2)
		{
			//If the pixel we're checking has color
			if (HasColor(m_ColorGrid[At(ConvBackX(_x + x), ConvBackY(_y + y))]))
			{
				//make sure we spread to the same color
				if (m_ColorGrid[At(ConvBackX(_x + x), ConvBackY(_y + y))] == m_ColorGrid[At(ConvBackX(_x), ConvBackY(_y))])
				{
					//make sure we have enough units to spread
					if (m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y)] >= 1)
					{
						//make sure that we won't just be swapping back and forth units
						if (m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y)] > m_GridStats.m_tileUnits[ConvBackX(_x + x)][ConvBackY(_y + y)] + 1 ||
							m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y)] < m_GridStats.m_tileUnits[ConvBackX(_x + x)][ConvBackY(_y + y)] - 1)
						{
							//spread the units
							m_GridStats.m_tileUnits[ConvBackX(_x + x)][ConvBackY(_y + y)]++;
							m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y)]--;

							//flag it as being changed
							m_GridStats.m_tileChanged[At(ConvBackX(_x + x), ConvBackY(_y + y))] = true;

							//rid it of its pencil status
							m_GridStats.m_isPencil[ConvBackX(_x + x)][ConvBackY(_y + y)] = false;

							if (ENABLE_CACHING)
							{
								SendToCache(m_ColorGrid[At(ConvBackX(_x + x), ConvBackY(_y + y))], D3DXVECTOR2(ConvBackX(_x + x), ConvBackY(_y + y)));
							}

							changed = true;
						}
					}
				}
			}
		}
	}

	return changed;
}

bool FluidGrid::TestSameColor(int _x, int _y, int _xOrig, int _yOrig)
{
	if (m_ColorGrid[At(_x + 1, _y)] == m_ColorGrid[At(_xOrig, _yOrig)] &&    //right
		m_ColorGrid[At(_x - 1, _y)] == m_ColorGrid[At(_xOrig, _yOrig)] &&    //left
		m_ColorGrid[At(_x, _y + 1)] == m_ColorGrid[At(_xOrig, _yOrig)] &&    //up
		m_ColorGrid[At(_x, _y - 1)] == m_ColorGrid[At(_xOrig, _yOrig)])    //down
	{
		return true;
	}
	else return false;
}


D3DXVECTOR2 FluidGrid::TestForLeastUnits(int _x, int _y)
{
	D3DXVECTOR2 coordinates;

	bool left = false, right = false, up = false, down = false;

#pragma region testing area that has the least units

	//test between left and right
	if (m_GridStats.m_tileUnits[ConvBackX(_x - 1)][ConvBackY(_y)] > m_GridStats.m_tileUnits[ConvBackX(_x + 1)][ConvBackY(_y)])
	{
		right = true;
	}
	else if (m_GridStats.m_tileUnits[ConvBackX(_x - 1)][ConvBackY(_y)] < m_GridStats.m_tileUnits[ConvBackX(_x + 1)][ConvBackY(_y)])
	{
		left = true;
	}
	else //if equal
	{
		if (rand() % 100 > 50)
			right = true;
		else
			left = true;
	}

	//test between up and down
	if (m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y - 1)] > m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y + 1)])
	{
		up = true;
	}
	else if (m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y - 1)] < m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y + 1)])
	{
		down = true;
	}
	else //if equal
	{
		if (rand() % 100 > 50)
			up = true;
		else
			down = true;
	}

	//test between the two left over
	if (right)
	{
		if (up)
		{
			//if right
			if (m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y + 1)] > m_GridStats.m_tileUnits[ConvBackX(_x + 1)][ConvBackY(_y)])
			{
				up = false;
			}
			else if (m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y + 1)] < m_GridStats.m_tileUnits[ConvBackX(_x + 1)][ConvBackY(_y)])
			{
				right = false;
			}
			else //if equal
			{
				if (rand() % 100 > 50)
					up = false;
				else
					right = false;
			}
		}
		else if (down)
		{
			//if right
			if (m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y - 1)] > m_GridStats.m_tileUnits[ConvBackX(_x + 1)][ConvBackY(_y)])
			{
				down = false;
			}
			else if (m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y - 1)] < m_GridStats.m_tileUnits[ConvBackX(_x + 1)][ConvBackY(_y)])
			{
				right = false;
			}
			else //if equal
			{
				if (rand() % 100 > 50)
					right = false;
				else
					down = false;
			}
		}
	}
	else if (left)
	{
		if (up)
		{
			//if right
			if (m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y + 1)] > m_GridStats.m_tileUnits[ConvBackX(_x - 1)][ConvBackY(_y)])
			{
				up = false;
			}
			else if (m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y + 1)] < m_GridStats.m_tileUnits[ConvBackX(_x - 1)][ConvBackY(_y)])
			{
				left = false;
			}
			else //if equal
			{
				if (rand() % 100 > 50)
					left = false;
				else
					up = false;
			}
		}
		else if (down)
		{
			//if right
			if (m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y - 1)] > m_GridStats.m_tileUnits[ConvBackX(_x - 1)][ConvBackY(_y)])
			{
				down = false;
			}
			else if (m_GridStats.m_tileUnits[ConvBackX(_x)][ConvBackY(_y - 1)] < m_GridStats.m_tileUnits[ConvBackX(_x - 1)][ConvBackY(_y)])
			{
				left = false;
			}
			else //if equal
			{
				if (rand() % 100 > 50)
					down = false;
				else
					left = false;
			}
		}
	}

#pragma endregion 

	//return now that we know which one to prioritise
	if (up)
	{
		coordinates = D3DXVECTOR2(0, 1);
	}
	else if (down)
	{
		coordinates = D3DXVECTOR2(0, -1);
	}
	else if (right)
	{
		coordinates = D3DXVECTOR2(1, 0);
	}
	else if (left)
	{
		coordinates = D3DXVECTOR2(-1, 0);
	}

	return coordinates;
}

//void FluidGrid::SendToCache(GridUnit* gu, D3DXVECTOR2 pos)
//{
//	if (!m_GridStats.m_addedToCache[(int)pos.x][(int)pos.y])
//	{
//		m_updateCache.push_back(std::make_pair(gu, pos));
//		m_GridStats.m_addedToCache[(int)pos.x][(int)pos.y] = true;
//	}
//}

void FluidGrid::SendToCache(GridUnit gu, D3DXVECTOR2 pos)
{
	if (!m_GridStats.m_addedToCache[(int)pos.x][(int)pos.y])
	{
		for (int i = (int)pos.y - 2; i < (int)pos.y + 3; ++i)
		{
			for (int j = (int)pos.x - 2; j < (int)pos.x + 3; ++j)
			{
				//Make sure it only draws colours within screen
				if (j > 0 && j < 1600 && i > 0 && i < 853)
				{
					//if (m_PrevGrid[At(j, i)].r != 0 && m_PrevGrid[At(j, i)].g != 0 && m_PrevGrid[At(j, i)].b != 0 && m_PrevGrid[At(j, i)].a == 1)
					if (!CanMoveTo(m_ColorGrid[At(j, i)], m_ColorGrid[At((int)pos.x, (int)pos.y)]))
					{
						//Do nothing if the colorgrid pixel is black
					}
					else
					{
						m_ColorGrid[At(j, i)].r = gu.r;
						m_ColorGrid[At(j, i)].g = gu.g;
						m_ColorGrid[At(j, i)].b = gu.b;
						m_ColorGrid[At(j, i)].a = gu.a;
					}

					//Calculates new scores
					Scores(gu, j, i);
				}
			}
		}

		if (poolPos >= 19999)
		{
			poolPos = 0;
		}

		updateCache[poolPos] = (std::make_pair(gu, pos));
		m_GridStats.m_addedToCache[(int)pos.x][(int)pos.y] = true;

		activeCache[activeSize] = poolPos;

		poolPos++;
		activeSize++;
	}
}

void FluidGrid::RemoveFromCache(int i, D3DXVECTOR2 pos)
{
	if (m_ColorGrid[At((int)pos.x, (int)pos.y)].r == 0.0f && m_ColorGrid[At((int)pos.x, (int)pos.y)].g == 0.0f && m_ColorGrid[At((int)pos.x, (int)pos.y)].b == 0.0f)
	{
		if (deletionPos >= 19999)
		{
			deletionPos = 0;
		}

		D3DXVECTOR2 d = D3DXVECTOR2((int)pos.x, (int)pos.y);
		flaggedForDeletion[deletionPos] = (std::make_pair(d, Utility::RandomFloat(1.5f, 3.5f)));
		deletionCache[deletionSize] = deletionPos;

		deletionPos++;
		deletionSize++;
	}

	m_GridStats.m_addedToCache[(int)pos.x][(int)pos.y] = false;

	int temp = activeCache[activeSize - 1];

	activeCache[i] = temp;

	activeSize--;
	//m_updateCache.erase(m_updateCache.begin() + i);

}

void FluidGrid::Scores(GridUnit gu, int j, int i)
{
	//Add to percentage
	float addNum = (1.0f / (m_arenaPaint.m_whiteSpace / 100.0f)) * 100.0f;

	//Checks that a colour isn't already in this pixel then adds to the score
	if (!m_ScoreCounts[At(j, i)].player1Colour && !m_ScoreCounts[At(j, i)].player2Colour && !m_ScoreCounts[At(j, i)].player3Colour && !m_ScoreCounts[At(j, i)].player4Colour)
	{
		if (gu.r == 1 && gu.g == 0 && gu.b == 0 && gu.a == 1)
		{
			playerScores[0] += addNum;
			m_ScoreCounts[At(j, i)].player1Colour = true;
		}
		//Pink
		if (gu.r == 1 && gu.g == 0.4f && gu.b == 0.7f && gu.a == 1)
		{
			playerScores[1] += addNum;
			m_ScoreCounts[At(j, i)].player2Colour = true;
		}
		//Blue
		if (gu.r == 0 && gu.g == 1 && gu.b == 1 && gu.a == 1)
		{
			playerScores[2] += addNum;
			m_ScoreCounts[At(j, i)].player3Colour = true;
		}
		//Green
		if (gu.r == 0 && gu.g == 0.7f && gu.b == 0 && gu.a == 1)
		{
			playerScores[3] += addNum;
			m_ScoreCounts[At(j, i)].player4Colour = true;
		}
	}
	//if there is already a pixel with a colour, it removes that colour pixel score and replaces with new pixel score
	else
	{
		if (m_ScoreCounts[At(j, i)].player1Colour)
		{
			m_ScoreCounts[At(j, i)].player1Colour = false;
			if (playerScores[0] > 0)
				playerScores[0] -= addNum;
		}
		if (m_ScoreCounts[At(j, i)].player2Colour)
		{
			m_ScoreCounts[At(j, i)].player2Colour = false;
			if (playerScores[1] > 0)
				playerScores[1] -= addNum;
		}
		if (m_ScoreCounts[At(j, i)].player3Colour)
		{
			m_ScoreCounts[At(j, i)].player3Colour = false;
			if (playerScores[2] > 0)
				playerScores[2] -= addNum;
		}
		if (m_ScoreCounts[At(j, i)].player4Colour)
		{
			m_ScoreCounts[At(j, i)].player4Colour = false;
			if (playerScores[3] > 0)
				playerScores[3] -= addNum;
		}

		//Now adds new pixel score on
		if (gu.r == 1 && gu.g == 0 && gu.b == 0 && gu.a == 1)
		{
			m_ScoreCounts[At(j, i)].player1Colour = true;
			playerScores[0] += addNum;
		}
		//Pink
		if (gu.r == 1 && gu.g == 0.4f && gu.b == 0.7f && gu.a == 1)
		{
			m_ScoreCounts[At(j, i)].player2Colour = true;
			playerScores[1] += addNum;
		}
		//Blue
		if (gu.r == 0 && gu.g == 1 && gu.b == 1 && gu.a == 1)
		{
			m_ScoreCounts[At(j, i)].player3Colour = true;
			playerScores[2] += addNum;
		}
		//Green
		if (gu.r == 0 && gu.g == 0.7f && gu.b == 0 && gu.a == 1)
		{
			m_ScoreCounts[At(j, i)].player4Colour = true;
			playerScores[3] += addNum;
		}
	}
}