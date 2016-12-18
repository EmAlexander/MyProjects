#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include <lua.hpp>
#include <iostream>

#include "game.h"

using namespace std;

/* the Lua interpreter */
lua_State *L;

bool stop = false;
SDL_Event e;

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

int wmain(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "Unable to init! " << SDL_GetError() << endl;
	}
	else
	{
		if (!(IMG_Init(IMG_INIT_PNG) &IMG_INIT_PNG))
		{
			cout << "Unable to init SDL_IMG! " << IMG_GetError() << endl;
		}
		else
		{
			if ((!(IMG_Init(IMG_INIT_JPG) &IMG_INIT_JPG)))
			{
				cout << "Unable to init SDL_IMG! " << IMG_GetError() << endl;
			}
			else
			{
				if (!gameNSpace::init())
				{
					cout << "Unable to init game!" << endl;
				}
				else
				{

					/*L = luaL_newstate();
					luaL_openlibs(L);
					if (luaL_loadfile(L, "runthis.lua")) {
					cerr << "Something went wrong loading the chunk (syntax error?)" << endl;
					cerr << lua_tostring(L, -1) << endl;
					lua_pop(L, 1);
					}
					else
					{
					luaL_dofile(L, "runthis.lua");
					}*/

					while (!stop)
					{
						float startTicks = SDL_GetTicks();

						while (SDL_PollEvent(&e) != 0)
						{
							if (!gameNSpace::handleEvents(e))
							{
								stop = true;
							}
						}
						if (!gameNSpace::update())
						{
							stop = true;
						}

						int frameTicks = SDL_GetTicks() - startTicks;
						if (frameTicks < SCREEN_TICKS_PER_FRAME)
						{
							SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
						}
					}
				}
			}
		}
	}

	gameNSpace::close();

	//lua_close(L);

	SDL_Quit();

	return 0;
}