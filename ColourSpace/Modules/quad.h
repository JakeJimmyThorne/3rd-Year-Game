#ifndef _QUAD_H_
#define _QUAD_H_

#include <d3d11.h>
#include <d3dx10math.h>

class Quad
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

public:
	Quad();
	~Quad();

	bool Initialise(ID3D11Device*, int, int);
	bool Initialise(ID3D11Device*, int, int, float, float, float, float);

	void Shutdown();

	int GetIndexCount();

	ID3D11Buffer* GetVertexBuffer() { return m_vertexBuffer; }
	ID3D11Buffer* GetIndexBuffer()  { return m_indexBuffer; }

	unsigned int GetSize() { return sizeof(VertexType); }

	bool Update(ID3D11DeviceContext* dc) {
		return UpdateBuffers(dc); 
	}

	void UpdatePos(D3DXVECTOR2 newPos) { m_position = newPos; }
	D3DXVECTOR2* GetPosition() { return &m_position; }

	void IncrementWidth(float incre) { m_width += incre; }
	void SetWidth(float set) { m_width = set; }
	float GetWidth() { return m_width; }


private:
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;

	int m_vertexCount, m_indexCount;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	float m_previousPosX, m_previousPosY;

	D3DXVECTOR2 m_position;

	//height and width of the quad
	float m_height, m_width;
	float m_previousWidth;

	//offset from the side and bottom of the screen
	float m_offsetX, m_offsetY;

	//do we want the quad to scale to the screen?
	bool m_scale;
};

#endif