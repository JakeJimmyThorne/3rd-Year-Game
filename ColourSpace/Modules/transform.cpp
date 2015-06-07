#include "transform.h"

//used to convert euler angles to radians
const float RADIAN = 0.0174532925f;

Transform::Transform()
{
	
}

Transform::~Transform()
{

}

void Transform::SetWorldMatrix(D3DXMATRIX newMatrix)
{
	m_worldMatrix = newMatrix;
	return;
}

D3DXVECTOR3& Transform::GetPosition()
{
	return m_position;
}

D3DXVECTOR3& Transform::GetRotation()
{
	return m_rotation;
}


void Transform::InitialiseMatrices()
{
	D3DXMatrixIdentity(&m_worldMatrix);
	m_position = D3DXVECTOR3(0,0,0);
	m_rotation = D3DXVECTOR3(0,0,0);

	UpdateMatrices();
	return;
}

void Transform::UpdateMatrices()
{
	D3DXMATRIX POS;
	D3DXMATRIX ROT;

	D3DXMatrixIdentity(&POS);
	D3DXMatrixIdentity(&ROT);

	ClampRotation();

	D3DXMatrixRotationYawPitchRoll(&ROT, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&POS,m_position.x,m_position.y,m_position.z);

	D3DXMatrixMultiply(&m_worldMatrix,&ROT, &POS);

	return;
}

void Transform::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR3(x,y,z);

	UpdateMatrices();
	return;
}

void Transform::SetPosition(D3DXVECTOR3 newPos)
{
	m_position = newPos;

	UpdateMatrices();
	return;
}

void Transform::SetRotation(float x, float y, float z)
{
	m_rotation = D3DXVECTOR3(x,y,z);
	UpdateMatrices();
	return;
}

void Transform::SetRotation(D3DXVECTOR3 newRot)
{
	m_rotation = newRot;
	UpdateMatrices();
	return;
}

void Transform::UpdatePosition(float x, float y, float z)
{
	m_position += D3DXVECTOR3(x,y,z);

	UpdateMatrices();
	return;
}

void Transform::UpdatePosition(D3DXVECTOR3 pos)
{
	m_position += pos;
	UpdateMatrices();
	return;
}

void Transform::UpdateRotation(float x, float y, float z)
{
	m_rotation += D3DXVECTOR3(x,y,z);
	UpdateMatrices();

	return;
}

void Transform::UpdateRotation(D3DXVECTOR3 rot)
{
	m_rotation += D3DXVECTOR3(rot.x*RADIAN, rot.y*RADIAN, rot.z*RADIAN);
	UpdateMatrices();

	return;
}

void Transform::ClampRotation()
{
	if(m_rotation.x > 360 || m_rotation.x < -360)
		m_rotation.x = 0;

	if(m_rotation.y > 360 || m_rotation.y < -360)
		m_rotation.y = 0;

	if(m_rotation.z > 360 || m_rotation.z < -360)
		m_rotation.z = 0;
}
