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

class Bonus
{
public:
	int const SCREEN_WIDTH = 700;
	int const SCREEN_HEIGHT = 750;
	int const CAR_WIDTH = 50;
	int const CAR_HEIGHT = 76;
	int const BACKGROUND_SCROLL_SPEED = 2;
	int const DISTANCE_BETWEEN_BONUSES = 1000;
	int bonusX, bonusY, bonusSpeedX, bonusSpeedY, enemyLives;
	static const int BASE_ENEMY_SPEED = 2;
	bool isTaken = false;
	SDL_Rect collider;

	Bonus(int distanceMultiply);
	~Bonus();

	void handleEvent(SDL_Event& e);
	void move();
	//Shows the enemy on the screen
	void render(SDL_Renderer* gRenderer, Texture* bonusTexture);;
	int getPosX();
	int getPosY();
	void taken();
	bool isActive();

private:
	bool active = true;
	int distanceMultiply = 1;
	SDL_Rect mCollider;
};