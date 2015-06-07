#include "Quad.h"

const static float VIEWPORT_PERCENTAGE_X1 = 1 - (90.0f   /100);
const static float VIEWPORT_PERCENTAGE_Y1 = 1 - (90.0f   /100);

Quad::Quad()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

Quad::~Quad()
{
}


bool Quad::Initialise(ID3D11Device* device, int screenWidth, int screenHeight)
{
	bool result;

	m_offsetX = screenWidth * 0.01f;
	m_offsetY = screenHeight * 0.01f;
	m_scale = true;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//Initialise the previous rendering position to negative one.
	m_previousPosX = -1;
	m_previousPosY = -1;

	m_width = 100;
	m_height = 100;

	if(!m_scale)
	{
		m_position = D3DXVECTOR2(100,100);
	}
	else
	{
		m_position.x = screenWidth * VIEWPORT_PERCENTAGE_X1;
		m_position.y = screenHeight * VIEWPORT_PERCENTAGE_Y1;
	}

	result = InitialiseBuffers(device);
	if(!result)
	{
		return false;
	}

	return true;
}

bool Quad::Initialise(ID3D11Device* device, int screenWidth, int screenHeight, float bx, float by, float ex, float ey)
{
	bool result;

	m_offsetX = 0;
	m_offsetY = 0;
	m_scale = false;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//Initialise the previous rendering position to negative one.
	m_previousPosX = -1;
	m_previousPosY = -1;

	m_width = ex - bx;
	m_height = ey - by;

	if(!m_scale)
	{
		m_position = D3DXVECTOR2(bx,by);
	}
	else
	{
		m_position.x = screenWidth * VIEWPORT_PERCENTAGE_X1;
		m_position.y = screenHeight * VIEWPORT_PERCENTAGE_Y1;
	}

	result = InitialiseBuffers(device);
	if(!result)
	{
		return false;
	}

	return true;
}


void Quad::Shutdown()
{
	ShutdownBuffers();

	return;
}

int Quad::GetIndexCount()
{
	return m_indexCount;
}


bool Quad::InitialiseBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	m_vertexCount = 6;
	m_indexCount = m_vertexCount;

	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	//Initialise vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// Load the index array with data.
	for(i=0; i<m_indexCount; i++)
	{
		indices[i] = i;
	}

    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void Quad::ShutdownBuffers()
{
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


bool Quad::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
	//don't update if the quad is at the same position
	if ((m_position.x == m_previousPosX) && (m_position.y == m_previousPosY) && m_previousWidth == m_width)
	{
		return true;
	}

	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;
	
	// If it has changed then update the position it is being rendered to.
	m_previousPosX = m_position.x;
	m_previousPosY = m_position.y;
	m_previousWidth = m_width;

	//calculate the positions and quad sizes to fill up the screen according to screen size
	if(m_scale)
	{
		m_width = m_screenWidth - m_position.x - m_offsetX;
		m_height = m_screenHeight - m_position.y - m_offsetY;
	}

	// Calculate the screen coordinates of the left side of the bitmap.
	left = (float)((m_screenWidth / 2) * -1) + (float)m_position.x;

	// Calculate the screen coordinates of the right side of the bitmap.
	right = left + (float)m_width;

	// Calculate the screen coordinates of the top of the bitmap.
	top = (float)(m_screenHeight / 2) - (float)m_position.y;

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - (float)m_height;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Load the vertex array with data.
	// First triangle.
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = D3DXVECTOR2(1.0f, 1);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = D3DXVECTOR2(0.0f, 1);

	// Second triangle.
	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	vertices[4].texture = D3DXVECTOR2(1, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = D3DXVECTOR2(1, 1);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}
