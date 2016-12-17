/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#ifndef LSLIDER_H_INCLUDED
#define LSLIDER_H_INCLUDED

#include "LTexture.h"
#include "LButton.h"

class LSlider
{
public:
	LSlider();
	~LSlider();

	int getValue();

	void handleEvents(SDL_Event* e);
	void setPos(SDL_Rect pos);
	void setTextures(string endTxt, string lineTxt, SDL_Renderer* mren);
	void render(SDL_Renderer* mren);
	void setSValue(int input);
	void setClips(SDL_Rect clip1, SDL_Rect clip2); 
	void free();

	SDL_Rect getSliderPos();
	SDL_Rect getRenderC1();
	SDL_Rect getRenderC2();

	int getEndWidth();

	LButton dash;
private:

	void findValue();

	LTexture line;
	LTexture endTxts;

	bool update;
	int drawing;

	SDL_Rect sliderPos;
	SDL_Rect renderClips[2];

	int sValue;
};

#endif