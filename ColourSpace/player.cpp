#include "player.h"

Player::Player()
{
}

Player::~Player()
{
}

bool Player::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, WCHAR* filename, int index,
						int screenWidth, int screenHeight, D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	bool result;

	velocity = 2.0f;
	alive = true;

	m_index = index;

	Object2D::Initialise(0, 0, device, deviceContext, screenWidth, screenHeight, pos.x, pos.y, pos.x + size.x, pos.y + size.y,
		filename);

	*Object2D::GetPosition() = pos;
	GetQuad()->UpdatePos(pos);

	result = AddInput();
	if(!result)
		return false;

	m_prevKeyPress = false;
	toolUsed = 0;
	playerIsDrawing = false;
	playerIsErasing = false;
	frameCount = 0;

	enableRight = true;
	enableLeft = true;
	enableUp = true;
	enableDown = true;

	m_isDraggingColor = false;
	m_isDraggingBlack = false;

	initiatedBlackDrag = false;
	initiatedColorDrag = false;

	dragTimer = 0;
	dragColorCooldown = 0;
	dragBlackCooldown = 0;

	firstPoint = true;
	ableToDrawShape = false;

	moveSpeed = 2.0f;
	time = 3.0f;

	GetInput()->SetIndex(index);
	
	return true;
}

void Player::Shutdown()
{
	Object2D::Shutdown();
}

void Player::Frame(float frameTime)
{
	if (frameTime < 0.5f)
	{
		if (m_isDraggingColor || m_isDraggingBlack)
		{
			dragTimer += frameTime;

			dragColorCooldown = 0;
			dragBlackCooldown = 0;

			if (dragTimer > COLOR_DURATION && m_isDraggingColor)
			{
				m_isDraggingColor = false;
			}

			if (dragTimer > BLACK_DURATION && m_isDraggingBlack)
			{
				m_isDraggingBlack = false;
			}
		}
		else
		{
			dragColorCooldown += frameTime;
			dragBlackCooldown += frameTime;

			if (initiatedColorDrag)
				initiatedColorDrag = false;

			if (initiatedBlackDrag)
				initiatedBlackDrag = false;
		}

	}

	GetInput()->Frame();
	PlayerMove(frameTime);
	
	if (initColorDrag)
		timer(frameTime);
}

void Player::PlayerMove(float dt)
{
	if ((GetInput()->IsUpPressed(m_index) || GetInput()->DPad_UpPressed()) && enableUp)
		GetQuad()->GetPosition()->y -= moveSpeed;
	else if ((GetInput()->IsDownPressed(m_index) || GetInput()->DPad_DownPressed()) && enableDown)
		GetQuad()->GetPosition()->y += moveSpeed;

	if ((GetInput()->IsLeftPressed(m_index) || GetInput()->DPad_LeftPressed()) && enableLeft)
		GetQuad()->GetPosition()->x -= moveSpeed;
	else if ((GetInput()->IsRightPressed(m_index) || GetInput()->DPad_RightPressed()) && enableRight)
		GetQuad()->GetPosition()->x += moveSpeed;


	if (GetInput()->PaintPressed() || GetInput()->IsActionPressed(m_index))
	{
		//moveSpeed = 1;

		playerIsDrawing = false;
		playerIsErasing = false;
		playerIsPainting = true;
	}
	else
	{
		moveSpeed = LerpAcceleration(moveSpeed, 5.0f, 0.075f);
		//moveSpeed = 4.0f;

		playerIsDrawing = false;
		playerIsErasing = false;
		playerIsPainting = false;
	}
}

bool Player::timer(float number)
{
	if (initColorDrag)
	{
		time -= number;
		if (time <= 0)
		{
			initColorDrag = false;
			time = 3.0f;
			playerIsPainting = false;
			return true;
		}

		return false;
	}

	return false;
}

//Checks if player is within canvas bounds and prevents movement off the screen
void Player::ScreenBoundsDetection(int x, int y)
{
	if (x <= 2)
	{
		enableLeft = false;
	}
	else
	{
		enableLeft = true;
	}

	if (x >= 1598)
	{
		enableRight = false;
	}
	else
	{
		enableRight = true;
	}

	if (y <= 2)
	{
		enableUp = false;
	}
	else
	{
		enableUp = true;
	}

	if (y >= 853)
	{
		enableDown = false;
	}
	else
	{
		enableDown = true;
	}
}

bool Player::CollisionDetectionX(int minX, int maxX)
{
	//Check if inside the swirl that's moving inwards
	if (GetQuad()->GetPosition()->x >= minX - 26 && GetQuad()->GetPosition()->x <= maxX - 26)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Player::KillPlayer()
{
	//Simple animation when the player dies
	//send them quite far away so that AI doesn't Flipout on them
	if (GetQuad()->GetPosition()->y < 2000)
	{
		velocity -= 0.15f;

		float newPos = GetQuad()->GetPosition()->y - velocity;
		GetQuad()->GetPosition()->y = newPos;
	}
}

bool Player::ShapeSize(int currentPosX, int currentPosY)
{
	//Adjust position of x to put it in texture space
	currentPosX = currentPosX - 38;

	//Sets the initial data for where it starts drawing
	if (firstPoint)
	{
		prevPosX = currentPosX;
		prevPosY = currentPosY;

		startPosX = currentPosX;
		startPosY = currentPosY;

		minX = currentPosX;
		minY = currentPosY;
		maxX = currentPosX;
		maxY = currentPosY;

		firstPoint = false;
	}

	//Check if any change to max or min X points
	if (currentPosX > prevPosX)
	{
		if (currentPosX > maxX)
			maxX = currentPosX;
	}
	else if (currentPosX < prevPosX)
	{
		if (currentPosX < minX)
			minX = currentPosX;
	}

	//Check if any change to max or min Y points
	if (currentPosY > prevPosY)
	{
		if (currentPosY > maxY)
			maxY = currentPosY;
	}
	else if (currentPosY < prevPosY)
	{
		if (currentPosY < minY)
			minY = currentPosY;
	}

	if (currentPosX > startPosX + 10 || currentPosX < startPosX - 10 || currentPosY > startPosY + 10 || currentPosY < startPosY - 10)
	{
		ableToDrawShape = true;
	}

	prevPosX = currentPosX;
	prevPosY = currentPosY;

	if (ableToDrawShape)
	{
		if (currentPosX < startPosX + 5 && currentPosX > startPosX - 5 && currentPosY < startPosY + 5 && currentPosY > startPosY - 5)
		{
			return true;
		}
	}

	return false;
}

float Player::LerpAcceleration(float start, float end, float percent)
{
	return (start + percent*(end - start));
}

void Player::SetPlayerColour(D3DXVECTOR4 colour)
{
	playerColour = colour;
}

D3DXVECTOR4 Player::GetPlayerColour()
{
	return playerColour;
}

void Player::PlayerRotate()
{

}

void Player::PlayerTrail()
{

}

void Player::PlayerDraw()
{

}

