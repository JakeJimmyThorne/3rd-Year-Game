#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <D3DX11.h>
#include <D3DX10math.h>

#include "../settings.h"

#include "textureshader.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	//pointers to world, view and proj matrix in gamemanager
	bool Initialise(ID3D11DeviceContext*,ID3D11Device*, HWND, D3DXMATRIX*,D3DXMATRIX*,D3DXMATRIX*);
	void Shutdown();
	bool Frame();

	//render without any shaders
	void Render(ID3D11DeviceContext*,int,ID3D11Buffer*,ID3D11Buffer*,unsigned int);

	//render the texture
	bool RenderTexture(ID3D11DeviceContext*, int, ID3D11ShaderResourceView*,D3DXMATRIX*,D3DXMATRIX*, float);

private:
	TextureShader m_TextureShader; 
	D3DXMATRIX *m_worldMatrix, *m_viewMatrix, *m_projectionMatrix;
};

#endif