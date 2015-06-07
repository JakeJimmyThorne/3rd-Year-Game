#include "Camera.h"

#define RADIANS 0.0174532925f

Camera::Camera()
{
	m_frameTime = 0.0f;

	m_debug = false;

	m_forwardSpeed   = 0.0f;
	m_backwardSpeed  = 0.0f;
	m_upwardSpeed    = 0.0f;
	m_downwardSpeed  = 0.0f;
	m_leftTurnSpeed  = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_lookUpSpeed    = 0.0f;
	m_lookDownSpeed  = 0.0f;
}

Camera::~Camera()
{
}

bool Camera::Initialise()
{
	bool result;

	result = AddTransform();
	if(!result)
		return false;

	GetTransform()->InitialiseMatrices();

	result = AddInput();
	if(!result)
		return false;

	GetInput()->Initialise(*GameObject::s_hinstance,*GameObject::s_hwnd);
	GetInput()->SetIndex(0);

	return true;
}

void Camera::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position = GetTransform()->GetPosition();

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = GetTransform()->GetRotation().x * RADIANS;
	yaw = GetTransform()->GetRotation().y * RADIANS;
	roll = GetTransform()->GetRotation().z * RADIANS;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&GetTransform()->GetWorldMatrix(), &position, &lookAt, &up);

	return;
}

void Camera::SetComputerPosition(D3DXVECTOR3 newPos)
{
	m_computerPos = newPos;
}

void Camera::Frame(float time)
{
	m_frameTime = time;

	//if(!m_debug)
	//{
	//	//camera to follow the players
	//	D3DXVECTOR3 newPos;
	//	newPos = (p1 + p2 + p3 + p4) / 4;

	//	GetTransform()->SetPosition(newPos.x, 55, newPos.z);
	//	// TODO : Add zoom
	//}


	#if _DEBUG

	//limit pressed to one when pressing enable debug
	/*if(GetInput()->ButtonPressedOnce("UP"))
	{
			if(!m_debug)
			{
				std::cout << "Entering Camera Debug\n";
				Player::s_debug = m_debug = true;

				m_oldPos = GetTransform()->GetPosition();
				m_oldRot = GetTransform()->GetRotation();
			}
			else
			{
				std::cout << "Exited Camera Debug\n\n";
				Player::s_debug = m_debug = false;

				GetTransform()->SetPosition(m_oldPos);
				GetTransform()->SetRotation(m_oldRot);
			}
	}*/

	/*if (GetInput()->ButtonPressedOnce("DOWN"))
		m_debugComputer = !m_debugComputer;*/

	//if (m_debug)
	//{
	//	float z = GetInput()->GetLeftStick('y') / 10;
	//	float x = GetInput()->GetLeftStick('x') / 10;

	//	float y = GetInput()->GetRightStick('y') / 2;

	//	//float turn = GetInput()->GetRightStick('x');

	//	bool up = GetInput()->GetButtonState().Y;
	//	bool down = GetInput()->GetButtonState().A;

	//	if (z != 0) GP_ForwardsBackwards(z);
	//	if (x != 0) GP_SidewaysStrafe(x);

	//	if (y != 0) GP_UpDown(y);

	//	LookUpward(up);
	//	LookDownward(down);
	//}

	//if (m_debugComputer)
	//{
	//	float z = GetInput()->GetLeftStick('y') / 10;
	//	float x = GetInput()->GetLeftStick('x') / 10;

	//	float y = GetInput()->GetRightStick('y') / 2;

	//	//float turn = GetInput()->GetRightStick('x');

	//	bool up = GetInput()->GetButtonState().Y;
	//	bool down = GetInput()->GetButtonState().A;

	//	m_computerPos.z += z * CAMERA_DEBUG_SPEED;
	//	m_computerPos.z += x * CAMERA_DEBUG_SPEED;

	//	m_computerPos.z -= y * CAMERA_DEBUG_SPEED;

	//	

	//	//if(turn != 0) GP_Turn(turn);
	//}

	#endif
}


#pragma region KeyBoardControls

