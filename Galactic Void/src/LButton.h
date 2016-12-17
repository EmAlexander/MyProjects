/*
Version: 0.1
Last changed: 5/11/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#ifndef LBUTTON_H_INCLUDED
#define LBUTTON_H_INCLUDED

#include "SDL.h"
#include "SDL_mixer.h"
#include "vector.h"
#include "LTexture.h"

class LButton
{
public:
	LButton();
	~LButton();
	void setPos(int x, int y);
	void handleEvent(SDL_Event *e);
	void loadFromfile(string file, SDL_Renderer* ren);
	void render(SDL_Renderer* ren);
	bool getStuck();
	void free();

	void setSound(string hover, int volume);

	int getHeight();
	int getWidth();
	int getX();
	int getY();

	bool getUp();
	bool getDown();
	bool getOver();

	bool getUp2();
	bool getDown2(); 

	void makeStuck(bool _stuck);

	/*
	clip1 is when mouse is not inside of button
	clip2 is when mouse is inside of button
	clip3 is when mouse is down inside of button
	clip4 is when mouse is up inside of button
	*/
	void setClips(SDL_Rect clip1, SDL_Rect clip2, SDL_Rect clip3, SDL_Rect clip4);

protected:
	int BUTTON_WIDTH;
	int BUTTON_HEIGHT;
	AVE_vector BUTTON_POS;
	LTexture BUTTON_TEXTURE;

	string sound_hover;
	string sound_click;
	bool sounds = false;
	Mix_Chunk *hoverS = NULL;
	bool hoverSaf = false;

	bool update = false;

	bool mouseDown = false;
	bool mouseUp = false;
	bool mouseOver = false;

	bool mouseDown2 = false;
	bool mouseUp2 = false;

	bool stuck = false;

	int currentSprite;
	SDL_Rect BUTTON_SPRITES[4];
};

#endif