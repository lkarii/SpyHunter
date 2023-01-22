#include "Bullet.h"

Bullet::Bullet(Car car)
{
	bulletX = -100;
	bulletY = -100;
	bulletSpeedX = BULLET_SPEED;
	bulletSpeedY = BULLET_SPEED;

	collider.w = BULLET_WIDTH;
	collider.h = BULLET_HEIGHT;
};

Bullet::~Bullet()
{

};

void Bullet::render(SDL_Renderer* gRenderer, Texture* BulletTexture, int X, int Y)
{
	if (isActive())
	{
		BulletTexture->render(gRenderer, bulletX, bulletY);
	}
};

void Bullet::shoot(Car car, Bonus weaponBonus)
{
	active = true;
	bulletY -= bulletSpeedY;
	collider.x = bulletX;
	collider.y = bulletY;

	if (bulletY + BULLET_HEIGHT > SCREEN_HEIGHT)
	{
		bulletY -= bulletSpeedY;
	}

	if (bulletY < SCREEN_HEIGHT / 3)
	{
		if (weaponBonus.isActive() == false)
		{
			if (bulletY < 0)
			{
				active = false;
				bulletY = car.getY();
				bulletX = car.getX();
			}
		}
		else
		{
			active = false;
			bulletY = car.getY();
			bulletX = car.getX();
		}
	}
}

void Bullet::move()
{
	collider.x = bulletX;
	collider.y = bulletY;

	if ((bulletX < 0) || (bulletX + BULLET_WIDTH > SCREEN_WIDTH))
	{
		collider.x = bulletX;
		collider.y = bulletY;
	}
}

void Bullet::handleEvent(SDL_Event& e, Car car, Bonus weaponBonus)
{
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_SPACE: shoot(car, weaponBonus); break;
		}
	}
}


void Bullet::enemyShoot(Enemy enemy)
{
	active = true;
	bulletY += bulletSpeedY;
	collider.x = bulletX;
	collider.y = bulletY;

	if (bulletY + BULLET_HEIGHT < SCREEN_HEIGHT)
	{
		bulletY += bulletSpeedY;
	}

	if (bulletY > SCREEN_HEIGHT)
	{
		active = false;
		bulletY = enemy.getPosY();
		bulletX = enemy.getPosX();
	}
}

void Bullet::moveEnemyBullet()
{
	bulletY += bulletSpeedY;
	collider.x = bulletX;
	collider.y = bulletY;

	if ((bulletX > SCREEN_HEIGHT) || (bulletX + BULLET_WIDTH > SCREEN_WIDTH))
	{
		collider.x = bulletX;
		collider.y = bulletY;
	}
}

int Bullet::shotChance()
{
	index = rand() / 150;
	return index;
}

int Bullet::getX()
{
	return bulletX;
}

int Bullet::getY()
{
	return bulletY;
}

int Bullet::getShotChance()
{
	return index;
}

bool Bullet::isActive()
{
	return active;
}