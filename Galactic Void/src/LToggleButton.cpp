/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#include "LToggleButton.h"

LToggleButton::LToggleButton()
{
}


LToggleButton::~LToggleButton()
{
}

bool LToggleButton::getToggled()
{
	return toggled;
}

void LToggleButton::free()
{
	button.free();
	toggleTxt.free();
}

void LToggleButton::setClips(SDL_Rect clip1, SDL_Rect clip2)
{
	toggleClips[0] = clip1;
	toggleClips[1] = clip2;
}

void LToggleButton::loadTextures(string buttonTxt, SDL_Renderer* mRen)
{
	toggleTxt.free();
	toggleTxt.loadFromFile(buttonTxt, mRen);
}

void LToggleButton::render(SDL_Renderer *mRen)
{
	button.render(mRen);
	if (toggled)
	{
		toggleTxt.render(button.getX(), button.getY(), mRen, &toggleClips[0], NULL, NULL, SDL_FLIP_NONE);
	}
	else if (!toggled)
	{
		toggleTxt.render(button.getX(), button.getY(), mRen, &toggleClips[1], NULL, NULL, SDL_FLIP_NONE);
	}
}

void LToggleButton::handleEvents(SDL_Event* e)
{
	button.handleEvent(e);
	if (button.getDown() && !pressed)
	{
		toggled = !toggled;
		pressed = true;
		update = true;
	}
	else if (button.getUp())
	{
		pressed = false;
	}
	else
	{
		update = false;
	}
}

bool LToggleButton::getUpdate()
{
	return update;
}

void LToggleButton::setToggled(bool enter)
{
	toggled = enter;
}