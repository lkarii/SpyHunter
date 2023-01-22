#pragma once
#include<string.h>
#include "Texture.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}


class Car
{
public:
	int const SCREEN_WIDTH = 700;
	int const SCREEN_HEIGHT = 750;
	int const CAR_HEIGHT = 76;
	int const CAR_WIDTH = 50;
	int const CAR_SPEED = 3;
	int score;
	int carX, carY;
	bool leftDown, rightDown;
	int carSpeedX, carSpeedY;
	int carLives = 2;
	bool alive = true;
	SDL_Rect collider;
	bool canScored = true;
	int interval = 0;

	Car();
	~Car();

	void collisionWithNeutral();
	void checkInterval();
	void move();

	void handleEvent(SDL_Event& e);
	void render(SDL_Renderer* gRenderer, Texture* CarTexture);

	int getScore();
	int setScore(int score);
	int getX();
	int getY();
	int getLives();
	void killed();
	bool isAlive();
private:
	
};