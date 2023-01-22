#include "Enemy.h"

Enemy::Enemy() : CollidedObject()
{
	enemyX = SCREEN_WIDTH / 5;
	enemyY = SCREEN_HEIGHT+100;
	enemySpeedX = BASE_ENEMY_SPEED;
	enemySpeedY = BASE_ENEMY_SPEED;

	collider.w = CAR_WIDTH;
	collider.h = CAR_HEIGHT;
}

Enemy::~Enemy()
{

};

void Enemy::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: enemySpeedY += BACKGROUND_SCROLL_SPEED; break;
		case SDLK_DOWN: enemySpeedY -= BACKGROUND_SCROLL_SPEED / 2; break;
		case SDLK_LEFT: leftPressed = true;
		case SDLK_RIGHT: rightPressed = true;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: enemySpeedY -= BACKGROUND_SCROLL_SPEED; break;
		case SDLK_DOWN: enemySpeedY += BACKGROUND_SCROLL_SPEED / 2; break;
		case SDLK_LEFT: leftPressed = false;
		case SDLK_RIGHT: rightPressed = false;
		}
	}
}

void Enemy::move()
{
	enemyY += enemySpeedY;

	if (isMovingRight)
	{
		if (enemyX < RIGHT_BORDER)
		{
			enemyX += SIDE_SPEED_AFTER_COLLISION;
		}
	}

	if (isMovingLeft)
	{
		if (enemyX > LEFT_BORDER)
		{
			enemyX -= SIDE_SPEED_AFTER_COLLISION;
		}
	}

	collider.x = enemyX;
	collider.y = enemyY;

	if (enemyY > SCREEN_HEIGHT+CAR_HEIGHT)
	{
		enemyX = ObjectGenerator::getRandomXPosition();
		enemyY = 0;
		collider.x = enemyX;
		collider.y = enemyY;

		if (!isAlive())
		{
			alive = true;
		}
		isMovingRight = false;
		isMovingLeft = false;
	}
}



//Shows the enemy on the screen
void Enemy::render(SDL_Renderer* gRenderer, Texture* enemyTexture)
{
	if (isAlive())
	{
		enemyTexture->render(gRenderer, enemyX, enemyY);
	}
};

int Enemy::getPosX()
{
	return enemyX;
};

int Enemy::getPosY()
{
	return enemyY;
};

int Enemy::getLives()
{
	return enemyLives;
}