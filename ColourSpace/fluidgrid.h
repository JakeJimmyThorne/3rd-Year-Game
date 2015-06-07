#ifndef _FLUIDGRID_H_
#define _FLUIDGRID_H_

#include <d3d11.h>
#include <d3dx11tex.h>
#include <d3dx10math.h>

#include <vector>
#include <iterator>
#include <iostream>
#include <utility>      // std::pair, std::make_pair
#include "utility.h"

const bool ENABLE_CACHING = true;
const float UPDATE_FPS = 60.0f;

const float UPDATE_COLLISION = 0.1f;

const int SPREAD_SPEED = 12;

class FluidGrid
{
public:
	FluidGrid();
	~FluidGrid();

	bool Initialise(int, int, ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
	void Reset();
	void Shutdown();

	void DrawPencil(int, int, D3DXVECTOR4);
	void LayPaint(int, int, D3DXVECTOR4, bool playerStuck = false);
	void LayBlack(int, int, D3DXVECTOR4);

	void ScoreCanvasCover()
	{
		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				if (m_ColorGrid[At(x, y)] == D3DXVECTOR4(1, 1, 1, 1) ||
					m_ColorGrid[At(x, y)] == D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1))
				  //m_ColorGrid[At(x, y)] == D3DXVECTOR4(1, 0, 1, 1)) If we want to get rid of the erased canvas as well
				{
					m_ColorGrid[At(x, y)] = D3DXVECTOR4(0, 0, 0, 1);
				}
			}
		}
	}

	ID3D11ShaderResourceView* GetTexture() { return m_arenaPaint.texture; }

	D3DXVECTOR4 QueryColor(int x, int y)
	{
		if( x > 0 && x < 1600 && y > 0 && y < 855)
			return m_ColorGrid[At(x, y)].VectorCast();
		else return D3DXVECTOR4(1, 1, 1, 1);
	}

	inline D3DXVECTOR2 ConvertToExpandedGrid(int x, int y)
	{
		int first = (1600 - x) / 3;
		int second = 533 - first;

		int third = (855 - y) / 3;
		int fourth = 285 - third;

		return D3DXVECTOR2((float)second, (float)fourth);
	}

	inline int ConvBackX(int x)
	{
		int convBackX = 1600 - ((1600 - (3 * x)) + 1);
		if (convBackX < 0)
		{
			convBackX = 0;
		}
		return convBackX;
	}

	inline int ConvBackY(int y)
	{
		int convBackY = 855 - ((855 - (3 * y)) + 1);
		if (convBackY < 0)
		{
			convBackY = 0;
		}
		return convBackY;
	}


	void Apply(ID3D11DeviceContext* dc)
	{
		m_arenaPaint.ApplyChanges(dc, m_ColorGrid);
	}

	void Update(float, ID3D11DeviceContext*);

