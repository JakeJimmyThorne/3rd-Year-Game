#include "uiobject.h"

UIObject::UIObject()
{
	m_active = true;
}

UIObject::~UIObject()
{

}

bool UIObject::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, WCHAR* filename)
{
	bool result;

	result = AddQuad(device, screenWidth, screenHeight);
	if(!result)
		return false;

	result = AddTexture(0,0,device, deviceContext, filename);
	if(!result)
		return false;

	return true;
}

bool UIObject::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, float bx, float by, float ex, float ey, WCHAR* filename)
{
	bool result;

	result = AddQuad(device, screenWidth, screenHeight, bx, by, ex, ey);
	if(!result)
		return false;

	result = AddTexture(0, 0, device, deviceContext, filename);
	if(!result)
		return false;

	return true;
}

void UIObject::Shutdown()
{
	GameObject::Shutdown();
}
