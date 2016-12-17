/*
Version: 0.1
Last changed: 5/11/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#include "LButton.h"

LButton::LButton()
{
	BUTTON_POS.x = 0;
	BUTTON_POS.y = 0;
	currentSprite = 0;
}

LButton::~LButton()
{
	free();
}

void LButton::setPos(int x, int y)
{
	BUTTON_POS.x = x;
	BUTTON_POS.y = y;
}

void LButton::handleEvent(SDL_Event *e)
{
	if (!stuck)
	{
		if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
		{
			mouseDown = false;
			mouseUp = false;
			mouseOver = false;
			mouseDown2 = false;
			mouseUp2 = false;

			int x, y;
			SDL_GetMouseState(&x, &y);
			bool inside = true;

			if (x < BUTTON_POS.x)
			{
				inside = false;
			}
			else if (x > BUTTON_POS.x + BUTTON_WIDTH)
			{
				inside = false;
			}
			else if (y < BUTTON_POS.y)
			{
				inside = false;
			}
			else if (y > BUTTON_POS.y + BUTTON_HEIGHT)
			{
				inside = false;
			}

			if (!inside)
			{
				currentSprite = 0;
				hoverSaf = false;
			}
			else
			{
				switch (e->type)
				{
				case SDL_MOUSEMOTION:
					currentSprite = 1;
					mouseOver = true;
					update = true;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (update)
					{
						if (e->button.button == SDL_BUTTON_RIGHT)
						{
							mouseDown2 = true;
						}
						else if (e->button.button == SDL_BUTTON_LEFT)
						{
							currentSprite = 2;
							mouseDown = true;
						}
						update = false;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (!update)
					{
						if (e->button.button == SDL_BUTTON_RIGHT)
						{
							mouseUp2 = true;
						}
						else if (e->button.button == SDL_BUTTON_LEFT)
						{
							currentSprite = 3;
							mouseUp = true;
						}
						update = true;
					}
					break;
				}
			}
		}
	}
	else
	{
		currentSprite = 2;
	}

	if (mouseOver && sounds && !hoverSaf && !mouseDown)
	{
		Mix_PlayChannel(-1, hoverS, 0);
		hoverSaf = true;
	}
}

void LButton::loadFromfile(string file, SDL_Renderer* ren)
{
	BUTTON_TEXTURE.loadFromFile(file, ren);
	BUTTON_WIDTH = BUTTON_SPRITES[0].w;
	BUTTON_HEIGHT = BUTTON_SPRITES[0].h;
}

void LButton::render(SDL_Renderer* ren)
{
	BUTTON_TEXTURE.render(BUTTON_POS.x, BUTTON_POS.y, ren, &BUTTON_SPRITES[currentSprite], 0, NULL, SDL_FLIP_NONE);
}

void LButton::setClips(SDL_Rect clip1, SDL_Rect clip2, SDL_Rect clip3, SDL_Rect clip4)
{
	BUTTON_SPRITES[0] = clip1;
	BUTTON_SPRITES[1] = clip2;
	BUTTON_SPRITES[2] = clip3;
	BUTTON_SPRITES[3] = clip4;
}

bool LButton::getUp()
{
	return mouseUp;
}

bool LButton::getDown()
{
	return mouseDown;
}

bool LButton::getOver()
{
	return mouseOver;
}

int LButton::getHeight()
{
	return BUTTON_HEIGHT;
}

int LButton::getWidth()
{
	return BUTTON_WIDTH;
}

int LButton::getX()
{
	return BUTTON_POS.x;
}

int LButton::getY()
{
	return BUTTON_POS.y;
}

void LButton::free()
{
	BUTTON_TEXTURE.free();
	mouseDown = false;
	mouseOver = false;
	mouseUp = false;

	if (hoverS != NULL)
	{
		Mix_FreeChunk(hoverS);
		hoverS = NULL;
	}
}

bool LButton::getUp2()
{
	return mouseUp2;
}

bool LButton::getDown2()
{
	return mouseDown2;
}

void LButton::makeStuck(bool _stuck)
{
	stuck = _stuck;
	mouseDown = false;
}

bool LButton::getStuck()
{
	return stuck;
}

void LButton::setSound(string hover, int volume)
{
	sounds = true;
	sound_hover = hover;

	hoverS = Mix_LoadWAV(hover.c_str());
	if (hoverS == NULL)
	{
		cout << "Unable to load hoverS sound effect! " << Mix_GetError() << endl;
		sounds = false;
	}
	else
	{
		Mix_VolumeChunk(hoverS, volume);
	}
}