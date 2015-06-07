#include "score.h"

ID3D11ShaderResourceView* Score::m_texture0;
ID3D11ShaderResourceView* Score::m_texture1;
ID3D11ShaderResourceView* Score::m_texture2;
ID3D11ShaderResourceView* Score::m_texture3;
ID3D11ShaderResourceView* Score::m_texture4;
ID3D11ShaderResourceView* Score::m_texture5;
ID3D11ShaderResourceView* Score::m_texture6;
ID3D11ShaderResourceView* Score::m_texture7;
ID3D11ShaderResourceView* Score::m_texture8;
ID3D11ShaderResourceView* Score::m_texture9;

ID3D11ShaderResourceView* Score::m_greenTexture0;
ID3D11ShaderResourceView* Score::m_greenTexture1;
ID3D11ShaderResourceView* Score::m_greenTexture2;
ID3D11ShaderResourceView* Score::m_greenTexture3;
ID3D11ShaderResourceView* Score::m_greenTexture4;
ID3D11ShaderResourceView* Score::m_greenTexture5;
ID3D11ShaderResourceView* Score::m_greenTexture6;
ID3D11ShaderResourceView* Score::m_greenTexture7;
ID3D11ShaderResourceView* Score::m_greenTexture8;
ID3D11ShaderResourceView* Score::m_greenTexture9;

ID3D11ShaderResourceView* Score::m_blueTexture0;
ID3D11ShaderResourceView* Score::m_blueTexture1;
ID3D11ShaderResourceView* Score::m_blueTexture2;
ID3D11ShaderResourceView* Score::m_blueTexture3;
ID3D11ShaderResourceView* Score::m_blueTexture4;
ID3D11ShaderResourceView* Score::m_blueTexture5;
ID3D11ShaderResourceView* Score::m_blueTexture6;
ID3D11ShaderResourceView* Score::m_blueTexture7;
ID3D11ShaderResourceView* Score::m_blueTexture8;
ID3D11ShaderResourceView* Score::m_blueTexture9;

ID3D11ShaderResourceView* Score::m_pinkTexture0;
ID3D11ShaderResourceView* Score::m_pinkTexture1;
ID3D11ShaderResourceView* Score::m_pinkTexture2;
ID3D11ShaderResourceView* Score::m_pinkTexture3;
ID3D11ShaderResourceView* Score::m_pinkTexture4;
ID3D11ShaderResourceView* Score::m_pinkTexture5;
ID3D11ShaderResourceView* Score::m_pinkTexture6;
ID3D11ShaderResourceView* Score::m_pinkTexture7;
ID3D11ShaderResourceView* Score::m_pinkTexture8;
ID3D11ShaderResourceView* Score::m_pinkTexture9;

bool Score::m_texturesLoaded;

