#include "inputmanager.h"
#include <string>

const float INPUT_DEADZONE = ( 0.20f * FLOAT(0x7FFF) );

InputManager::InputManager()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;

	m_xAxis_Inverted = false;
	m_yAxis_Inverted = false;

	//Higher int is less sensitivity
	m_sensitivity = 8;
}


InputManager::InputManager(const InputManager& other)
{
}


InputManager::~InputManager()
{
}


bool InputManager::Initialise(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	m_hwnd = hwnd;
	m_hWndParent = GetParent(m_hwnd);
	m_cursorPoint = { 0 };

	MapWindowPoints(m_hwnd, m_hWndParent, &m_cursorPoint, 1);

	//Enable XInput
	XInputEnable(true);

	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialise the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialise the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Initialise the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	// Initialise the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if(FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void InputManager::Shutdown()
{
	// Release the mouse.
	if(m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if(m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if(m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}


bool InputManager::Frame()
{
	bool result;


	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if(!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if(!result)
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessMouse();
	UpdateControllerState();


	return true;
}


bool InputManager::ReadKeyboard()
{
	HRESULT result;

	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if(FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
		
	return true;
}

bool InputManager::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if(FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


void InputManager::ProcessMouse()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	///m_mouseX += m_mouseState.lX;
	//m_mouseY += m_mouseState.lY;

	POINT cursorPos;
	GetCursorPos(&cursorPos);
	
	m_mouseX = cursorPos.x - m_cursorPoint.x;
	m_mouseY = cursorPos.y - m_cursorPoint.y;

	// Ensure the mouse location doesn't exceed the screen width or height.
	//if(m_mouseX < 0)  { m_mouseX = 0; }
	//if(m_mouseY < 0)  { m_mouseY = 0; }
	
	//if(m_mouseX > m_screenWidth)  { m_mouseX = m_screenWidth; }
	//if(m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
	
	

	return;
}


void InputManager::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

#pragma region KeyboardGetters

bool InputManager::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputManager::IsLeftPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_LEFT] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputManager::IsRightPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_RIGHT] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputManager::IsUpPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_UP] & 0x80)
	{
		return true;
	}

	/*if(controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		return true;
	}*/

	return false;
}


bool InputManager::IsDownPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_DOWN] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputManager::IsXPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_X] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputManager::IsZPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_Z] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputManager::IsPgUpPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_PGUP] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputManager::IsPgDownPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_PGDN] & 0x80)
	{
		return true;
	}

	return false;
}

//Player Input
bool InputManager::IsWPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_W] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputManager::IsAPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_A] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputManager::IsSPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_S] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputManager::IsDPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_D] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputManager::IsSpacePressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_SPACE] & 0x80)
	{
		return true;
	}

	return false;
}

#pragma endregion

#pragma region ControllerGetters

float InputManager::GetLeftStick(char axis)
{
	if(axis == 'x')
	{
		//Only return the stick value if not in the dead zone
		if(m_ControllerState.Gamepad.sThumbLX < INPUT_DEADZONE && m_ControllerState.Gamepad.sThumbLX > -INPUT_DEADZONE)
			return 0;
		else
			return (float)m_ControllerState.Gamepad.sThumbLX / 3276;
	}
	else if(axis == 'y')
	{
		if(m_ControllerState.Gamepad.sThumbLY < INPUT_DEADZONE && m_ControllerState.Gamepad.sThumbLY > -INPUT_DEADZONE)
			return 0;
		else
			return (float)m_ControllerState.Gamepad.sThumbLY / 3276;
	}

	return 0;
}

float InputManager::GetRightStick(char axis)
{
	if(axis == 'x')
	{
		//Only return the stick value if not in the dead zone
		if(m_ControllerState.Gamepad.sThumbRX < INPUT_DEADZONE && m_ControllerState.Gamepad.sThumbRX > -INPUT_DEADZONE)
			return 0;
		else
		{
			if(!m_xAxis_Inverted)
				return (float)-m_ControllerState.Gamepad.sThumbRX / 3276;
			else
				return (float)m_ControllerState.Gamepad.sThumbRX / 3276;
		}
	}
	else if(axis == 'y')
	{
		if(m_ControllerState.Gamepad.sThumbRY < INPUT_DEADZONE && m_ControllerState.Gamepad.sThumbRY > -INPUT_DEADZONE)
			return 0;
		else
		{
			if(!m_yAxis_Inverted)
				return (float)m_ControllerState.Gamepad.sThumbRY / 3276;
			else
				return (float)-m_ControllerState.Gamepad.sThumbRY / 3276;
		}
	}

	return 0;
}

bool InputManager::GetButtonPressed(std::string button)
{
	if(button == "Y")
		return m_ControllerState.Gamepad.wButtons && XINPUT_GAMEPAD_Y;
	else if(button == "B")
		return m_ControllerState.Gamepad.wButtons && XINPUT_GAMEPAD_B;
	else if(button  == "X")
		return m_ControllerState.Gamepad.wButtons && XINPUT_GAMEPAD_X;
	else if(button == "A")
		return m_ControllerState.Gamepad.wButtons && XINPUT_GAMEPAD_A;

	if(button == "LB")
		return m_ControllerState.Gamepad.wButtons && XINPUT_GAMEPAD_LEFT_THUMB;
	else if(button == "RB")
		return m_ControllerState.Gamepad.wButtons && XINPUT_GAMEPAD_RIGHT_THUMB;

	return false;
}

#pragma endregion 

HRESULT InputManager::UpdateControllerState()
{
    DWORD dwResult;
    for( DWORD i = 0; i < 1; i++ )
    {
        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState( i, &m_ControllerState );

        if( dwResult == ERROR_SUCCESS )
            m_connected = true;
        else
            m_connected = false;
    }

    return S_OK;
}