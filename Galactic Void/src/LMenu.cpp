/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#include "LMenu.h"


LMenu::LMenu()
{
}


LMenu::~LMenu()
{
	free();
}

void LMenu::setClips(SDL_Rect clip1, SDL_Rect clip2)
{
	renderClips[0] = clip1;
	renderClips[1] = clip2;
}

void LMenu::loadTextures(string corner, string line, SDL_Renderer* mRen)
{
	free();
	mCorner.loadFromFile(corner, mRen);
	mLine.loadFromFile(line, mRen);
}

void LMenu::free()
{
	mLine.free();
	mCorner.free();
}

void LMenu::setMenuDiment(SDL_Rect _rq)
{
	renderQuad = _rq;
}

void LMenu::render(SDL_Renderer* mRen)
{
	if (!error)
	{
		if (renderQuad.w >= SCREEN_WIDTH)
		{
			cout << "Renderquad is larger then screen width: defaulting to screen width" << endl;
			renderQuad.w = SCREEN_WIDTH;
			error = true;
		}
		if (renderQuad.h >= SCREEN_HEIGHT)
		{
			cout << "Renderquad is larger then screen height: defaulting to screen height" << endl;
			renderQuad.h = SCREEN_HEIGHT;
			error = true;
		}
	}

	//define basic values needed for rendering
	SDL_Point cornerC;
	cornerC.x = renderClips[0].w / 2;
	cornerC.y = renderClips[0].h / 2;

	//render corners
	mCorner.render(renderQuad.x, renderQuad.y, mRen, &renderClips[0], 0, &cornerC, SDL_FLIP_VERTICAL);
	mCorner.render((renderQuad.x + renderQuad.w) - renderClips[0].w, renderQuad.y, mRen, &renderClips[0], -90, &cornerC, SDL_FLIP_HORIZONTAL);
	mCorner.render(renderQuad.x, (renderQuad.y + renderQuad.h) - renderClips[0].h, mRen, &renderClips[0], 0, &cornerC, SDL_FLIP_NONE);
	mCorner.render((renderQuad.x + renderQuad.w) - renderClips[0].w, ((renderQuad.y + renderQuad.h) - renderClips[0].h) - 1, mRen, &renderClips[0], -90, &cornerC, SDL_FLIP_NONE);

	//render lines
	renderClips[1].w = 1;
	int npix = renderQuad.w - (mCorner.getWidth() * 2);
	for (int i = 0; i < npix; i++)
	{
		mLine.render(renderQuad.x + (i + mCorner.getWidth()), renderQuad.y, mRen, &renderClips[1], NULL, NULL, SDL_FLIP_NONE);
		mLine.render(renderQuad.x + (i + mCorner.getWidth()), (renderQuad.y + renderQuad.h) - renderClips[1].h, mRen, &renderClips[1], NULL, NULL, SDL_FLIP_NONE);
	}

	int npix2 = renderQuad.h - (renderClips[0].h * 2);
	SDL_Point rp;
	rp.x = 0;
	rp.y = 0;
	for (int i = 0; i < npix2; i++)
	{
		mLine.render((renderQuad.x + renderClips[1].h) - 1, renderQuad.y + i + renderClips[0].h, mRen, &renderClips[1], 90, &rp, SDL_FLIP_NONE);
		mLine.render(((renderQuad.x + renderClips[1].h) - 2) + (renderQuad.w - renderClips[1].h), renderQuad.y + i + renderClips[0].h, mRen, &renderClips[1], 90, &rp, SDL_FLIP_NONE);
	}
}

void LMenu::setScreenWH(int screenw, int screenh)
{
	SCREEN_HEIGHT = screenh;
	SCREEN_WIDTH = screenw;
}

SDL_Rect LMenu::getDiment()
{
	return renderQuad;
}

SDL_Rect LMenu::getRenderClip1()
{
	return renderClips[0];
}

SDL_Rect LMenu::getRenderClip2()
{
	return renderClips[1];
}