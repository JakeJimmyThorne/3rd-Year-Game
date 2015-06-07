#include "input.h"

const float INPUT_DEADZONE = ( 0.20f * FLOAT(0x7FFF) );

Input::Input()
{
	m_xAxis_Inverted = false;
	m_yAxis_Inverted = false;

	//Higher int is less sensitivity
	m_sensitivity = 8;

	m_controllerIndex = 0;

	m_directInput = 0;
	m_keyboard = 0;
	m_actionPrev = false;
}

Input::~Input()
{

}

bool Input::Initialise(HINSTANCE hinstance, HWND hwnd)
{
	HRESULT result;

	//Enable XInput
	XInputEnable(true);

	// Initialise the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialise the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Input::Shutdown()
{
	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}

bool Input::Frame()
{
	bool result;

	result = ReadKeyboard();
	if (!result)
		return false;

	result = UpdateControllerState();

	if(!result)
		return false;

	



	return true;
}

bool Input::ReadKeyboard()
{
	HRESULT result;

	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
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

bool Input::IsLeftPressed(int index)
{
	if (m_keyboardState[DIK_A] & 0x80 && index == 0)
		return true;

	if (m_keyboardState[DIK_F] & 0x80 && index == 1)
		return true;

	if (m_keyboardState[DIK_J] & 0x80 && index == 2)
		return true;

	if (m_keyboardState[DIK_LEFT] & 0x80 && index == 3)
		return true;

	return false;
}


bool Input::IsRightPressed(int index)
{
	if (m_keyboardState[DIK_D] & 0x80 && index == 0)
		return true;

	if (m_keyboardState[DIK_H] & 0x80 && index == 1)
		return true;

	if (m_keyboardState[DIK_L] & 0x80 && index == 2)
		return true;

	if (m_keyboardState[DIK_RIGHT] & 0x80 && index == 3)
		return true;

	return false;
}


bool Input::IsUpPressed(int index)
{
	if (m_keyboardState[DIK_W] & 0x80 && index == 0)
		return true;

	if (m_keyboardState[DIK_T] & 0x80 && index == 1)
		return true;

	if (m_keyboardState[DIK_I] & 0x80 && index == 2)
		return true;

	if (m_keyboardState[DIK_UP] & 0x80 && index == 3)
		return true;

	return false;
}


bool Input::IsDownPressed(int index)
{
	if (m_keyboardState[DIK_S] & 0x80 && index == 0)
		return true;

	if (m_keyboardState[DIK_G] & 0x80 && index == 1)
		return true;

	if (m_keyboardState[DIK_K] & 0x80 && index == 2)
		return true;

	if (m_keyboardState[DIK_DOWN] & 0x80 && index == 3)
		return true;

	return false;
}

bool Input::IsActionPressed(int index)
{
	if (index == 0)
	{
		if (m_keyboardState[DIK_E] & 0x80)
		{
			return true;
		}
	}
	
	if (index == 1)
	{
		if (m_keyboardState[DIK_Y] & 0x80)
		{
			return true;
		}
	}

	if (index == 2)
	{
		if (m_keyboardState[DIK_O] & 0x80)
		{
			return true;
		}
	}

	if (index == 3)
	{
		if (m_keyboardState[DIK_NUMPAD0] & 0x80)
		{
			return true;
		}
	}

	m_actionPrev = false;
	return false;
}






void Input::SetIndex(int idx)
{
	m_controllerIndex = idx;
}

void Input::SetInverted(bool xInv, bool yInv)
{
	m_xAxis_Inverted = xInv;
	m_yAxis_Inverted = yInv;
}

void Input::SetSensitivity(int sens)
{
	m_sensitivity = sens;
}


float Input::GetLeftStick(char axis)
{
	if(m_connected)
	{
		if(axis == 'x' || axis == 'X')
		{
			//Only return the stick value if not in the dead zone
			if (m_ControllerState.Gamepad.sThumbLX < INPUT_DEADZONE && m_ControllerState.Gamepad.sThumbLX > -INPUT_DEADZONE)
				return 0;
			else
				return (float)m_ControllerState.Gamepad.sThumbLX / 327;
		}
		else if(axis == 'y' || axis == 'Y')
		{
			if (m_ControllerState.Gamepad.sThumbLY < INPUT_DEADZONE && m_ControllerState.Gamepad.sThumbLY > -INPUT_DEADZONE)
				return 0;
			else
				return (float)m_ControllerState.Gamepad.sThumbLY / 327;
		}
	}

	return 0;
}

float Input::GetRightStick(char axis)
{
	if(m_connected)
	{
		if(axis == 'x' || axis == 'X')
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
		else if(axis == 'y' || axis == 'Y')
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
	}

	return 0;
}

bool Input::ButtonPressedOnce(std::string button)
{
	bool result = false;

	if(button == "B" || button == "b")
	{
		if(m_buttonState.B)
		{
			if(!m_buttonStatePrev.B)
			{
				m_buttonStatePrev.B = true;
				result = true;	
			}
		}
		else m_buttonStatePrev.B = false;
	}

	if (button == "A" || button == "a")
	{
		if (m_buttonState.A)
		{
			if (!m_buttonStatePrev.A)
			{
				m_buttonStatePrev.A = true;
				result = true;
			}
		}
		else m_buttonStatePrev.A = false;
	}

	if(button == "DOWN" || button == "down")
	{
		if(m_buttonState.DOWN)
		{
			if(!m_buttonStatePrev.DOWN)
			{
				m_buttonStatePrev.DOWN = true;
				result = true;	
			}
		}
		else m_buttonStatePrev.DOWN = false;
	}

	if (button == "UP" || button == "up")
	{
		if (m_buttonState.UP)
		{
			if (!m_buttonStatePrev.UP)
			{
				m_buttonStatePrev.UP = true;
				result = true;
			}
		}
		else m_buttonStatePrev.UP = false;
	}

	return result;
}

bool Input::ButtonPressed(std::string button)
{
	if (!m_connected)
		return false;

	if (button == "B" || button == "b")
	{
		return m_buttonState.B;
	}

	if (button == "A" || button == "a")
	{
		return m_buttonState.A;
	}

	if (button == "X" || button == "x")
	{
		return m_buttonState.X;
	}

	if (button == "Y" || button == "y")
	{
		return m_buttonState.Y;
	}

	return false;
}

bool Input::PencilPressed()
{
	if (!m_connected)
		return false;

	return m_buttonState.A;
}

bool Input::PaintPressed()
{
	if (!m_connected)
		return false;

	if (m_buttonState.B || m_buttonState.A || m_buttonState.Y || m_buttonState.X)
		return true;
	else return false;
}

bool Input::MarqueePressed()
{
	if (!m_connected)
		return false;

	return m_buttonState.Y;
}

float Input::RightTrigger()
{
	if (!m_connected)
		return 0;

	return m_ControllerState.Gamepad.bRightTrigger / 255.0f;
}

bool Input::UpdateControllerState()
{
	DWORD dwResult;

    //get the state of the controller from XInput
    dwResult = XInputGetState(m_controllerIndex, &m_ControllerState);

    if( dwResult == ERROR_SUCCESS )
        m_connected = true;
	else
        m_connected = false;  

	if(m_connected)
	{
		m_buttonState.X = m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X ? true : false;
		m_buttonState.Y = m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y ? true : false;
		m_buttonState.B = m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B ? true : false;
		m_buttonState.A = m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A ? true : false;

		//m_buttonState.START = m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START ? true : false;
		//m_buttonState.BACK = m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK ? true : false;

		//m_buttonState.LB = m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ? true : false;
		//m_buttonState.RB = m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ? true : false;

		m_buttonState.UP = m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP ? true : false;
		m_buttonState.DOWN = m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN ? true : false;
		m_buttonState.LEFT = m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT ? true : false;
		m_buttonState.RIGHT = m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ? true : false;
	}

    return m_connected;
}

bool Input::DPad_LeftPressed()
{
	if (!m_connected)
		return false;

	return m_buttonState.LEFT ? true : false;
}
bool Input::DPad_RightPressed()
{
	if (!m_connected)
		return false;

	return m_buttonState.RIGHT ? true : false;
}
bool Input::DPad_UpPressed()
{
	if (!m_connected)
		return false;

	return m_buttonState.UP ? true : false;
}
bool Input::DPad_DownPressed()
{
	if (!m_connected)
		return false;

	return m_buttonState.DOWN ? true : false;
}
