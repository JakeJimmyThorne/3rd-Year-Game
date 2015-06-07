#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include <vector>
using namespace std;

class Model
{
public:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 texture;
	};

	struct ModelType
	{
		float x, y, z;
		float ux, uy;
		float nx, ny, nz;
	};

public:
	Model();
	~Model();

	bool Initialise(ID3D11Device*, HWND, const char*);
	void Shutdown();
	void Frame();
	void Render(ID3D11ShaderResourceView*,ID3D11ShaderResourceView*);

	int GetIndexCount() { return m_indexCount; }

	unsigned int GetSize() { return sizeof(VertexType); }

	ID3D11Buffer* GetVertexBuffer() { return m_vertexBuffer; }
	ID3D11Buffer* GetIndexBuffer() { return m_indexBuffer; }

	D3DXVECTOR3 *BB_min, *BB_max;
	void ReleaseBB();

private:
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();

	bool LoadModel(const char*);
	void ReleaseModel();

	void GenerateBoundingBox();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	ModelType* model;

	

	

};

#endif