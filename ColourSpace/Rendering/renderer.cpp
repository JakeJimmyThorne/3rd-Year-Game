#include "renderer.h"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

bool Renderer::Initialise(ID3D11DeviceContext* devContext, ID3D11Device* dev, HWND hwnd, D3DXMATRIX* world, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	bool result;

	m_worldMatrix = world;
	m_viewMatrix = view;
	m_projectionMatrix = proj;

	result = m_TextureShader.Initialise(dev, hwnd);
	if(!result)
		return false;

	return true;
}

void Renderer::Shutdown()
{
	m_worldMatrix = 0;
	m_viewMatrix = 0;
	m_projectionMatrix = 0;
}

bool Renderer::Frame()
{

	return true;
}



void Renderer::Render(ID3D11DeviceContext* deviceContext, int indexCount,ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer,unsigned int size)
{
	
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = size; 
	offset = 0;
    
	//set the vertex&index buffers to active in the input assembler
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    //set the type of primitive to render
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool Renderer::RenderTexture(ID3D11DeviceContext* deviceContext, int indexCount, ID3D11ShaderResourceView* texture, D3DXMATRIX* uiView, D3DXMATRIX* ortho, float alpha)
{
	bool result;

	result = m_TextureShader.Render(deviceContext, indexCount, *m_worldMatrix, *uiView, *ortho, texture, alpha);
	if(!result)
		return false;

	return true;
}

