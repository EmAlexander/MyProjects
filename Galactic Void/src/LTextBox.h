/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#ifndef __LTEXTBOX_H_INCLUDED
#define __LTEXTBOX_H_INCLUDED

#include "VTextInput.h"
#include "LMenu.h"

class LTextBox
{
public:
	LTextBox();
	~LTextBox();

	void loadStartText(string defaults, TTF_Font* font, SDL_Renderer* renderer);
	void render(SDL_Renderer* renderer);
	void handleEvents(SDL_Event* e, SDL_Renderer* renderer, TTF_Font* font);
	void setDefaultString(string textString);
	string getText();
	void setCharLim(int lim);
	void setSelected(bool sec);
	void setText(string tx);

	//use insted of box.setMenuDiment
	void setPos(SDL_Rect renderPos);

	//use setpos not box.setMenuDiment
	LMenu box;
private:
	LTexture boxText;
	VTextInput txtI;
	SDL_Rect renderQuad;

	bool selected = false;

	int charLimit;
};

#endif