void Camera::MoveForward(bool keydown)
{
	float radians;

	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_forwardSpeed += m_frameTime * 0.001f;

		if(m_forwardSpeed > (m_frameTime * 0.03f))
		{
			m_forwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_forwardSpeed -= m_frameTime * 0.0007f;

		if(m_forwardSpeed < 0.0f)
		{
			m_forwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = GetTransform()->GetRotation().y * 0.0174532925f;

	// Update the position.
	GetTransform()->GetPosition().x += sinf(radians) * m_forwardSpeed;
	GetTransform()->GetPosition().z += cosf(radians) * m_forwardSpeed;

	return;
}
void Camera::MoveBackward(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_backwardSpeed += m_frameTime * 0.001f;

		if(m_backwardSpeed > (m_frameTime * 0.03f))
		{
			m_backwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_backwardSpeed -= m_frameTime * 0.0007f;
		
		if(m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = GetTransform()->GetRotation().y * 0.0174532925f;

	// Update the position.
	GetTransform()->GetPosition().x -= sinf(radians) * m_backwardSpeed;
	GetTransform()->GetPosition().z -= cosf(radians) * m_backwardSpeed;

	return;
}
void Camera::MoveUpward(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_upwardSpeed += m_frameTime * 0.003f;

		if(m_upwardSpeed > (m_frameTime * 0.03f))
		{
			m_upwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_upwardSpeed -= m_frameTime * 0.002f;

		if(m_upwardSpeed < 0.0f)
		{
			m_upwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	GetTransform()->GetPosition().y += m_upwardSpeed;

	return;
}
void Camera::MoveDownward(bool keydown)
{
	// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_downwardSpeed += m_frameTime * 0.003f;

		if(m_downwardSpeed > (m_frameTime * 0.03f))
		{
			m_downwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_downwardSpeed -= m_frameTime * 0.002f;

		if(m_downwardSpeed < 0.0f)
		{
			m_downwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	GetTransform()->GetPosition().y -= m_downwardSpeed;

	return;
}
void Camera::TurnLeft(bool keydown)
{
	// Update the left turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if(m_leftTurnSpeed > (m_frameTime * 0.15f))
		{
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime* 0.005f;

		if(m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	// Update the rotation.
	GetTransform()->GetRotation().y -= m_leftTurnSpeed;

	// Keep the rotation in the 0 to 360 range.
	if(GetTransform()->GetRotation().y < 0.0f)
	{
		GetTransform()->GetRotation().y += 360.0f;
	}

	return;
}
void Camera::TurnRight(bool keydown)
{
	// Update the right turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_rightTurnSpeed += m_frameTime * 0.01f;

		if(m_rightTurnSpeed > (m_frameTime * 0.15f))
		{
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime* 0.005f;

		if(m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation.
	GetTransform()->GetRotation().y += m_rightTurnSpeed;

	// Keep the rotation in the 0 to 360 range.
	if(GetTransform()->GetRotation().y > 360.0f)
	{
		GetTransform()->GetRotation().y -= 360.0f;
	}

	return;
}
void Camera::LookUpward(bool keydown)
{
	// Update the upward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookUpSpeed += 1;

		if(m_lookUpSpeed > (1 * 0.15f))
		{
			m_lookUpSpeed = 1 * 0.15f;
		}
	}
	else
	{
		m_lookUpSpeed -= 1* 0.005f;

		if(m_lookUpSpeed < 0.0f)
		{
			m_lookUpSpeed = 0.0f;
		}
	}

	// Update the rotation.
	GetTransform()->GetRotation().x -= m_lookUpSpeed;

	// Keep the rotation maximum 90 degrees.
	if(GetTransform()->GetRotation().x > 90.0f)
	{
		GetTransform()->GetRotation().x = 90.0f;
	}

	return;
}
void Camera::LookDownward(bool keydown)
{
	// Update the downward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookDownSpeed += 1;

		if(m_lookDownSpeed > (1 * 0.15f))
		{
			m_lookDownSpeed = 1 * 0.15f;
		}
	}
	else
	{
		m_lookDownSpeed -= 1* 0.005f;

		if(m_lookDownSpeed < 0.0f)
		{
			m_lookDownSpeed = 0.0f;
		}
	}

	// Update the rotation.
	GetTransform()->GetRotation().x += m_lookDownSpeed;

	// Keep the rotation maximum 90 degrees.
	if(GetTransform()->GetRotation().x < -90.0f)
	{
		GetTransform()->GetRotation().x = -90.0f;
	}

	return;
}

#pragma endregion

#pragma region GamePad controls

void Camera::GP_ForwardsBackwards(float speed)
{
	GetTransform()->GetPosition().z += speed * CAMERA_DEBUG_SPEED;

	return;
}

void Camera::GP_SidewaysStrafe(float speed)
{
	GetTransform()->GetPosition().x += speed * CAMERA_DEBUG_SPEED;

	return;
}

void Camera::GP_UpDown(float speed)
{
	GetTransform()->GetPosition().y -= speed * CAMERA_DEBUG_SPEED;

	return;
}

void Camera::GP_Turn(float speed)
{
	GetTransform()->GetRotation().y -= speed * CAMERA_DEBUG_SPEED * 3;

	return;
}

#pragma endregion 