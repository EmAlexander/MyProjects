/*
Version: 0.1
Last changed: 5/7/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

//main.cpp defines the main pipeline for OregonTrail Game.exe

#include <SDL.h>
#include <lua.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <vector>
#include <memory>

#include "conio.h"
#include "game.h"
#include "LTimer.h"
#include "modbase.hpp"

using namespace std;

game* g_game = new game;

LTimer capTimer;

const int FPS = 60;
int SCREEN_TICK_PER_FRAME = 1000 / FPS;

// Load the objects from the plugin folder.
//
// Takes as a parameter a reference to a list of modules,
// which will be emptied and then refilled with handles to
// the modules loaded. These should be freed with the
// FreeLibrary() after use.
//
// Returns a list of Base*, contained in a smart pointer
// to ease memory deallocation and help prevent memory
// leaks.
/*std::vector<std::unique_ptr<ModBase>> getPlugins(std::vector<HINSTANCE>& modules) {
	// A temporary structure to return.
	std::vector<std::unique_ptr<ModBase>> ret;
	// empty the modules list passed
	modules.clear();

	// Find the files contained in the 'plugins' folder
	WIN32_FIND_DATA fileData;
	HANDLE fileHandle = FindFirstFile(R"(mods\*.dll)", &fileData);

	if (fileHandle == (void*)ERROR_INVALID_HANDLE ||
		fileHandle == (void*)ERROR_FILE_NOT_FOUND) {
		// If we couldn't find any plugins, quit gracefully,
		// returning an empty structure.
		return std::vector<std::unique_ptr<ModBase>>();
	}

	// Loop over every plugin in the folder and store in our
	// temporary return list
	do {
		// Define the function types for what we are retrieving
		typedef std::unique_ptr<ModBase>(__cdecl *ObjProc)(void);
		typedef std::string(__cdecl *NameProc)(void);
		typedef void(__cdecl *InitProc)(void);

		// Load the library
		HINSTANCE mod = LoadLibrary((R"(mods\)" + std::string(fileData.cFileName)).c_str());

		if (!mod) {
			// Couldn't load the library, cleaning module list and quitting.
			for (HINSTANCE hInst : modules)
				FreeLibrary(hInst);
			throw std::runtime_error("Library " + std::string(fileData.cFileName) + " wasn't loaded successfully!");
		}

		// Get the function and the class exported by the DLL.
		// If you aren't using the MinGW compiler, you may need to adjust
		// this to cope with name mangling (I haven't gone into this here,
		// look it up if you want).
		ObjProc objFunc = (ObjProc)GetProcAddress(mod, "getObj");
		NameProc nameFunc = (NameProc)GetProcAddress(mod, "getName");
		InitProc initFunc = (InitProc)GetProcAddress(mod, "initDLL");

		if (!objFunc || !nameFunc || !initFunc)
			throw std::runtime_error("Invalid Plugin DLL: both 'getObj' , 'getName' and 'init' must be defined.");

		// push the objects and modules into our vectors
		ret.push_back(objFunc());
		modules.push_back(mod);

		clog << nameFunc() << endl;
		initFunc();

	} while (FindNextFile(fileHandle, &fileData));

	clog << endl;

	// Close the file when we are done
	FindClose(fileHandle);
	return ret;
}*/

//return a random string from a list of strings
string getRandStr()
{
	int r = rand() % 9 + 1;
	string returnStr;

	//create a ton of random nerd things
	switch (r)
	{
	case 1:
		returnStr = "Spacey things!";
		break;
	case 2:
		returnStr = "Like the Oregon Trail!";
		break;
	case 3:
		returnStr = "3.1415926% Death!";
		break;
	case 4:
		returnStr = "Herry has dysentery..";
		break;
	case 5:
		returnStr = "Explosions!";
		break;
	case 6:
		returnStr = "I\'m sorry, Dave. I\'m afraid I can\'t do that.";
		break;
	case 7:
		returnStr = "It\'s a TRAP!";
		break;
	case 8:
		returnStr = "Mineing For Gold!";
		break;
	case 9:
		returnStr = "Get In The Ship!";
		break;
	case 10:
		returnStr = "Space For Dummies!";
		break;
	default:
		returnStr = "IMPOSSABREW";
		break;
	}
	return returnStr;
}

int main(int argc, char *argv[])
{
	B2_NOT_USED(argc);
	B2_NOT_USED(argv);

	//std::vector<HINSTANCE> modules;

	// Enter a block, to prevent the Base objects being
	// deallocated after the modules are freed (which will
	// cause your program to crash)
	{

		//random seed
		srand(time(0));
		string name = getRandStr();
		//init game
		if (!g_game->g_init(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOW_SHOWN))
		{
			//best error ever
			cout << "well shit..." << endl;
			_getch();
			return 0;
		}
		else
		{
			//load media
			if (!g_game->g_loadMedia())
			{
				//throw this
				cout << "well shit..." << endl;
				_getch();
				return 0;
			}
			else
			{
				//std::vector<std::unique_ptr<ModBase>> objs;
				// Load the plugins using our function
				/*try {
					objs = getPlugins(modules);
				}
				catch (const std::exception& e) {
					std::cerr << "Exception caught: " << e.what() << std::endl;
					_getch();
					return 1;
				}
				*/
				//successful load!
				cout << "-------------------------\nSuccessfully loaded!" << endl;

				while (!g_game->g_getStop())
				{
					//start captimer
					capTimer.start();

					//handle events and update program
					g_game->g_handleEvents();
					g_game->g_update();

					//do prerender and render stuff
					g_game->g_preRender();
					g_game->g_render();

					//run modd's update and render
					/*for (auto& x : objs) 
					{
						x->update();
						x->render();
					}*/

					//render
					g_game->g_aftRender();

					//cap the game around 60 fps
					if (!g_game->getFastLoop())
					{
						int frameTicks = capTimer.getTicks();
						if (frameTicks < SCREEN_TICK_PER_FRAME)
						{
							SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
						}
					}
				}
			}
		}

		//output to a file
		g_game->getFlieO();

		//close program
		/*for (HINSTANCE hInst : modules)
			FreeLibrary(hInst);*/

		delete g_game;
		g_game = NULL;
		return 0;
	}
}
