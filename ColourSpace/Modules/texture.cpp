#include "Texture.h"

Texture::Texture()
{
	m_texture = 0;
}

Texture::~Texture()
{

}

bool Texture::LoadTexture(ID3D11Device* dev, WCHAR* filename)
{
	HRESULT result;

	result = D3DX11CreateShaderResourceViewFromFile(dev, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool Texture::Initialise(int xSize, int ySize, ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)
{
	HRESULT result;

	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return false;
	}

	m_width = xSize;
	m_height = ySize;

	return true;
}

void Texture::Shutdown()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}



ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_texture;
}

void Texture::SetTexture(ID3D11ShaderResourceView* newTex)
{
	if (newTex != m_texture)
		m_texture = newTex;

	return;
}


