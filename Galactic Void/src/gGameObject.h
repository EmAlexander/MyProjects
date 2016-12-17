#ifndef __GGAMEOBJECT_H_INCLUDED
#define __GGAMEOBJECT_H_INCLUDED

#include "LTexture.h"

class gGameObject
{
public:
	struct txtInfo
	{
		SDL_Rect* clip;
		SDL_Point* center;
		double rotation;
		SDL_RendererFlip flip;
	};

	gGameObject();
	~gGameObject();

	void setPos(int x, int y);
	void render(SDL_Renderer* mren, SDL_Rect g_camera, txtInfo rotationVal);
	SDL_Rect getPos();

	LTexture gObj_txt;
private:
	SDL_Rect gObj_pos;
};

#endif //__GGAMEOBJECT_H_INCLUDED