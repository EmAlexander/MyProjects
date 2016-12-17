/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#include "LTextBox.h"


LTextBox::LTextBox()
{
}


LTextBox::~LTextBox()
{
}

void LTextBox::setPos(SDL_Rect renderPos)
{
	box.setMenuDiment(renderPos);
	renderQuad = renderPos;
}

void LTextBox::handleEvents(SDL_Event* e, SDL_Renderer* renderer, TTF_Font* font)
{
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		bool inside = true;

		if (x < renderQuad.x)
		{
			inside = false;
		}
		else if (x > renderQuad.x + renderQuad.w)
		{
			inside = false;
		}
		else if (y < renderQuad.y)
		{
			inside = false;
		}
		else if (y > renderQuad.y + renderQuad.h)
		{
			inside = false;
		}
		if (!inside)
		{
			if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				if (e->button.button == SDL_BUTTON_LEFT)
				{
					selected = false;
				}
			}
		}
		else
		{
			if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				if (e->button.button == SDL_BUTTON_LEFT)
				{
					selected = true;
				}
			}
		}
	}

	if (selected)
	{
		txtI.handleEvents(e, charLimit);
		if (getText().length() == 0)
		{
			boxText.free();
			boxText.loadFromTtf(" ", font, { 255, 255, 255, 255 }, renderer);
		}
		if (getText().length() > 0)
		{
			boxText.free();
			boxText.loadFromTtf(getText(), font, { 255, 255, 255, 255 }, renderer);
		}
	}
}

string LTextBox::getText()
{
	string returnStr = txtI.getText();
	returnStr.erase(0, 1);
	return returnStr;
}

void LTextBox::render(SDL_Renderer* renderer)
{
	box.render(renderer);
	boxText.render(renderQuad.x +(renderQuad.w - boxText.getWidth()) / 2, renderQuad.y + (renderQuad.h - boxText.getHeight()) / 2, renderer, NULL, NULL, NULL, SDL_FLIP_NONE);
}

void LTextBox::setCharLim(int lim)
{
	charLimit = lim;
}

void LTextBox::setDefaultString(string textString)
{
	txtI.setText(textString);
}

void LTextBox::loadStartText(string defaults, TTF_Font* font, SDL_Renderer* renderer)
{
	boxText.free();
	boxText.loadFromTtf(defaults, font, { 255, 255, 255, 255 }, renderer);
}

void LTextBox::setSelected(bool sec)
{
	selected = sec;
}

void LTextBox::setText(string tx)
{
	txtI.setText(tx);
}