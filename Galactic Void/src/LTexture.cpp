/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#include "LTexture.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <string>

using namespace std;

LTexture::LTexture()
{
	mTexture = NULL;
}

LTexture::~LTexture()
{
	free();
}


void LTexture::loadFromFile(string file, SDL_Renderer* renderer)
{
	free();
	SDL_Surface* tempSurface = IMG_Load(file.c_str());
	SDL_Texture* returnTexture = NULL;

	returnTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	if (returnTexture == NULL)
	{
		cout << "Unable to load texture: " << file.c_str() << " " << IMG_GetError() << endl;
	}

	mHeight = tempSurface->h;
	mWidth = tempSurface->w;

	SDL_FreeSurface(tempSurface);
	mTexture = returnTexture;
}

void LTexture::loadFromTtf(string text, TTF_Font *font, SDL_Color color, SDL_Renderer* g_renderer)
{
	free();
	SDL_Surface* tffSurf = NULL;

	if (font != NULL)
	{
		tffSurf = TTF_RenderText_Solid(font, text.c_str(), color);

		mTexture = SDL_CreateTextureFromSurface(g_renderer, tffSurf);
		if (mTexture == NULL)
		{
			cout << "Unable to create ttf: " << TTF_GetError() << endl;
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Texture creation error", string("Could not create ttf texture\nSDL error: " + string(SDL_GetError())).c_str(), NULL);
		}
		else
		{
			mWidth = tffSurf->w;
			mHeight = tffSurf->h;
		}

		SDL_FreeSurface(tffSurf);
	}
	else
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Texture creation error", string("Font passed is NULL\nSDL error: " + string(SDL_GetError())).c_str(), NULL);
}

void LTexture::render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip, double angle, SDL_Point *cPoint, SDL_RendererFlip flipTag)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if (clip != NULL)
	{
		renderQuad.h = clip->h;
		renderQuad.w = clip->w;
	}

	SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, cPoint, flipTag);
}

void LTexture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mHeight = 0;
		mWidth = 0;
	}
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

void LTexture::setColor(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetTextureColorMod(mTexture, r, g, b);
}

void LTexture::setBlendMode(SDL_BlendMode mode)
{
	SDL_SetTextureBlendMode(mTexture, mode);
}

void LTexture::setAlpha(Uint8 a)
{
	SDL_SetTextureAlphaMod(mTexture, a);
}