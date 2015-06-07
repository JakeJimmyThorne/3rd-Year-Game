#ifndef _UIOBJECT_H_
#define _UIOBJECT_H_

#include <d3d11.h>
#include <d3dx10math.h>

#include "gameobject.h"

class UIObject : public GameObject
{
public:
	UIObject();
	~UIObject();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, int, int, WCHAR*);
	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, int, int, float, float, float, float, WCHAR*);

	void Shutdown();

	bool Update(ID3D11DeviceContext* dc) { return GetQuad()->Update(dc); }

	//texture getters
	ID3D11ShaderResourceView* GetUITexture() { return GetTexture()->GetTexture(); }
	void SetUITexture(ID3D11ShaderResourceView* newTex) { GetTexture()->SetTexture(newTex); }

	//quad getters
	int GetIndexCount()    { return GetQuad()->GetIndexCount(); }
	unsigned int GetSize() { return GetQuad()->GetSize(); }

	ID3D11Buffer* GetVertexBuffer() { return GetQuad()->GetVertexBuffer(); }
	ID3D11Buffer* GetIndexBuffer()  { return GetQuad()->GetIndexBuffer(); }

	void SetActive(bool b) { m_active = b; }
	bool IsActive() { return m_active; }

private:
	bool m_active;
};

#endif