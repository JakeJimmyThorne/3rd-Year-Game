#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <d3d11.h>
#include <d3dx10math.h>

class Transform
{
public:

	Transform();
	~Transform();

	void SetWorldMatrix(D3DXMATRIX);
	D3DXMATRIX &GetWorldMatrix() { return m_worldMatrix; }

	D3DXVECTOR3& GetPosition();
	D3DXVECTOR3& GetRotation();

	void InitialiseMatrices();
	void UpdateMatrices();

	//Setters for initialising at that pos/rot
	void SetPosition(float, float, float);
	void SetPosition(D3DXVECTOR3);

	void SetRotation(float, float, float);
	void SetRotation(D3DXVECTOR3);

	//Updaters to use during the main logic to update pos/rot
	void UpdatePosition(float, float, float);
	void UpdatePosition(D3DXVECTOR3);

	void UpdateRotation(float, float, float);
	void UpdateRotation(D3DXVECTOR3);

	void ClampRotation();

private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_rotation;
	D3DXMATRIX m_worldMatrix;
};

#endif