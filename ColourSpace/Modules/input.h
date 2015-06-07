#ifndef _INPUT_H_
#define _INPUT_H_

//Make sure it's this version. It can default to 0x0800 but if anything changes it could break
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <XInput.h>
#include <string>
#include <iostream>

/* Class explanation

Input is a module class to be added to GameObjects.

The main use for this will be to assign an input module to each player
so that they all have their own functions. Each player will have a different
controller index to say what controller is mapped to which player.

*/

class Input
{
public:
	struct Buttons
	{
		bool X, Y, B, A;
		bool START, BACK;
		bool LB, RB;
		bool UP, DOWN, LEFT, RIGHT;
	};

public:
	Input();
	~Input();

	bool Initialise(HINSTANCE, HWND);
	void Shutdown();
	bool Frame();

	//Setters
	void SetIndex(int);
	void SetInverted(bool,bool);
	void SetSensitivity(int);

	//keyboard functions
	bool IsLeftPressed(int);
	bool IsRightPressed(int);
	bool IsUpPressed(int);
	bool IsDownPressed(int);

	bool IsActionPressed(int);

	bool PencilPressed();
	bool PaintPressed();
	bool MarqueePressed();


	//XInput getters
	bool IsConnected()   { return m_connected; }
	bool IsInverted_X()  { return m_xAxis_Inverted; }
	bool IsInverted_Y()  { return m_yAxis_Inverted; }
	int GetSensitivity() { return m_sensitivity; }

	//return values between 0 and 10
	float GetLeftStick(char axis);
	float GetRightStick(char axis);

	Buttons& GetButtonState() { return m_buttonState; }

	bool ButtonPressedOnce(std::string);
	bool ButtonPressed(std::string);

	bool DPad_LeftPressed();
	bool DPad_RightPressed();
	bool DPad_UpPressed();
	bool DPad_DownPressed();

	float RightTrigger();

private:
	bool UpdateControllerState();

	bool ReadKeyboard();

private:

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;

	unsigned char m_keyboardState[256];

	//player numbers 0 - 3
	int m_controllerIndex;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;


	bool m_actionPrev;




	//old xinput stuff
	XINPUT_STATE m_ControllerState;
	Buttons m_buttonState;
	Buttons m_buttonStatePrev;

    bool m_connected;
	bool m_deadZoneOn;

	//GAMEPAD CONFIGURATION
	bool m_xAxis_Inverted;
	bool m_yAxis_Inverted;
	int m_sensitivity;
};

#endif