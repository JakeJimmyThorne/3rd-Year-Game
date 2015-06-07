#include "model.h"

Model::Model()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	model = 0;

	BB_min = 0;
	BB_max = 0;
}

Model::~Model()
{
	
}



bool Model::Initialise(ID3D11Device* device, HWND hwnd, const char* modelFilename)
{
	bool result;

//	result = GameObject::Initialise(deviceContext,device,hwnd);
//	if(!result)
//		return false;

	result = LoadModel(modelFilename);
	if(!result)
		return false;

	result = InitialiseBuffers(device);
	if(!result)
		return false;

	//copy to the renderer component
	//renderer->SetVertices(m_vertexBuffer,m_vertexCount);
	//renderer->SetIndices(m_indexBuffer,m_indexCount);

	//initialise & update matrices
	//transform->InitialiseMatrices();
	//transform->UpdateMatrices();

	GenerateBoundingBox();
	ReleaseModel();

	return true;
}

void Model::Shutdown()
{
	//GameObject::Shutdown();
}

void Model::Frame()
{

	return;
}

void Model::Render(ID3D11ShaderResourceView* shadowMap, ID3D11ShaderResourceView* shadowMap2)
{
	//GameObject::Render(shadowMap,shadowMap2);
	return;
}


bool Model::InitialiseBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}
	int v = 0;

	// Load the vertex array and index array with data.
	for(i=0; i<m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(model[i].x, model[i].y, model[i].z);
		vertices[i].color = D3DXVECTOR4(0.7f, 0.7f, 0.7f, 1.0f);
		vertices[i].normal = D3DXVECTOR3(model[i].nx, model[i].ny, model[i].nz);
		vertices[i].texture = D3DXVECTOR2(model[i].ux, model[i].uy);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void Model::ShutdownBuffers()
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


bool Model::LoadModel(const char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);
	
	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	model = new ModelType[m_vertexCount];
	if(!model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	//dummy variables to take in the uv coordinates
	float ux, uy;
	ux = uy = 0;
	// Read in the vertex data.
	for(i=0; i<m_vertexCount; i++)
	{
		fin >> model[i].x >> model[i].y >> model[i].z;
		fin >> model[i].ux >> model[i].uy;
		fin >> model[i].nx >> model[i].ny >> model[i].nz;
		
	}
	
	// Close the model file.
	fin.close();

	return true;
}

void Model::ReleaseModel()
{
	if(model)
	{
		delete model;
		model = 0;
	}

	return;
}

void Model::GenerateBoundingBox()
{
	BB_min = new D3DXVECTOR3(0,0,0);
	BB_max = new D3DXVECTOR3(0,0,0);

	for(int i = 0; i < m_vertexCount; i++)
	{
		BB_min->x = model[i].x < BB_min->x ? model[i].x : BB_min->x;
		BB_min->y = model[i].y < BB_min->y ? model[i].y : BB_min->y;
		BB_min->z = model[i].z < BB_min->z ? model[i].z : BB_min->z;

		BB_max->x = model[i].x > BB_max->x ? model[i].x : BB_max->x;
		BB_max->y = model[i].y > BB_max->y ? model[i].y : BB_max->y;
		BB_max->z = model[i].z > BB_max->z ? model[i].z : BB_max->z;
	}

	return;
}

void Model::ReleaseBB()
{
	BB_min = 0;
	BB_max = 0;
}


