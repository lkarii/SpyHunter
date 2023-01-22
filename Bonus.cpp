#include "Bonus.h"

Bonus::Bonus(int distanceMultiply)
{
	bonusX = SCREEN_WIDTH / 3;
	bonusY = -100;
	bonusSpeedX = BASE_ENEMY_SPEED;
	bonusSpeedY = BASE_ENEMY_SPEED;
	enemyLives = 0;

	collider.w = CAR_WIDTH;
	collider.h = CAR_HEIGHT;
	isTaken = false;

	this->distanceMultiply = distanceMultiply;
};

Bonus::~Bonus()
{
};

void Bonus::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
			case SDLK_UP: bonusSpeedY += BACKGROUND_SCROLL_SPEED; break;
			case SDLK_DOWN: bonusSpeedY -= BACKGROUND_SCROLL_SPEED / 2; break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
			case SDLK_UP: bonusSpeedY -= BACKGROUND_SCROLL_SPEED; break;
			case SDLK_DOWN: bonusSpeedY += BACKGROUND_SCROLL_SPEED / 2; break;
		}
	}
}

void Bonus::move()
{
	bonusY += bonusSpeedY;
	collider.x = bonusX;
	collider.y = bonusY;

	if (bonusY > SCREEN_HEIGHT + (this->distanceMultiply * DISTANCE_BETWEEN_BONUSES))
	{
		bonusX = ObjectGenerator::getRandomXPosition();
		bonusY = 0;
		collider.x = bonusX;
		collider.y = bonusY;

		if (!isActive())
		{
			active = true;
		}
	}
}
void Bonus::render(SDL_Renderer* gRenderer, Texture* bonusTexture)
{
	if (isActive())
	{
		bonusTexture->render(gRenderer, bonusX, bonusY);
	}
};

int Bonus::getPosX()
{
	return bonusX;
};

int Bonus::getPosY()
{
	return bonusY;
};

void Bonus::taken()
{
	active = false;
}

bool Bonus::isActive()
{
	return active;
}