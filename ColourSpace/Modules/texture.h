#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3d11.h>
#include <d3dx11tex.h>
#include <d3dx10math.h>

#include <vector>
#include <iterator>


const float TRAIL_FADE = 1;

class Texture
{
public:
	Texture();
	~Texture();

	bool Initialise(int, int, ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void Shutdown();


	ID3D11ShaderResourceView* GetTexture();

	void SetTexture(ID3D11ShaderResourceView*);

	void ResetTexture(ID3D11DeviceContext*, int);



	ID3D11Texture2D* GetDynamicTexture() { return m_dynamicTex2D; }


private:
	//Texture used on the object
	ID3D11ShaderResourceView* m_texture;
	ID3D11UnorderedAccessView* m_writeToTexBlackWhite;
	ID3D11UnorderedAccessView* m_writeToTexGreyscale;
	ID3D11UnorderedAccessView* m_writeToTexColour;

	ID3D11ComputeShader* m_initialiseTexCompute;

	//Textures used to alter main texture pixel colours
	ID3D11Texture2D* m_texBlackWhite;
	ID3D11Texture2D* m_texGreyscale;
	ID3D11Texture2D* m_texColour;

	std::vector<ID3D11Texture2D*> writeToTextures;

	ID3D11Texture2D* m_dynamicTex2D;
	ID3D11Texture2D* m_tempTexture;
	ID3D11Texture2D* m_collisionTexture;

	int m_width, m_height;
};

#endif