bool Score::LoadNumberTextures(ID3D11Device* device)
{
	HRESULT hr;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//RED TEXTURES
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
#if _DEBUG
	//WCHAR* filepath = L"../ColourSpace/data/GFX/0.png";
	WCHAR* filepath = L"../ColourSpace/data/GFX/Scores/Red/0.png";
#else
	WCHAR* filepath = L"data/GFX/Scores/Red/0.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_texture0, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/1.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Red/1.png";
#else
	filepath = L"data/GFX/Scores/Red/1.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_texture1, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/2.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Red/2.png";
#else
	filepath = L"data/GFX/Scores/Red/2.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_texture2, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/3.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Red/3.png";
#else
	filepath = L"data/GFX/Scores/Red/3.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_texture3, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/4.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Red/4.png";
#else
	filepath = L"data/GFX/Scores/Red/4.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_texture4, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/5.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Red/5.png";
#else
	filepath = L"data/GFX/Scores/Red/5.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_texture5, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/6.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Red/6.png";
#else
	filepath = L"data/GFX/Scores/Red/6.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_texture6, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/7.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Red/7.png";
#else
	filepath = L"data/GFX/Scores/Red/7.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_texture7, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/8.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Red/8.png";
#else
	filepath = L"data/GFX/Scores/Red/8.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_texture8, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/9.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Red/9.png";
#else
	filepath = L"data/GFX/Scores/Red/9.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_texture9, NULL);
	if (FAILED(hr))
	{
		return false;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PINK TEXTURES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if _DEBUG
	//WCHAR* filepath = L"../ColourSpace/data/GFX/0.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Pink/0.png";
#else
	filepath = L"data/GFX/Scores/Pink/0.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_pinkTexture0, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/1.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Pink/1.png";
#else
	filepath = L"data/GFX/Scores/Pink/1.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_pinkTexture1, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/2.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Pink/2.png";
#else
	filepath = L"data/GFX/Scores/Pink/2.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_pinkTexture2, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/3.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Pink/3.png";
#else
	filepath = L"data/GFX/Scores/Pink/3.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_pinkTexture3, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/4.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Pink/4.png";
#else
	filepath = L"data/GFX/Scores/Pink/4.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_pinkTexture4, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/5.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Pink/5.png";
#else
	filepath = L"data/GFX/Scores/Pink/5.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_pinkTexture5, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/6.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Pink/6.png";
#else
	filepath = L"data/GFX/Scores/Pink/6.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_pinkTexture6, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/7.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Pink/7.png";
#else
	filepath = L"data/GFX/Scores/Pink/7.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_pinkTexture7, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/8.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Pink/8.png";
#else
	filepath = L"data/GFX/Scores/Pink/8.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_pinkTexture8, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/9.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Pink/9.png";
#else
	filepath = L"data/GFX/Scores/Pink/9.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_pinkTexture9, NULL);
	if (FAILED(hr))
	{
		return false;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BLUE TEXTURES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if _DEBUG
	//WCHAR* filepath = L"../ColourSpace/data/GFX/0.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Blue/0.png";
#else
	filepath = L"data/GFX/Scores/Blue/0.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_blueTexture0, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/1.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Blue/1.png";
#else
	filepath = L"data/GFX/Scores/Blue/1.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_blueTexture1, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/2.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Blue/2.png";
#else
	filepath = L"data/GFX/Scores/Blue/2.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_blueTexture2, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/3.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Blue/3.png";
#else
	filepath = L"data/GFX/Scores/Blue/3.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_blueTexture3, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/4.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Blue/4.png";
#else
	filepath = L"data/GFX/Scores/Blue/4.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_blueTexture4, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/5.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Blue/5.png";
#else
	filepath = L"data/GFX/Scores/Blue/5.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_blueTexture5, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/6.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Blue/6.png";
#else
	filepath = L"data/GFX/Scores/Blue/6.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_blueTexture6, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/7.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Blue/7.png";
#else
	filepath = L"data/GFX/Scores/Blue/7.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_blueTexture7, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/8.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Blue/8.png";
#else
	filepath = L"data/GFX/Scores/Blue/8.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_blueTexture8, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/9.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Blue/9.png";
#else
	filepath = L"data/GFX/Scores/Blue/9.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_blueTexture9, NULL);
	if (FAILED(hr))
	{
		return false;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GREEN TEXTURES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if _DEBUG
	//WCHAR* filepath = L"../ColourSpace/data/GFX/0.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Green/0.png";
#else
	filepath = L"data/GFX/Scores/Green/0.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_greenTexture0, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/1.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Green/1.png";
#else
	filepath = L"data/GFX/Scores/Green/1.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_greenTexture1, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/2.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Green/2.png";
#else
	filepath = L"data/GFX/Scores/Green/2.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_greenTexture2, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/3.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Green/3.png";
#else
	filepath = L"data/GFX/Scores/Green/3.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_greenTexture3, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/4.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Green/4.png";
#else
	filepath = L"data/GFX/Scores/Green/4.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_greenTexture4, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/5.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Green/5.png";
#else
	filepath = L"data/GFX/Scores/Green/5.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_greenTexture5, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/6.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Green/6.png";
#else
	filepath = L"data/GFX/Scores/Green/6.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_greenTexture6, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/7.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Green/7.png";
#else
	filepath = L"data/GFX/Scores/Green/7.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_greenTexture7, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/8.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Green/8.png";
#else
	filepath = L"data/GFX/Scores/Green/8.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_greenTexture8, NULL);
	if (FAILED(hr))
	{
		return false;
	}

#if _DEBUG
	//filepath = L"../ColourSpace/data/GFX/9.png";
	filepath = L"../ColourSpace/data/GFX/Scores/Green/9.png";
#else
	filepath = L"data/GFX/Scores/Green/9.png";
#endif

	hr = D3DX11CreateShaderResourceViewFromFile(device, filepath, NULL, NULL, &m_greenTexture9, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}


bool Score::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, float bx, float by, float ex, float ey, WCHAR* filename)
{
	bool result;

	for (int i = 0; i < SCORE_LENGTH; i++)
	{
		m_score[i] = 0;
	}

	//POSSIBLY NEEDS MORE WORK
	for (int i = 0; i < SCORE_LENGTH; i++)
	{
		if (i < SCORE_LENGTH - 2)
		{
			result = m_Numbers[i].Initialise(0, 0, device, deviceContext, screenWidth, screenHeight, bx + i * 42, by, ex + i * 42, ey, filename);
		}
		else if (i >= SCORE_LENGTH - 1)
		{
			result = m_Numbers[i].Initialise(0, 0, device, deviceContext, screenWidth, screenHeight, bx + i * 45, by, ex + i * 45, ey, filename);
		}
		else
		{
			result = m_Numbers[i].Initialise(0, 0, device, deviceContext, screenWidth, screenHeight, bx + i * 47, by, ex + i * 47, ey, filename);
		}


		if (!result)
			return false;
	}

	m_totalScore = 0;
	m_runningScore = 0;

	return true;
}

void Score::Render(ID3D11DeviceContext* dc, D3DXMATRIX* viewMatrix, D3DXMATRIX* orthoMatirx, Renderer* renderer)
{
	for (int i = 0; i < SCORE_LENGTH; i++)
	{
		m_Numbers[i].Update(dc);
		renderer->Render(dc, m_Numbers[i].GetIndexCount(), m_Numbers[i].GetVertexBuffer(), m_Numbers[i].GetIndexBuffer(), m_Numbers[i].GetSizeOf());
		renderer->RenderTexture(dc, m_Numbers[i].GetIndexCount(), m_Numbers[i].AccessTexture(), viewMatrix, orthoMatirx, 1);
	}

	return;
}

void Score::AddScore(int amount)
{
	//Make sure we don't go above the maximum values
	int nineCount = 0;

	for (int i = 0; i < SCORE_LENGTH; i++)
	{
		if (m_score[i] == 9)
			nineCount++;
	}

	if (nineCount == SCORE_LENGTH)
		return;

	m_runningScore += amount;

	int tempScr[SCORE_LENGTH];
	int tempNumber = amount;

	//separate the amount to add into an array that matches m_score
	for (int i = 1; i <= SCORE_LENGTH; i++) 
	{
		int index = SCORE_LENGTH - i;
		int digit = tempNumber / Power(10, index);

		tempScr[i - 1] = digit;
		tempNumber = tempNumber - Power(10, index) * digit;
	}

	//add them together
	for (int i = 0; i < SCORE_LENGTH; i++)
	{
		m_score[i] += tempScr[i];
	}

	bool calculating = true;

	//Make sure everything is properly rounded up
	while (calculating)
	{
		int changed = 0;

		for (int i = 0; i < SCORE_LENGTH; i++)
		{
			if (m_score[i] >= 10)
			{
				int remainder = m_score[i] - 10;
				m_score[i] = remainder;
				m_score[i - 1]++;
				changed++;
			}
		}

		if (changed == 0)
		{
			calculating = false;
		}
	}

	//update textures
	for (int i = 0; i < SCORE_LENGTH; i++)
	{
		switch (m_score[i])
		{
		case 0:
			m_Numbers[i].SetUITexture(m_texture0);
			break;
		case 1:
			m_Numbers[i].SetUITexture(m_texture1);
			break;
		case 2:
			m_Numbers[i].SetUITexture(m_texture2);
			break;
		case 3:
			m_Numbers[i].SetUITexture(m_texture3);
			break;
		case 4:
			m_Numbers[i].SetUITexture(m_texture4);
			break;
		case 5:
			m_Numbers[i].SetUITexture(m_texture5);
			break;
		case 6:
			m_Numbers[i].SetUITexture(m_texture6);
			break;
		case 7:
			m_Numbers[i].SetUITexture(m_texture7);
			break;
		case 8:
			m_Numbers[i].SetUITexture(m_texture8);
			break;
		case 9:
			m_Numbers[i].SetUITexture(m_texture9);
			break;

		}
	}
}
void Score::SetScore(int amount, int playerID)
{
	//Make sure we don't go above the maximum values
	int nineCount = 0;

	for (int i = 0; i < SCORE_LENGTH; i++)
	{
		if (m_score[i] == 9)
			nineCount++;
	}

	if (nineCount == SCORE_LENGTH)
		return;

	m_runningScore = amount;

	int tempScr[SCORE_LENGTH];
	int tempNumber = amount;

	//separate the amount to add into an array that matches m_score
	for (int i = 1; i <= SCORE_LENGTH; i++)
	{
		int index = SCORE_LENGTH - i;
		int digit = tempNumber / Power(10, index);

		tempScr[i - 1] = digit;
		tempNumber = tempNumber - Power(10, index) * digit;
	}

	//add them together
	for (int i = 0; i < SCORE_LENGTH; i++)
	{
		m_score[i] = tempScr[i];
	}

	//update textures
	for (int i = 0; i < SCORE_LENGTH; i++)
	{
		switch (m_score[i])
		{
		case 0:
			if (playerID == 0)
			{
				m_Numbers[i].SetUITexture(m_texture0);
			}
			else if (playerID == 1)
			{
				m_Numbers[i].SetUITexture(m_pinkTexture0);
			}
			else if (playerID == 2)
			{ 
				m_Numbers[i].SetUITexture(m_blueTexture0);
			}
			else if (playerID == 3)
			{ 
				m_Numbers[i].SetUITexture(m_greenTexture0);
			}
			break;
		case 1:
			if (playerID == 0)
			{
				m_Numbers[i].SetUITexture(m_texture1);
			}
			else if (playerID == 1)
			{
				m_Numbers[i].SetUITexture(m_pinkTexture1);
			}
			else if (playerID == 2)
			{
				m_Numbers[i].SetUITexture(m_blueTexture1);
			}
			else if (playerID == 3)
			{
				m_Numbers[i].SetUITexture(m_greenTexture1);
			}
			break;
		case 2:
			if (playerID == 0)
			{
				m_Numbers[i].SetUITexture(m_texture2);
			}
			else if (playerID == 1)
			{
				m_Numbers[i].SetUITexture(m_pinkTexture2);
			}
			else if (playerID == 2)
			{
				m_Numbers[i].SetUITexture(m_blueTexture2);
			}
			else if (playerID == 3)
			{
				m_Numbers[i].SetUITexture(m_greenTexture2);
			}
			break;
		case 3:
			if (playerID == 0)
			{
				m_Numbers[i].SetUITexture(m_texture3);
			}
			else if (playerID == 1)
			{
				m_Numbers[i].SetUITexture(m_pinkTexture3);
			}
			else if (playerID == 2)
			{
				m_Numbers[i].SetUITexture(m_blueTexture3);
			}
			else if (playerID == 3)
			{
				m_Numbers[i].SetUITexture(m_greenTexture3);
			}
			break;
		case 4:
			if (playerID == 0)
			{
				m_Numbers[i].SetUITexture(m_texture4);
			}
			else if (playerID == 1)
			{
				m_Numbers[i].SetUITexture(m_pinkTexture4);
			}
			else if (playerID == 2)
			{
				m_Numbers[i].SetUITexture(m_blueTexture4);
			}
			else if (playerID == 3)
			{
				m_Numbers[i].SetUITexture(m_greenTexture4);
			}
			break;
		case 5:
			if (playerID == 0)
			{
				m_Numbers[i].SetUITexture(m_texture5);
			}
			else if (playerID == 1)
			{
				m_Numbers[i].SetUITexture(m_pinkTexture5);
			}
			else if (playerID == 2)
			{
				m_Numbers[i].SetUITexture(m_blueTexture5);
			}
			else if (playerID == 3)
			{
				m_Numbers[i].SetUITexture(m_greenTexture5);
			}
			break;
		case 6:
			if (playerID == 0)
			{
				m_Numbers[i].SetUITexture(m_texture6);
			}
			else if (playerID == 1)
			{
				m_Numbers[i].SetUITexture(m_pinkTexture6);
			}
			else if (playerID == 2)
			{
				m_Numbers[i].SetUITexture(m_blueTexture6);
			}
			else if (playerID == 3)
			{
				m_Numbers[i].SetUITexture(m_greenTexture6);
			}
			break;
		case 7:
			if (playerID == 0)
			{
				m_Numbers[i].SetUITexture(m_texture7);
			}
			else if (playerID == 1)
			{
				m_Numbers[i].SetUITexture(m_pinkTexture7);
			}
			else if (playerID == 2)
			{
				m_Numbers[i].SetUITexture(m_blueTexture7);
			}
			else if (playerID == 3)
			{
				m_Numbers[i].SetUITexture(m_greenTexture7);
			}
			break;
		case 8:
			if (playerID == 0)
			{
				m_Numbers[i].SetUITexture(m_texture8);
			}
			else if (playerID == 1)
			{
				m_Numbers[i].SetUITexture(m_pinkTexture8);
			}
			else if (playerID == 2)
			{
				m_Numbers[i].SetUITexture(m_blueTexture8);
			}
			else if (playerID == 3)
			{
				m_Numbers[i].SetUITexture(m_greenTexture8);
			}
			break;
		case 9:
			if (playerID == 0)
			{
				m_Numbers[i].SetUITexture(m_texture9);
			}
			else if (playerID == 1)
			{
				m_Numbers[i].SetUITexture(m_pinkTexture9);
			}
			else if (playerID == 2)
			{
				m_Numbers[i].SetUITexture(m_blueTexture9);
			}
			else if (playerID == 3)
			{
				m_Numbers[i].SetUITexture(m_greenTexture9);
			}
			break;

		}
	}
}

int Score::Power(int number, int index) 
{
	if (index == 0) 
	{
		return 1;
	}

	int num = number;
	for (int i = 1; i < index; i++) 
	{
		number = number * num;
	}
	return number;
}