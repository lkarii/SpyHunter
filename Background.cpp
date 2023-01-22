#include "Background.h"

Background::Background(Texture* bgTexture)
{
	_bgTexture = bgTexture;
}

Background::~Background()
{

};

void Background::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: scrollSpeed += BACKGROUND_SCROLL_SPEED; break;
		case SDLK_DOWN: scrollSpeed -= BACKGROUND_SCROLL_SPEED; break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: scrollSpeed -= BACKGROUND_SCROLL_SPEED; break;
		case SDLK_DOWN: scrollSpeed += BACKGROUND_SCROLL_SPEED; break;
		}
	}
}

void Background::render(SDL_Renderer* gRenderer)
{
	_bgTexture->render(gRenderer, 0, scrollingOffset);
	_bgTexture->render(gRenderer, 0, scrollingOffset - _bgTexture->getHeight());

};

void Background::scroll()
{
	scrollingOffset += scrollSpeed;
	if (scrollingOffset > _bgTexture->getHeight())
	{
		scrollingOffset -= _bgTexture->getHeight();
	}
}

int Background::getSpeed()
{
	return scrollSpeed;
}