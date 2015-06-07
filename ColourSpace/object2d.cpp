#include "object2d.h"

Object2D::Object2D()
{
	
}

Object2D::~Object2D()
{

}

bool Object2D::Initialise(int x, int y, ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, WCHAR* filename)
{
	bool result;

	result = AddQuad(device, screenWidth, screenHeight);
	if (!result)
		return false;

	//specify x and y if the texture can be overwritten
	result = AddTexture(x,y,device, deviceContext, filename);
	if (!result)
		return false;

	return true;
}

bool Object2D::Initialise(int x, int y, ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, float bx, float by, float ex, float ey, WCHAR* filename)
{
	bool result;

	result = AddQuad(device, screenWidth, screenHeight, bx, by, ex, ey);
	if (!result)
		return false;

	//specify x and y if the texture can be overwritten
	result = AddTexture(x, y, device, deviceContext, filename);
	if (!result)
		return false;

	return true;
}

void Object2D::Shutdown()
{
	GameObject::Shutdown();
}
