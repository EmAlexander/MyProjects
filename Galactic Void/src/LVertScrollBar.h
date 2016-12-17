#ifndef __LVERTSCROLLBAR_H_INCLUEDED
#define __LVERTSCROLLBAR_H_INCLUEDED

#include "LTexture.h"
#include "LMenu.h"

class LVertScrollBar
{
public:
	LVertScrollBar();
	~LVertScrollBar();

	void handleEvents(SDL_Event *e);
	void render(SDL_Renderer *mren);
	void setLim(SDL_Point topLimit, SDL_Point botLimit);
	double getValue();
	void free();

	LMenu bar;
private:
	double value;
	SDL_Point lim1, lim2;
	SDL_Rect colBox;

	bool over = false;
	bool down = false;
};

#endif //__LVERTSCROLLBAR_H_INCLUEDED