#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <D3DX11.h>
#include <D3DX10math.h>

#include "object2d.h"
#include "Modules\texture.h"

const float MOVE_SPEED = 2;
const int DEGREES_OF_FREEDOM = 8;

const float COLOR_DURATION = 1.5f;
const float BLACK_DURATION = 1;

const float COLOR_COOLDOWN = 2;
const float BLACK_COOLDOWN = 4;


class Player : public Object2D
{
public:
	Player();
	~Player();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, HWND, WCHAR*, int,
		int, int, D3DXVECTOR2, D3DXVECTOR2);
	void Shutdown();

	void Frame(float);

	void SetPlayerColour(D3DXVECTOR4 colour);
	D3DXVECTOR4 GetPlayerColour();

	void ScreenBoundsDetection(int x, int y);
	bool CollisionDetectionX(int xMax, int xMin);
	void KillPlayer();
	bool alive;
	float velocity;
	
	//Function and variables for finding bounds of drawn shape
	bool ShapeSize(int currentPosX, int currentPosY);
	bool firstPoint;
	bool ableToDrawShape;
	int prevPosX;
	int prevPosY;
	int minX;
	int maxX;
	int minY;
	int maxY;
	int startPosX;
	int startPosY;

	bool playerIsDrawing;
	bool playerIsPainting;
	bool playerIsErasing;
	unsigned int toolUsed;

	bool enableRight;
	bool enableLeft;
	bool enableUp;
	bool enableDown;

	float moveSpeed;

	bool movingThroughPaint = false;
	bool onOtherColor = false;
	bool onBlack = false;

	bool timer(float number);
	float time;

	bool paintingAccepted = false;

	bool initColorDrag = false;
	D3DXVECTOR4 lastDragColor = D3DXVECTOR4(1,1,1,1);

	void PlayerPaintSpeed()
	{
		moveSpeed = 2;
	}

	void PlayerGreySpeed()
	{
		if (playerIsPainting)
			moveSpeed = 0.5f;
		else
			moveSpeed = 0.75f;
	}

	void PlayerBlackSpeed()
	{
		moveSpeed = 0.2f;
	}

	//NEW COLOR DRAG FUNCTIONS
	bool IsDraggingColor() { return m_isDraggingColor; }
	bool IsDraggingBlack() { return m_isDraggingBlack; }

	void SetDraggingColor(bool b) { m_isDraggingColor = b; }
	void SetDraggingBlack(bool b) { m_isDraggingBlack = b; }

	void ResetTimer() { dragTimer = 0; }

	void ResetColorCooldown() { dragColorCooldown = 0; }
	void ResetBlackCooldown() { dragBlackCooldown = 0; }

	bool CooldownColorDone() { return dragColorCooldown > COLOR_COOLDOWN ? true : false; }
	bool CooldownBlackDone() { return dragBlackCooldown > BLACK_COOLDOWN ? true : false; }

	bool HasInitiatedColor() { return initiatedColorDrag; }
	bool HasInitiatedBlack() { return initiatedBlackDrag; }

	void SetInitiatedColor() { initiatedColorDrag = true; }
	void SetInitiatedBlack() { initiatedBlackDrag = true; }

	D3DXVECTOR4 GetDragColor() { return m_dragColor; }
	void SetDragColor(D3DXVECTOR4 col) { m_dragColor = col; }

private:

	float LerpAcceleration(float, float, float);

	void PlayerMove(float);
	void PlayerRotate();

	void PlayerTrail();
	void PlayerDraw();

private:

	//NEW color drag variables
	bool m_isDraggingColor;
	bool m_isDraggingBlack;

	bool initiatedBlackDrag;
	bool initiatedColorDrag;

	float dragTimer;
	float dragColorCooldown;
	float dragBlackCooldown;

	D3DXVECTOR4 m_dragColor;


	D3DXVECTOR3 m_direction;
	int m_index;

	bool m_prevKeyPress;


	int frameCount;

	D3DXVECTOR4 playerColour;
};

#endif