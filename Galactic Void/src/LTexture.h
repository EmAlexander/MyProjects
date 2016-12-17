/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#ifndef LTEXTURE_H_INCLUDED
#define LTEXTURE_H_INCLUDED

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <string>

using namespace std;

class LTexture
{
public:
	LTexture();
	~LTexture();
	void loadFromFile(string file, SDL_Renderer* renderer);
	void loadFromTtf(string text, TTF_Font *font, SDL_Color color, SDL_Renderer* g_renderer);
	void render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip = NULL, double angle = 0, SDL_Point *cPoint = NULL, SDL_RendererFlip flipTag = SDL_FLIP_NONE);
	void setColor(Uint8 r, Uint8 g, Uint8 b);
	void setBlendMode(SDL_BlendMode mode);
	void setAlpha(Uint8 a);
	int getWidth();
	int getHeight();

	void free();
private:
	SDL_Texture* mTexture;
	int mHeight;
	int mWidth;
};

#endif
