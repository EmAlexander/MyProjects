/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#include "LSButtonSet.h"


LSButtonSet::LSButtonSet()
{
}


LSButtonSet::~LSButtonSet()
{
}

void LSButtonSet::setMaxButtons(int max)
{
	maxButtons = max;
}

void LSButtonSet::handleEvents(SDL_Event* e)
{
	for (int i = 0; i < maxButtons; i++)
	{
		buttons[i].handleEvent(e);
		if (buttons[i].getDown())
		{
			buttons[i].makeStuck(true);
			current = i;
		}
		if (i != current)
		{
			buttons[i].makeStuck(false);
		}
	}
}

void LSButtonSet::setClips(SDL_Rect clip1, SDL_Rect clip2, SDL_Rect clip3, SDL_Rect clip4)
{
	for (int i = 0; i < maxButtons; i++)
	{
		buttons[i].setClips(clip1, clip2, clip3, clip4);
	}
}

void LSButtonSet::render(SDL_Renderer* mren)
{
	for (int i = 0; i < maxButtons; i++)
	{
		buttons[i].render(mren);
	}
}

void LSButtonSet::setPos(int _x, int _y)
{
	for (int i = 0; i < maxButtons; i++)
	{
		buttons[i].setPos(_x + (buttons[i].getWidth() * i), _y);
	}
}

void LSButtonSet::loadTextures(string buttonTxt, SDL_Renderer* mren)
{
	for (int i = 0; i < maxButtons; i++)
	{
		buttons[i].loadFromfile(buttonTxt, mren);
	}
}

void LSButtonSet::free()
{
	for (int i = 0; i < maxButtons; i++)
	{
		buttons[i].free();
	}
}

int LSButtonSet::getCurrent()
{
	return current;
}

SDL_Rect LSButtonSet::getDiment1()
{
	return{ buttons[0].getX(), buttons[0].getY(), buttons[0].getWidth(), buttons[0].getHeight() };
}

SDL_Rect LSButtonSet::getDiment2()
{
	return{ buttons[1].getX(), buttons[1].getY(), buttons[1].getWidth(), buttons[1].getHeight() };
}

SDL_Rect LSButtonSet::getDiment3()
{
	return{ buttons[2].getX(), buttons[2].getY(), buttons[2].getWidth(), buttons[2].getHeight() };
}
