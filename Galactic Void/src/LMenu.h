/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#ifndef LMENU_H_INCLUDED
#define LMENU_H_INCLUDED

#include "LButton.h"
#include <iostream>

using namespace std;

class LMenu
{
public:
	LMenu();
	~LMenu();
	void loadTextures(string corner, string line, SDL_Renderer* mRen);
	void render(SDL_Renderer* mRen);
	/*
	clip1 is corner
	clip2 is line
	*/
	void setClips(SDL_Rect clip1, SDL_Rect clip2);
	void setMenuDiment(SDL_Rect _rq);
	SDL_Rect getDiment();
	void free();
	void setScreenWH(int screenw, int screenh);

	SDL_Rect getRenderClip1();
	SDL_Rect getRenderClip2();
private:
	LTexture mCorner;
	LTexture mLine;

	bool error = false;

	int SCREEN_HEIGHT;
	int SCREEN_WIDTH;

	SDL_Rect renderQuad;
	SDL_Rect renderClips[2];
};

#endif