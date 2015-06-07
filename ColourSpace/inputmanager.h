#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

//Make sure it's this version. It can default to 0x0800 but if anything changes it could break
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <XInput.h>
#include <string>

class InputManager
{
public:
	InputManager();
	InputManager(const InputManager&);
	~InputManager();

	bool Initialise(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	void GetMouseLocation(int&, int&);

	//Keyboard getters
	bool IsEscapePressed();
	bool IsLeftPressed();
	bool IsRightPressed();
	bool IsUpPressed();
	bool IsDownPressed();
	bool IsXPressed();
	bool IsZPressed();
	bool IsPgUpPressed();
	bool IsPgDownPressed();
	bool IsWPressed();
	bool IsAPressed();
	bool IsSPressed();
	bool IsDPressed();
	bool IsSpacePressed();

	//XInput functions
	bool IsConnected() { return m_connected; }
	bool IsInverted_X() { return m_xAxis_Inverted; }
	bool IsInverted_Y() { return m_yAxis_Inverted; }
	int GetSensitivity() { return m_sensitivity; }

	float GetLeftStick(char axis);
	float GetRightStick(char axis);

	bool GetButtonPressed(std::string);

	bool GetMousePressed() { return m_mouseState.rgbButtons[0]; }

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessMouse();

	HRESULT UpdateControllerState();

private:
	HWND m_hwnd;
	HWND m_hWndParent;
	POINT m_cursorPoint;

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

	//XInput controller variables
	XINPUT_STATE m_ControllerState;

    bool m_connected;
	bool m_deadZoneOn;

	//GAMEPAD CONFIGURATION
	bool m_xAxis_Inverted;
	bool m_yAxis_Inverted;
	int m_sensitivity;
};

#endif