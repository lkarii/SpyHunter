#include "Texture.h"

Texture::Texture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
};

Texture::~Texture()
{
	free();
};

bool Texture::CreateFromSprite(SDL_Renderer* gRenderer, SDL_Surface* sprite)
{
	free();

	SDL_Texture* newTexture = NULL;

	if (sprite == NULL)
	{
		printf("Unable to load bmp! SDL_image Error: \n");
	}
	else
	{
		SDL_SetColorKey(sprite, SDL_TRUE, SDL_MapRGB(sprite->format, 0, 0xFF, 0xFF));

		newTexture = SDL_CreateTextureFromSurface(gRenderer, sprite);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from BMP SDL Error");
		}
		else
		{
			mWidth = sprite->w;
			mHeight = sprite->h;
		}

		SDL_FreeSurface(sprite);
	}

	mTexture = newTexture;
	return mTexture != NULL;
};
void Texture::UpdateTexture(SDL_Renderer* gRenderer, SDL_Surface* sprite) {
	SDL_UpdateTexture(mTexture, NULL, sprite->pixels, sprite->pitch);
}

void Texture::Texture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
};

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(mTexture, red, green, blue);
};

void Texture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
};

void Texture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void Texture::render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
};

int Texture::getWidth()
{
	return mWidth;
};

int Texture::getHeight()
{
	return mHeight;
};