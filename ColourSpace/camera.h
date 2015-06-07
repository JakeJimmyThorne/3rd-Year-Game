#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <D3DX11.h>
#include <d3dx10math.h>

#include <iostream>

#include "player.h"
#include "gameobject.h"


const float CAMERA_DEBUG_SPEED = 0.04f;

/*
Press down on the D-PAD on controller one to active debug camera 

*/

class Camera : public GameObject
{
public:
	Camera();
	~Camera();

	bool Initialise();

	//setters for initialisation use or to quickly jump the camera from one place
	//to another.
	void SetPosition(float x, float y, float z) { GetTransform()->SetPosition(x,y,z); }
	void SetRotation(float x, float y, float z) { GetTransform()->SetRotation(x,y,z); }

	D3DXVECTOR3 GetPosition() { return GetTransform()->GetPosition(); }
	D3DXVECTOR3 GetRotation() { return GetTransform()->GetRotation(); }

	//Called in GameManager::RenderScene() - we use the world matrix from transform module as view matrix
	D3DXMATRIX& GetViewMatrix() { return GetTransform()->GetWorldMatrix(); }

	D3DXVECTOR3 GetComputerPosition() { return m_computerPos; }

	//Called in GameManager::RenderScene() - needs to be called every frame before any rendering takes place.
	//Updates matrices
	void Render();

	void SetComputerPosition(D3DXVECTOR3);

	void Frame(float);


	//Called in GameManager::HandleInput() - called every frame, pass through a bool, if true move
	//keyboard movement
	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUpward(bool);
	void LookDownward(bool);

	//gamepad functions
	//Called in GameManager::HandleInput() - called every frame, pass through values from sticks, bools for buttons
	void GP_ForwardsBackwards(float);
	void GP_SidewaysStrafe(float);

	void GP_Turn(float);

	void GP_UpDown(float);

private:
	float m_frameTime;

	bool m_debug;
	bool m_debugComputer;


	D3DXVECTOR3 m_computerPos;
	D3DXVECTOR3 m_computerPosOld;

	//to use to store the old position when going into debug mode
	D3DXVECTOR3 m_oldPos;
	D3DXVECTOR3 m_oldRot;

	//speeds that get added to m_position & m_rotation
	float m_forwardSpeed, m_backwardSpeed;
	float m_upwardSpeed, m_downwardSpeed;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_lookUpSpeed, m_lookDownSpeed;
};

#endif