private:

	struct GridUnit
	{
		float r, g, b, a;

		GridUnit() {}
		GridUnit(float _r, float _g, float _b, float _a)
		{
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}

		~GridUnit() {}

		inline void operator=(const GridUnit& other)
		{
			this->r = other.r;
			this->g = other.g;
			this->b = other.b;

			this->a = other.a;
		}

		D3DXVECTOR4 VectorCast()
		{
			return D3DXVECTOR4(r, g, b, a);
		}

		inline bool operator==(const GridUnit& other)
		{
			if (this->r == other.r &&
				this->g == other.g &&
				this->b == other.b)
			{
				return true;
			}
			else return false;
		}

		inline bool operator!=(const GridUnit& other)
		{
			if (this->r == other.r &&
				this->g == other.g &&
				this->b == other.b)
			{
				return false;
			}
			else return true;
		}

		inline void operator=(const D3DXVECTOR4& other)
		{
			if ((this->r == 1.0f && this->g == 0.0f && this->b == 0.0f) || //red
				(this->r == 1.0f && this->g == 0.4f && this->b == 0.7f) || //pink
				(this->r == 0.0f && this->g == 1.0f && this->b == 1.0f) || //blue
				(this->r == 0.0f && this->g == 1.0f && this->b == 0.0f))   //green
			{
				return;
			}

			this->r = other.x;
			this->g = other.y;
			this->b = other.z;

			this->a = other.w;
		}

		inline bool IsBlack()
		{
			if (this->r == 0 && this->g == 0 && this->b == 0)
				return true;
			else return false;
		}

		inline bool operator!=(const float other)
		{
			if (this->r != other || this->g != other || this->b != other)
				return false;
			else return true;
		}

		inline bool operator==(const D3DXVECTOR4& other)
		{
			if (this->r == other.x && this->g == other.y && this->b == other.z)
				return true;
			else return false;
		}
	};
	struct Texture
	{
		//get rid of unneeded variables
		ID3D11ShaderResourceView* texture;

		ID3D11Texture2D* writeToTex;
		ID3D11Texture2D* stagingTex;
		ID3D11Texture2D* imageTexture;

		ID3D11Resource* tex;

		unsigned int m_whiteSpace = 0;

		int height, width;

		bool LoadBackgroundImage(int w, int h, ID3D11Device* device, WCHAR* filename)
		{
			HRESULT result;

			width = w;
			height = h;

			D3DX11_IMAGE_LOAD_INFO loadInfo;
			ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
			loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			loadInfo.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			loadInfo.Height = h;
			loadInfo.Width = w;
			loadInfo.MipLevels = 1;

			result = D3DX11CreateShaderResourceViewFromFile(device, filename, &loadInfo, NULL, &texture, NULL);
			if (FAILED(result))
			{
				return false;
			}

			texture->GetResource(&tex);

			return true;
		}

		void CreateTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, GridUnit* grid)
		{
			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(textureDesc));

			textureDesc.Width = width;
			textureDesc.Height = height;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_STAGING;
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			textureDesc.BindFlags = 0;
			textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

			device->CreateTexture2D(&textureDesc, NULL, &stagingTex);
			device->CreateTexture2D(&textureDesc, NULL, &imageTexture);

			//Copies image data into the texture
			deviceContext->CopyResource(imageTexture, tex);

			float averageR = 0;
			float averageG = 0;
			float averageB = 0;

			D3D11_MAPPED_SUBRESOURCE mappedData;
			deviceContext->Map(imageTexture, 0, D3D11_MAP_READ, 0, &mappedData);

			GridUnit* obj;
			obj = (GridUnit *)mappedData.pData;


			//Get image average colour values
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					averageR += obj[y*width + x].r;
					averageG += obj[y*width + x].g;
					averageB += obj[y*width + x].b;
				}
			}

			averageR = averageR / (width * height);
			averageG = averageG / (width * height);
			averageB = averageB / (width * height);

			//Write new colour values into texture depending on value of pixels
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					if (obj[y*width + x].r < averageR - 0.1f && obj[y*width + x].g - 0.1f < averageG && obj[y*width + x].b - 0.1f < averageB)
					{
						obj[y*width + x].r = 0.5f;
						obj[y*width + x].g = 0.5f;
						obj[y*width + x].b = 0.5f;
						obj[y*width + x].a = 1.0f;
					}
					else
					{
						obj[y*width + x].r = 1.0f;
						obj[y*width + x].g = 1.0f;
						obj[y*width + x].b = 1.0f;
						obj[y*width + x].a = 1.0f;

						m_whiteSpace++;
					}

					//Write new colour value into the grid to be drawn
					grid[y*width + x].r = obj[y*width + x].r;
					grid[y*width + x].g = obj[y*width + x].g;
					grid[y*width + x].b = obj[y*width + x].b;
					grid[y*width + x].a = obj[y*width + x].a;
				}
			}

			deviceContext->Unmap(imageTexture, 0);

			//Texture that is dynamic and can be written to
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;

			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.CPUAccessFlags = 0;

			desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			desc.MiscFlags = 0;

			device->CreateTexture2D(&desc, NULL, &writeToTex);

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			device->CreateShaderResourceView(writeToTex, &srvDesc, &texture);

			return;
		}
		void ApplyChanges(ID3D11DeviceContext* dc, GridUnit* grid)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			dc->Map(stagingTex, 0, D3D11_MAP_READ_WRITE, 0, &mappedResource);
			memcpy(mappedResource.pData, grid, width * height * sizeof(GridUnit));
			dc->Unmap(stagingTex, 0);

			dc->CopyResource(writeToTex, stagingTex);
		}
	};
	struct RandomiseCheck
	{
		RandomiseCheck() {}
		~RandomiseCheck() {}

		int m_order[4];

		void Randomise()
		{
			bool one = false, two = false, three = false, zero = false;
			for (int i = 0; i < 4; i++)
			{
				int random = -1;
				bool found = false;

				do
				{
					random = rand() % 4;

					switch (random)
					{
					case 0:
						if (!zero)
						{
							zero = true;
							found = true;
						}
						break;
					case 1:
						if (!one)
						{
							one = true;
							found = true;
						}
						break;
					case 2:
						if (!two)
						{
							two = true;
							found = true;
						}
						break;
					case 3:
						if (!three)
						{
							three = true;
							found = true;
						}
						break;
					}

				} while (!found);

				m_order[i] = random;
			}


		}
	};
	struct GridStats
	{
		//to test whether the tile has recently been changed. Stops it from continuously spreading
		//every tick
		bool* m_tileChanged = 0;
		bool* m_tileChangedOriginal = 0;

		//optimisation to not check for colour if outside of the width or height of drawing
		int minX = 0, maxX = 1, minY = 0, maxY = 1;

		//how many units of paint are in every grid space. this determines when the paint
		//should stop spreading.
		int** m_tileUnits = 0;

		//we don't want paint to spread over pencil
		bool** m_isPencil = 0;

		//Used to check where we've sent gridunits to the cache
		//we don't want to be adding a point multiple times to the cache
		bool** m_addedToCache = 0;

		void Initialise(int w, int h)
		{
			/*m_tileChanged = new bool*[w];
			for (int i = 0; i < w; i++)
			m_tileChanged[i] = new bool[h];
			m_tileChangedOriginal = new bool*[w];
			for (int i = 0; i < w; i++)
			m_tileChangedOriginal[i] = new bool[h];*/

			if (!m_tileChanged)
				m_tileChanged = new bool[w*h];

			if (!m_tileChangedOriginal)
				m_tileChangedOriginal = new bool[w*h];

			for (int i = 0; i < w*h; i++)
			{
				m_tileChanged[i] = false;
				m_tileChangedOriginal[i] = false;
			}

			if (!m_isPencil)
			{
				m_isPencil = new bool*[w];
				for (int i = 0; i < w; i++)
					m_isPencil[i] = new bool[h];
			}

			if (!m_addedToCache)
			{
				m_addedToCache = new bool*[w];
				for (int i = 0; i < w; i++)
					m_addedToCache[i] = new bool[h];
			}

			if (!m_tileUnits)
			{
				m_tileUnits = new int*[w];
				for (int i = 0; i < w; i++)
					m_tileUnits[i] = new int[h];
			}

			for (int y = 0; y < h; y++)
			{
				for (int x = 0; x < w; x++)
				{
					m_addedToCache[x][y] = false;
					m_isPencil[x][y] = false;
					m_tileUnits[x][y] = 0;
				}
			}
		}
		inline void Reset(int w, int h)
		{
			memcpy(m_tileChanged, m_tileChangedOriginal, sizeof(bool)* w * h);
		}
	};

	inline bool HasColor(GridUnit unit)
	{
		if ((unit.r == 1.0f && unit.g == 0.0f && unit.b == 0.0f) || //red
			(unit.r == 1.0f && unit.g == 0.4f && unit.b == 0.7f) || //pink
			(unit.r == 0.0f && unit.g == 1.0f && unit.b == 1.0f) || //blue
			(unit.r == 0.0f && unit.g == 0.7f && unit.b == 0.0f) || //green
			(unit.r == 0.0f && unit.g == 0.0f && unit.b == 0.0f) || //black
			(unit.r == 1.0f && unit.g == 0.0f && unit.b == 1.0f))   //purple
		{
			return true;
		}
		else return false;
	}

	inline bool HasOtherColor(GridUnit unit, D3DXVECTOR4 color)
	{
		if ((unit.r == 1.0f && unit.g == 0.0f && unit.b == 0.0f) || //red
			(unit.r == 1.0f && unit.g == 0.4f && unit.b == 0.7f) || //pink
			(unit.r == 0.0f && unit.g == 1.0f && unit.b == 1.0f) || //blue
			(unit.r == 0.0f && unit.g == 0.7f && unit.b == 0.0f) || //green
			(unit.r == 0.0f && unit.g == 0.0f && unit.b == 0.0f) || //black
			(unit.r == 1.0f && unit.g == 0.0f && unit.b == 1.0f))   //purple
		{
			if (unit.r == color.x && unit.g == color.y && unit.b == color.z)
				return false;

			return true;
		}
		else return false;
	}

	//Check that the color the fluid is moving to can infact be moved to
	bool CanMoveTo(GridUnit unit, GridUnit current)
	{
		//Don't let the color spread on black or the gray render box
		if ((unit.r == 0.0f && unit.g == 0.0f && unit.b == 0.0f) ||
			(unit.r == 0.5f && unit.g == 0.5f && unit.b == 0.5f) ||
			(unit.r == 1.0f && unit.g == 0.0f && unit.b == 1.0f))
		{
			if (current.r != 0.0f || current.g != 0.0f || current.b != 0.0f)
			{
				return false;
			}
		}
		
		return true;
	}

	inline bool InBounds(int x, int y)
	{
		if (x > 0 && x < m_width &&
			y > 0 && y < m_height)
		{
			return true;
		}
		else return false;
	}

	struct ScorePositions
	{
		bool player1Colour;
		bool player2Colour;
		bool player3Colour;
		bool player4Colour;
	};

	//Spreads the fluid to new pixels
	bool TestForEmpty(int, int);

	//Tests collision against other colors to see what happens
	bool TestForCollision(int, int);

	//Spreads density among its peers
	bool TestForSpread(int, int);

	bool TestSameColor(int, int, int, int);

	void SendToCache(GridUnit, D3DXVECTOR2);
	void RemoveFromCache(int, D3DXVECTOR2);

	//Calculates scores
	void Scores(GridUnit, int, int);

	//little helper function for easy 2D -> 1D array access
	inline int At(int x, int y)
	{
		return y*m_width + x;
	}

	


	//test to see which tile has the least units for spreading paint
	D3DXVECTOR2 TestForLeastUnits(int, int);

