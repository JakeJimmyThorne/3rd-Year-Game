#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include <d3d11.h>
#include <d3dx10math.h>

#include "gameobject.h"

class Object2D : public GameObject
{
public:
	Object2D();
	~Object2D();

	bool Initialise(int, int, ID3D11Device*, ID3D11DeviceContext*, int, int, WCHAR*);
	bool Initialise(int, int, ID3D11Device*, ID3D11DeviceContext*, int, int, float, float, float, float, WCHAR*);

	void Shutdown();

	bool Update(ID3D11DeviceContext* dc) { return GetQuad()->Update(dc); }

	//texture getters
	ID3D11ShaderResourceView* AccessTexture() { return GetTexture()->GetTexture(); }
	Texture* AccessTextureClass() { return GetTexture(); }
	void SetUITexture(ID3D11ShaderResourceView* newTex) { GetTexture()->SetTexture(newTex); }

	//quad getters
	int GetIndexCount()    { return GetQuad()->GetIndexCount(); }
	unsigned int GetSizeOf() { return GetQuad()->GetSize(); }

	ID3D11Buffer* GetVertexBuffer() { return GetQuad()->GetVertexBuffer(); }
	ID3D11Buffer* GetIndexBuffer()  { return GetQuad()->GetIndexBuffer(); }

	D3DXVECTOR2* GetSize() { return &m_size; }
	D3DXVECTOR2* GetPosition() { return &m_position; }

	

private:
	D3DXVECTOR2 m_size;
	D3DXVECTOR2 m_position;

	
};

#endif