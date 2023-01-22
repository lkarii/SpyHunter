#pragma once
#include<math.h>
#include<stdio.h>
#include<string.h>
#include <cstdlib>
#include "CollidedObject.h"
#include "ObjectGenerator.h"
#include "Texture.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

class Enemy : public CollidedObject
{
public:
	int const SCREEN_WIDTH = 700;
	int const SCREEN_HEIGHT = 750;
	int const CAR_HEIGHT = 76;
	int const CAR_WIDTH = 50;
	int const CAR_SPEED = 3;
	int const BACKGROUND_SCROLL_SPEED = 2;
	int enemyX, enemyY, enemySpeedX, enemySpeedY, enemyLives;
	static const int BASE_ENEMY_SPEED = 3;
	SDL_Rect collider;
	
	Enemy();
	~Enemy();

	void handleEvent(SDL_Event& e);
	void move();
	//Shows the enemy on the screen
	void render(SDL_Renderer* gRenderer, Texture* enemyTexture);
	int getPosX();
	int getPosY();
	int getLives();

private:
	bool isAnemy = true;
	SDL_Rect mCollider;
};