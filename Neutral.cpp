#include "Neutral.h"
#include "CollidedObject.h"

Neutral::Neutral() : CollidedObject()
{
	notEnemyX = SCREEN_WIDTH / 1.4;
		notEnemyY = 0;
		notEnemySpeedX = BASE_NOT_ENEMY_SPEED;
		notEnemySpeedY = BASE_NOT_ENEMY_SPEED;

		collider.w = CAR_WIDTH;
		collider.h = CAR_HEIGHT;
}

Neutral::~Neutral()
{

};

void Neutral::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: notEnemySpeedY += BACKGROUND_SCROLL_SPEED; break;
		case SDLK_DOWN: notEnemySpeedY -= BACKGROUND_SCROLL_SPEED / 2; break;
		case SDLK_LEFT: leftPressed = true;
		case SDLK_RIGHT: rightPressed = true;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: notEnemySpeedY -= BACKGROUND_SCROLL_SPEED; break;
		case SDLK_DOWN: notEnemySpeedY += BACKGROUND_SCROLL_SPEED / 2; break;
		case SDLK_LEFT: leftPressed = false;
		case SDLK_RIGHT: rightPressed = false;
		}
	}
}

void Neutral::move()
{
	notEnemyY += notEnemySpeedY;
	if (isMovingRight)
	{
		if (notEnemyX < RIGHT_BORDER)
		{
			notEnemyX += SIDE_SPEED_AFTER_COLLISION;
		}
	}

	if (isMovingLeft)
	{
		if (notEnemyX > LEFT_BORDER)
		{
			notEnemyX -= SIDE_SPEED_AFTER_COLLISION;
		}
	}
	collider.x = notEnemyX;
	collider.y = notEnemyY;

	if (notEnemyY > SCREEN_HEIGHT + SPACE_BW_CARS)
	{
		notEnemyX = ObjectGenerator::getRandomXPosition();
		notEnemyY = 0;

		collider.x = notEnemyX;
		collider.y = notEnemyY;

		if (!isAlive())
		{
			alive = true;
		}
		isMovingLeft = false;
		isMovingRight = false;
	}
}

void Neutral::render(SDL_Renderer* gRenderer, Texture* notEnemy)
{
	if (isAlive())
	{
		notEnemy->render(gRenderer, notEnemyX, notEnemyY);
	}
};

int Neutral::getPosX()
{
	return notEnemyX;
};

int Neutral::getPosY()
{
	return notEnemyY;
};

int Neutral::getLives()
{
	return enemyLives;
}