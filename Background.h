#pragma once
#include<math.h>
#include<stdio.h>
#include<string.h>
#include <cstdlib>
#include "Texture.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

class Background
{
public:
	int const SCREEN_WIDTH = 700;
	int const SCREEN_HEIGHT = 750;
	int const BACKGROUND_SCROLL_SPEED = 2;
	int scrollSpeed = 3;
	int scrollingOffset = SCREEN_HEIGHT;
	SDL_Rect bgDST = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect srcrect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect dstrect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	Texture* _bgTexture;

	Background(Texture* bgTexture);
	~Background();


	void handleEvent(SDL_Event& e);
	void render(SDL_Renderer* gRenderer);
	void scroll();
	int getSpeed();


private:

};