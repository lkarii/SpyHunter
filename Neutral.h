#pragma once

#include<math.h>
#include<stdio.h>
#include<string.h>
#include <cstdlib>
#include "Texture.h"
#include "ObjectGenerator.h"
#include "CollidedObject.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

class Neutral : public CollidedObject
{
public:
	int const SCREEN_WIDTH = 700;
	int const SCREEN_HEIGHT = 750;
	int const CAR_WIDTH = 50;
	int const CAR_HEIGHT = 76;
	int const BACKGROUND_SCROLL_SPEED = 2;
	int const SPACE_BW_CARS = 550;

	int notEnemyX, notEnemyY, notEnemySpeedX, notEnemySpeedY, enemyLives;
	static const int BASE_NOT_ENEMY_SPEED = 2;
	SDL_Rect collider;

	Neutral();

	~Neutral();

	void handleEvent(SDL_Event& e);

	void move();
	void render(SDL_Renderer* gRenderer, Texture* notEnemy);
	int getPosX();
	int getPosY();
	int getLives();

private:
	bool isEnemy = false;
	SDL_Rect mCollider;
};