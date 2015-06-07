#include "gameobject.h"

HINSTANCE* GameObject::s_hinstance;
HWND* GameObject::s_hwnd;

GameObject::GameObject()
{
	m_Input = 0;
	m_Quad = 0;
	m_Sound = 0;
	m_Texture = 0;
	m_Transform = 0;


}

GameObject::~GameObject()
{

}

void GameObject::Shutdown()
{
	if(m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	if(m_Quad)
	{
		m_Quad->Shutdown();
		delete m_Quad;
		m_Quad = 0;
	}

	if(m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}

	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	if(m_Transform)
	{
		delete m_Transform;
		m_Transform = 0;
	}
}

void GameObject::Frame(float frameTime)
{

}

bool GameObject::AddInput()
{
	bool result;

	m_Input = new Input;
	result = m_Input->Initialise(*s_hinstance,*s_hwnd);

	if(!result)
		return false;

	return true;
}

bool GameObject::AddQuad(ID3D11Device* device, int screenWidth, int screenHeight)
{
	bool result;

	m_Quad = new Quad;
	result = m_Quad->Initialise(device, screenWidth, screenHeight);

	if(!result)
		return false;

	return true;
}

bool GameObject::AddQuad(ID3D11Device* device, int screenWidth, int screenHeight, float bx, float by, float ex, float ey)
{
	bool result;

	m_Quad = new Quad;
	result = m_Quad->Initialise(device, screenWidth, screenHeight, bx, by, ex, ey);

	if(!result)
		return false;

	return true;
}

bool GameObject::AddSound(const char* filename,HWND* hwnd)
{
	bool result;

	m_Sound = new Sound;
	result = m_Sound->Initialize(filename, *hwnd, 0);

	if(!result)
		return false;

	return true;
}

bool GameObject::AddTexture(int x, int y, ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)
{
	bool result;

	m_Texture = new Texture;
	result = m_Texture->Initialise(x,y,device, deviceContext, filename);

	if(!result)
		return false;

	return true;
}

bool GameObject::AddTransform()
{
	m_Transform = new Transform;

	return true;
}

bool GameObject::LoadTexture(ID3D11Device* dev, WCHAR* filename)
{
	if (m_Texture)
	{
		return m_Texture->LoadTexture(dev, filename);
	}
	else return false;
}

