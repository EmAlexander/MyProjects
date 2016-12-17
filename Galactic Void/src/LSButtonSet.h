/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#ifndef __LSBUTTONSET_H_INCLUDED
#define __LSBUTTONSET_H_INCLUDED

#include <iostream>
#include "LButton.h"

using namespace std;

class LSButtonSet
{
public:
	LSButtonSet();
	~LSButtonSet();

	void setMaxButtons(int max);
	void handleEvents(SDL_Event* e);
	void loadTextures(string buttonTxt, SDL_Renderer* mren);
	void setClips(SDL_Rect clip1, SDL_Rect clip2, SDL_Rect clip3, SDL_Rect clip4);
	void setPos(int _x, int _y);
	void render(SDL_Renderer* mren);
	void free();
	int getCurrent();

	SDL_Rect getDiment1();
	SDL_Rect getDiment2();
	SDL_Rect getDiment3();
private:
	LButton buttons[15];
	int maxButtons;
	int current = 0;
};

#endif //__LSBUTTONSET_H_INCLUDED