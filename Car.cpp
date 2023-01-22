#include "Car.h"

Car::Car()
{
	carX = SCREEN_WIDTH / 2.5;
	carY = SCREEN_HEIGHT - 4 * CAR_HEIGHT;
	carSpeedX = 0;
	carSpeedY = 0;
	collider.w = CAR_WIDTH;
	collider.h = CAR_HEIGHT;
	carLives = 2;
	score = 0;
}

Car::~Car()
{

}

void Car::collisionWithNeutral()
{
	interval = 300;
	canScored = false;
}

void Car::checkInterval()
{
	if (interval > 0)
	{
		interval--;
	}
	else
	{
		interval = 0;
		canScored = true;
	}
}

void Car::move()
{
	carX += carSpeedX;
	collider.x = carX;
	collider.y = carY;

	if ((carX < 0) || (carX + CAR_WIDTH > SCREEN_WIDTH))
	{
		carX -= carSpeedX;
		collider.x = carX;
		collider.y = carY;
	}
}


void Car::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT: carSpeedX -= CAR_SPEED; break;
		case SDLK_RIGHT: carSpeedX += CAR_SPEED;  break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT: carSpeedX += CAR_SPEED; break;
		case SDLK_RIGHT: carSpeedX -= CAR_SPEED; break;
		}
	}
}

void Car::render(SDL_Renderer* gRenderer, Texture* CarTexture)
{
	CarTexture->render(gRenderer, carX, carY);
};

int Car::getScore()
{
	return score;
}

int Car::setScore(int score)
{
	return score;
}

int Car::getX()
{
	return carX;
}

int Car::getY()
{
	return carY;
}

int Car::getLives()
{
	return carLives;
}

void Car::killed()
{
	alive = false;
}

bool Car::isAlive()
{
	return alive;
}