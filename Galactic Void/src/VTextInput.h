/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#ifndef __VTEXTINPUT_H_INCLUDED
#define __VTEXTINPUT_H_INCLUDED

#include "SDL.h"
#include <iostream>
#include <string>

using namespace std;

class VTextInput
{
public:
	VTextInput();
	~VTextInput();

	string getText();
	void setText(string tx);
	void handleEvents(SDL_Event* e, unsigned int limit);
private:
	string text = "";

	bool shiftDown = false;
};

#endif