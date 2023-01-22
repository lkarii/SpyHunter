#pragma once
class CollidedObject
{
public:
	void collisionWithPlayer()
	{
		if (leftPressed)
		{
			moveToLeftSide();
		}
		else if (rightPressed)
		{
			moveToRightSide();
		}
		else
		{
			killed();
		}
	}

	void killed()
	{
		alive = false;
	}
protected:

	const int SIDE_SPEED_AFTER_COLLISION = 7;
	const int RIGHT_BORDER = 620;
	const int LEFT_BORDER = 20;


	bool isAlive()
	{
		return alive;
	}

	void moveToRightSide()
	{
		this->isMovingRight = true;
	}

	void moveToLeftSide()
	{
		this->isMovingLeft = true;
	}

	bool isMovingRight = false;
	bool isMovingLeft = false;
	bool leftPressed = false;
	bool rightPressed = false;
	bool alive = true;
};

