/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#include "LSlider.h"


LSlider::LSlider()
{
	update = false;
}


LSlider::~LSlider()
{
	free();
}

int LSlider::getValue()
{
	return sValue;
}

void LSlider::setPos(SDL_Rect pos)
{
	sliderPos = pos;
}

void LSlider::setTextures(string endTxt, string lineTxt, SDL_Renderer* mren)
{
	line.free();
	endTxts.free();
	endTxts.loadFromFile(endTxt, mren);
	line.loadFromFile(lineTxt, mren);
}

void LSlider::setSValue(int input)
{
	sValue = input;
	int _aDis = 0;
	int _scale = sliderPos.w / 100;
	if (input <= 0)
	{
		_aDis = 0 + sliderPos.x;
	}
	else
	{
		_aDis = (input * _scale) + sliderPos.x;
	}
	dash.setPos(_aDis, ((sliderPos.h - dash.getHeight()) / 2) + sliderPos.y);
}

void LSlider::handleEvents(SDL_Event* e)
{
	int mouseX = 0;
	int mouseY = 0;

	SDL_GetMouseState(&mouseX, &mouseY);

	dash.handleEvent(e);
	bool inside = true;

	if (mouseX < sliderPos.x)
	{
		inside = false;
	}
	else if (mouseX > sliderPos.x + sliderPos.w)
	{
		inside = false;
	}
	else if (mouseY < sliderPos.y)
	{
		inside = false;
	}
	else if (mouseY > sliderPos.y + sliderPos.h)
	{
		inside = false;
	}

	switch (e->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (inside)
		{
			drawing = 1;
			dash.setPos(e->motion.x, ((sliderPos.h - dash.getHeight()) / 2) + sliderPos.y);

			if (dash.getX() >= sliderPos.x + sliderPos.w)
			{
				dash.setPos(sliderPos.x + sliderPos.w, ((sliderPos.h - dash.getHeight()) / 2) + sliderPos.y);
			}
			else if (dash.getX() <= sliderPos.x)
			{
				dash.setPos(sliderPos.x, ((sliderPos.h - dash.getHeight()) / 2) + sliderPos.y);
			}
			findValue();
		}
		break;
	case SDL_MOUSEMOTION:
		if (drawing == 1)
		{
			dash.setPos(e->motion.x, ((sliderPos.h - dash.getHeight()) / 2) + sliderPos.y);

			if (dash.getX() >= sliderPos.x + sliderPos.w)
			{
				dash.setPos(sliderPos.x + sliderPos.w, ((sliderPos.h - dash.getHeight()) / 2) + sliderPos.y);
			}
			else if (dash.getX() <= sliderPos.x)
			{
				dash.setPos(sliderPos.x, ((sliderPos.h - dash.getHeight()) / 2) + sliderPos.y);
			}
			findValue();
		}
		break;
	case SDL_MOUSEBUTTONUP:
		drawing = 0;
		findValue();
		break;
	}
}

void LSlider::render(SDL_Renderer* mren)
{
	if (drawing == 1 || drawing == 0)
	{
		double neededPxW = double(sliderPos.w) / double(75);

		renderClips[1].w = int(neededPxW) + 1;

		for (int i = 0; i <= 77; i++)
		{
			line.render(sliderPos.x + (neededPxW * i), ((renderClips[0].h - renderClips[1].h) / 2) + sliderPos.y, mren, &renderClips[1], NULL, NULL, SDL_FLIP_NONE);
		}
		dash.render(mren);
		endTxts.render(sliderPos.x - renderClips[0].w, sliderPos.y, mren, &renderClips[0], NULL, NULL, SDL_FLIP_NONE);
		endTxts.render((sliderPos.x + sliderPos.w) + dash.getWidth(), sliderPos.y, mren, &renderClips[0], NULL, NULL, SDL_FLIP_HORIZONTAL);
		dash.render(mren);
	}
}

void LSlider::setClips(SDL_Rect clip1, SDL_Rect clip2)
{
	renderClips[0] = clip1;
	renderClips[1] = clip2;
}

void LSlider::findValue()
{
	int _scale = sliderPos.w / 100;
	int _aDis = dash.getX() - sliderPos.x;
	int _value = _aDis / _scale;

	sValue = _value;
}

SDL_Rect LSlider::getSliderPos()
{
	return sliderPos;
}

SDL_Rect LSlider::getRenderC1()
{
	return renderClips[0];
}

SDL_Rect LSlider::getRenderC2()
{
	return renderClips[1];
}

int LSlider::getEndWidth()
{
	return endTxts.getWidth();
}

void LSlider::free()
{
	endTxts.free();
	line.free();
	dash.free();
}