public:
	GridUnit* GetGrid() { return m_ColorGrid; }

	float playerScores[4];

private:

	int headPos;
	int activeSize;

	Texture m_arenaPaint;

	float m_testCollisionTimer;

	//Higher density means more units of the fluid can be stored in one pixel space.
	//The bigger the number the less the fluid can spread out
	float m_density;

	//thickness, how fast the fluid will travel every tick. the bigger the number the 
	//slower the fluid spreads
	float m_viscosity;

	//1D array of actual color values to copy to the texture
	GridUnit* m_ColorGrid;

	//backup of original m_ColorGrid
	GridUnit* m_OriginalGrid;

	ScorePositions* m_ScoreCounts;

	//Stats for each gridunit
	//test if it is pencil, if it has changed and the unit amount
	GridStats m_GridStats;
	int m_width, m_height;


	float m_fluidTimer;

	//cache of current pixel points that need updating
	//Pointer to the gridunit, and its position in the texture
	//std::vector<std::pair<GridUnit*, D3DXVECTOR2>> m_updateCache;

	std::vector<std::pair<GridUnit, D3DXVECTOR2>> updateCache;
	std::vector<int> activeCache;
	int poolPos;
	
	std::vector<std::pair<D3DXVECTOR2, float>> flaggedForDeletion;
	std::vector<int> deletionCache;
	int deletionPos;
	int deletionSize;
};

#endif