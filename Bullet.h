#pragma once
#include<math.h>
#include<stdio.h>
#include<string.h>
#include <cstdlib>
#include "CollidedObject.h"
#include "ObjectGenerator.h"
#include "Texture.h"
#include "Car.h"
#include "Bonus.h"
#include "Enemy.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

class Bullet
{
public:
	int const SCREEN_WIDTH = 700;
	int const SCREEN_HEIGHT = 750;
	int const CAR_WIDTH = 50;
	int const CAR_HEIGHT = 76;
	int const BACKGROUND_SCROLL_SPEED = 2;
	int const BULLET_SPEED = 10;
	int const BULLET_WIDTH = 16;
	int const BULLET_HEIGHT = 28;
	int const CAR_SPEED = 3;

	bool active = false;
	int index;
	int bulletX;
	SDL_Rect collider;

	Bullet(Car car);
	~Bullet();

	void render(SDL_Renderer* gRenderer, Texture* BulletTexture, int X, int Y);
	void shoot(Car car, Bonus weaponBonus);
	void move();
	void handleEvent(SDL_Event& e, Car car, Bonus weaponBonus);
	void enemyShoot(Enemy enemy);
	void moveEnemyBullet();
	int shotChance();
	int getX();
	int getY();
	int getShotChance();
	bool isActive();
private:
	int bulletY, bulletSpeedX, bulletSpeedY;
};