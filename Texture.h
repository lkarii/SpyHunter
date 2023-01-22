#pragma once
#include<math.h>
#include<stdio.h>
#include<string.h>
#include <cstdlib>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}


class Texture
{
public:
	
	Texture();
	~Texture();

	bool Texture::CreateFromSprite(SDL_Renderer* gRenderer, SDL_Surface* sprite);
	void UpdateTexture(SDL_Renderer* gRenderer, SDL_Surface* sprite);
	void free();
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);
	void render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	int getWidth();
	int getHeight();

private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};