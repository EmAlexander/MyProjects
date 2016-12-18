#ifndef __GAME_H_INCLUDED
#define __GAME_H_INCLUDED

#include <SDL.h>

namespace gameNSpace
{
	bool init();

	bool handleEvents(SDL_Event e);

	bool update();

	void close();
}

#endif //__GAME_H_INCLUDED