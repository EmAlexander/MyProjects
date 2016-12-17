/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#ifndef LTOGGLEBUTTON_H_INCLUDED
#define LTOGGLEBUTTON_H_INCLUDED

#include "LButton.h"

class LToggleButton
{
public:
	LToggleButton();
	~LToggleButton();

	void handleEvents(SDL_Event* e);
	void free();
	void loadTextures(string buttonTxt, SDL_Renderer* mRen);
	void render(SDL_Renderer *mRen);
	/*
	clip1 defines when toggle is true
	clip2 defines when toggle is false
	*/
	void setClips(SDL_Rect clip1, SDL_Rect clip2);

	LButton button;

	bool getToggled();
	bool getUpdate();

	void setToggled(bool enter);
private:
	LTexture toggleTxt;

	SDL_Rect toggleClips[2];

	bool toggled = false;
	bool pressed = false;
	bool update = true;
};

#endif