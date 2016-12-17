/*
Version: 1.2
Last changed: 6/29/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#include "game.h"

std::vector<std::string> &__split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> __split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	__split(s, delim, elems);
	return elems;
}

bool _checkColsion(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		//colsion
		return false;
	}

	if (topA >= bottomB)
	{
		//colsion
		return false;
	}

	if (rightA <= leftB)
	{
		//colsion
		return false;
	}

	if (leftA >= rightB)
	{
		//colsion
		return false;
	}

	//If none of the sides from A are outside B
	//no colsion
	return true;
}

game::game()
{
	//set default values for varbials
	mWindow = NULL;
	mRen = NULL;

	srand(time(0));

	stop = false;
}

game::~game()
{
	lua_close(L);

	//free all of the things!
	text[0].free();
	text[1].free();
	text[2].free();
	text[3].free();

	button[0].free();
	button[1].free();
	button[2].free();
	button[3].free();

	background.free();

	virsNumb.free();

	//free options menu
	options.free();
	optionsMSTxt.free();
	soundOTxt.free();
	vSyncTxt.free();
	vSyncToggle.free();
	optionsSS.free();
	optionsMS.free();
	optionsBack.free();
	optsBackTxt.free();

	//remove window/renderer
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRen);
	mWindow = NULL;
	mRen = NULL;

	//close fonts
	TTF_CloseFont(font);
	TTF_CloseFont(newG_font);
	TTF_CloseFont(newG_newgamefont);
	newG_font = NULL;
	font = NULL;
	newG_newgamefont = NULL;

	if (menuMusic != NULL)
	{
		Mix_FreeMusic(menuMusic);
		menuMusic = NULL;
	}

	//quit sdl and external libs
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool game::g_init(const char* wName, int xPos, int yPos, int tags)
{
	//create return value
	bool passed = true;

	getFileI();

	//init sdl
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		//if failed... jesus help us all
		cout << "Unable to init! " << SDL_GetError() << endl;
		passed = false;
	}
	else
	{
		//create window
		mWindow = SDL_CreateWindow(wName, xPos, yPos, SCREEN_WIDTH, SCREEN_HEIGHT, tags);
		if (mWindow == NULL)
		{
			//if failed... 
			cout << "Unable to create window! " << SDL_GetError() << endl;
			passed = false;
		}
		else
		{
			//create window with window
			if (!vSyncEnabled)
			{
				mRen = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
			}
			else
			{
				mRen = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			}

			if (mRen == NULL)
			{
				//if failed...
				cout << "Unable to create renderer! " << SDL_GetError() << endl;
				passed = false;
			}
			else
			{
				//init extern libs
				if (!(IMG_Init(IMG_INIT_PNG) &IMG_INIT_PNG))
				{
					cout << "Unable to init SDL_IMG! " << IMG_GetError() << endl;
					passed = false;
				}

				//init ttf
				if (TTF_Init() == -1)
				{
					cout << "Unable to init SDL_TTF! " << TTF_GetError();
					passed = false;
				}

				//Initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
					passed = false;
				}

				L = luaL_newstate();
				luaL_openlibs(L);

				lua_register(L, "__addHook", addHook);
				lua_register(L, "__removeHook", removeHook);
				lua_register(L, "__drawRect", drawRect);
				lua_register(L, "__drawLine", drawLine);
				lua_register(L, "__splitString", splitLuaString);
				lua_register(L, "__stringtoUpper", luaStringToUpper);
				lua_register(L, "__stringtoLower", luaStringToLower);
				lua_register(L, "setPlyPos", setPlyPosFLua);

				luaRen = mRen;

				//register hook.*
				luaL_dostring(L, "hook = { add = function(hookN, id, functionN) __addHook(hookN, id, functionN) return end, remove = function(hookN, id) __removeHook(hookN, id) return end }");

				//register draw.*
				luaL_dostring(L, "draw = { rectangle = function(x,y,w,h,r,g,b,filled) __drawRect(x,y,w,h,r,g,b,filled) return end, line = function(x1,y1,x2,y2,r,g,b) __drawLine(x1,y1,x2,y2,r,g,b) return end }");

				//register string.*
				luaL_dostring(L, "string = { split = function(str,inc) return __splitString(str,inc) end, upper = function(str) return __stringtoUpper(str) end, lower = function(str) return __stringtoLower(str) end }");

				WIN32_FIND_DATA modFile;
				HANDLE hFind = FindFirstFile(R"(mods\*)", &modFile);
				if (hFind != INVALID_HANDLE_VALUE)
				{
					do
					{
						string dir = modFile.cFileName;
						if ((modFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(dir == "." || dir == ".."))
						{
							modFiles.push_back(modFile.cFileName);
						}

					} while (FindNextFile(hFind, &modFile));

					FindClose(hFind);
				}
				
				for (int i = 0; i < modFiles.size(); i++)
				{
					string luaModFile = "mods/" + string(modFiles.at(i)) + "/main.lua";
					if (luaL_loadfile(L, luaModFile.c_str()))
					{
						cout << "Something went wrong loading the chunk (syntax error?)" << endl;
						cout << lua_tostring(L, -1) << endl;
						lua_pop(L, 1);
						cout << endl;
					}
					else
					{
						luaL_dofile(L, luaModFile.c_str());
					}
				}

				//init hook
				for (int i = 0; i < initHooks.size(); i++)
				{
					lua_getglobal(L, initHooks.at(i).functionN.c_str());
					lua_pcall(L, 0, 0, 0);
				}

				//set default render color
				SDL_SetRenderDrawColor(mRen, 255, 255, 255, 255);

				//load font
				font = TTF_OpenFont("data/ERASMD.TTF", 35);
			}
		}
	}
	return passed;
}

bool game::g_loadMedia()
{
	bool passed = true;

	//clickS = Mix_LoadWAV("data/wav/click.wav");
	/*if (clickS == NULL)
	{
		cout << "Unable to load clickS sound effect! " << Mix_GetError() << endl;
		passed = false;
	}*/

	// load main menu

	menuMusic = Mix_LoadMUS("data/wav/Orbitting Orion.mp3");
	if (menuMusic == NULL)
	{
		cout << "Unable to load menu music! " << Mix_GetError() << endl;
		passed = false;
	}
	else if (Mix_PlayingMusic() == 0)
	{
		Mix_VolumeMusic(musicVol);
		Mix_PlayMusic(menuMusic, -1);
	}

	windowIco = IMG_Load("icon.png");
	SDL_SetWindowIcon(mWindow, windowIco);
	SDL_FreeSurface(windowIco);

	/*testS.bar.setScreenWH(SCREEN_WIDTH, SCREEN_HEIGHT);
	testS.bar.setClips({ 0, 0, 17, 16 }, { 0, 17, 17, 2 });
	testS.bar.setMenuDiment({ 100, 100, 50, 100 });
	testS.bar.loadTextures("data/img/menutxt2.png", "data/img/menutxt2.png", mRen);
	SDL_Point ___pref = {100,100};
	SDL_Point __pref = {300 + 100, 300 + 100};
	testS.setLim(___pref, __pref);*/

	button[0].setSound("data/wav/hover1.wav", soundVol);
	button[0].setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
	button[0].loadFromfile("data/img/newbutton.png", mRen);
	button[0].setPos(35, (SCREEN_HEIGHT - button[0].getHeight()) - 15);
	text[0].loadFromTtf("Start Game", font, { 255, 255, 255, 255 }, mRen);

	button[1].setSound("data/wav/hover1.wav", soundVol);
	button[1].setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
	button[1].loadFromfile("data/img/newbutton.png", mRen);
	button[1].setPos((button[1].getWidth() * 1) + 105, (SCREEN_HEIGHT - button[1].getHeight()) - 15);
	text[1].loadFromTtf("Extras", font, { 255, 255, 255, 255 }, mRen);

	button[2].setSound("data/wav/hover1.wav", soundVol);
	button[2].setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
	button[2].loadFromfile("data/img/newbutton.png", mRen);
	button[2].setPos((button[2].getWidth() * 2) + 175, (SCREEN_HEIGHT - button[2].getHeight()) - 15);
	text[2].loadFromTtf("Options", font, { 255, 255, 255, 255 }, mRen);

	button[3].setSound("data/wav/hover1.wav", soundVol);
	button[3].setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
	button[3].loadFromfile("data/img/newbutton.png", mRen);
	button[3].setPos((button[3].getWidth() * 3) + 245, (SCREEN_HEIGHT - button[3].getHeight()) - 15);
	text[3].loadFromTtf("Quit", font, { 255, 255, 255, 255 }, mRen);

	titleFont = TTF_OpenFont("data/ERASDEMI.TTF", 80);
	menu_title.loadFromTtf("Galactic void", titleFont, { 255, 255, 255, 255 }, mRen);
	TTF_CloseFont(titleFont);

	background.loadFromFile("data/img/background.png", mRen);

	fpsTimer.start();
	fpsCounter.loadFromTtf("default", font, { 255, 255, 255, 255 }, mRen);

	version_Font = TTF_OpenFont("data/ERASDEMI.TTF", 31);
	virsNumb.loadFromTtf(VIRSION_NUMBER, version_Font, { 255, 255, 255, 255 }, mRen);
	TTF_CloseFont(version_Font);
	return passed;
}

void game::g_handleEvents()
{
	//handle main event poll
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			stop = true;
			_TGame.freeAll();
			freeAll();
		}
		if (e.type == SDL_WINDOWEVENT)
		{
			switch (e.window.event)
			{
			case SDL_WINDOWEVENT_MINIMIZED:
				minimized = true;
				break;
			case SDL_WINDOWEVENT_RESTORED:
				minimized = false;
				break;
			}
		}
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_F1:
				displayFps = !displayFps;
				break;
			}
		}
		if (currentMenu == 3)
		{
			optionsSS.handleEvents(&e);
			optionsMS.handleEvents(&e);
		}
		if (currentMenu == 1)
		{
			newG_saveName.handleEvents(&e, mRen, newG_font);
			newG_seed.handleEvents(&e, mRen, newG_font);
			newG_createGameButton.handleEvent(&e);
			newG_backButton.handleEvent(&e);
			loadG_menuDownButton.handleEvent(&e);
			newG_difficultyButton.handleEvents(&e);
			if (loadG_saveNames.size() > 0)
				loadG_loadBar.handleEvents(&e);
			for (int i = 0; i < loadG_saveNames.size(); i++)
			{
				//if (loadG_buttons[i].getY() >= loadG_box.y && loadG_buttons[i].getY() <= loadG_box.y + loadG_box.h)
				//{
				loadG_buttons[i].setPos(loadG_box.x + 0, loadG_box.y + 10 + (i * loadG_buttons[i].getHeight()) - ((loadG_loadBar.bar.getDiment().y - loadG_box.y) * /*1.66*/2));

				if (_checkColsion({ loadG_box.x + 10 + 0, loadG_box.y + 10, loadG_buttons[i].getWidth(), 330 }, { loadG_buttons[i].getX(), loadG_buttons[i].getY(), loadG_buttons[i].getWidth(), loadG_buttons[i].getHeight() }))
				{
					loadG_buttons[i].handleEvent(&e);
					if (loadG_buttons[i].getDown())
					{
						freeAll();
						currentMenu = 4;
						newG_keyframe = false;
						newG_keyframeDown = false;
						newG_keyframED = false;
						loadG_menuKey_down = false;
						loadG_menuKey_up = false;
						loadG_menuKey_upEd = false;

						Mix_HaltMusic();
						Mix_FreeMusic(menuMusic);
						menuMusic = NULL;
						_TGame.setScreenWH(SCREEN_WIDTH, SCREEN_HEIGHT);
						_TGame.startGame(loadG_saveNames.at(i), mRen, mWindow, VIRSION_NUMBER, font, false);
					}
				}
				//}
			}
		}
		if (currentMenu == 2)
		{
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && runCredits)
			{
				creditXpos = extras.getDiment().h - 200;
				runCredits = false;
				creditTexture.free();
			}
		}
		if (currentMenu == 4)
		{
			_TGame.handleEvents(&e, mRen, mWindow, L);
			if (_TGame.getBTT())
			{
				getFileI();
				_TGame.freeAll();
				currentMenu = 0;
				changeMenu(0);
				_TGame.setBBT(false);
				_TGame.setPause(false);

				menuMusic = Mix_LoadMUS("data/wav/Orbitting Orion.mp3");
				if (menuMusic == NULL)
				{
					cout << "Unable to load menu music! " << Mix_GetError() << endl;
				}
				else if (Mix_PlayingMusic() == 0)
				{
					Mix_VolumeMusic(musicVol);
					Mix_PlayMusic(menuMusic, -1);
				}
			}
		}
		if (currentMenu == 0)
		{
			//testS.handleEvents(&e);
		}
	}

	if (!minimized)
	{
		if (currentMenu == 1)
		{
			newGameButton.handleEvent(&e);
			loadGameButton.handleEvent(&e);
			gameBackButton.handleEvent(&e);
		}
		if (currentMenu == 0)
		{
			button[0].handleEvent(&e);
			button[1].handleEvent(&e);
			button[2].handleEvent(&e);
			button[3].handleEvent(&e);
		}
		if (currentMenu == 3)
		{
			vSyncToggle.handleEvents(&e);
			optionsBack.handleEvent(&e);
			optsSav.handleEvent(&e);
		}
	}
}

void game::g_update()
{
	float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
	if (avgFPS > 2000000)
	{
		avgFPS = 0;
	}

	string fpsString = to_string(avgFPS);
	vector<string> fpsV = __split(fpsString, '.');
	string aftP = fpsV.at(1);

	aftP.erase(aftP.begin() + 3, aftP.end());

	fpsCounter.loadFromTtf(fpsV.at(0) + "." + aftP, font, { 255, 255, 255, 255 }, mRen);
	//if (!minimized)
	//{
		if (currentMenu == 4)
		{
			_TGame.update(mRen,mWindow);
		}
		else if (currentMenu == 1)
		{
			// menu 1 update
			if (gameBackButton.getDown())
			{
				changeMenu(0);
			}
			else if (newGameButton.getDown())
			{
				//start animation (menu animation init)
				if (!newG_keyframED)
				{
					newG_keyframEDDown = false;
					newG_keyframe = true;
					newG_index = SCREEN_HEIGHT;
					newG_keyframED = true;
					newG_keyframeDown = false;
					newG_neededPos = (newGameButton.getY() - newG_box.h - 15) - 9;
					newG_box.x = newGameButton.getX();
					newG_multFact = 0.1;
				}
			}
			// load menu up init
			else if (loadGameButton.getDown())
			{
				if (!loadG_menuKey_upEd)
				{
					loadG_menuKey_downEd = false;
					loadG_menuKey_down = false;
					loadG_menuKey_upEd = true;
					loadG_menuKey_up = true;
					loadG_menuKey_index = loadG_box.y;
					loadG_menuKey_multFact = 0.1;
					loadG_menuKey_neededPos = (loadGameButton.getY() - loadG_box.h - 15);
				}
			}
			// load menu down init
			else if (loadG_menuDownButton.getDown())
			{
				if (!loadG_menuKey_downEd)
				{
					loadG_menuKey_downEd = true;
					loadG_menuKey_down = true;
					loadG_menuKey_upEd = false;
					loadG_menuKey_up = false;
					loadG_menuKey_index = loadG_box.y;
					loadG_menuKey_multFact = 0.1;
					loadG_menuKey_neededPos = SCREEN_HEIGHT + 9;
				}
			}
			//move new game menu down
			else if (newG_backButton.getDown())
			{
				if (!newG_keyframEDDown)
				{
					newG_keyframED = false;
					newG_keyframEDDown = true;
					newG_keyframeDown = true;
					newG_keyframe = false;
					newG_neededPos = SCREEN_HEIGHT + 9;
					newG_index = newG_box.y;
					newG_multFact = 0.1;
					newG_box.x = newGameButton.getX();
				}
			}
			else if (newG_createGameButton.getDown())
			{
				if (newG_saveName.getText() != "")
				{
					fstream save;
					stringstream fileSs;

					//create save file
					fileSs << "sav/" << newG_saveName.getText();
					string strAd = fileSs.str();
					_mkdir(strAd.c_str());
					string dir = newG_saveName.getText();

					string seed = "1234567891234567";

					if (newG_seed.getText().length() == 0)
					{
						for (int i = 0; i < 16; i++)
						{
							char ncam[90] = {
								'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E',
								'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
								'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '`', '~', '-', '_', '=', '+', '[', ']', '{', '}', ';', ':', '<', '>', ',', '.', '/', '?'
							};
							seed[i] = ncam[rand() % 90];
						}
					}
					else
					{
						seed = newG_seed.getText();
					}

					save.open("sav/" + newG_saveName.getText() + "/data.sav", ios::trunc | ios::out);
					save << newG_saveName.getText() << endl << newG_difficultyButton.getCurrent() << endl << seed << endl;
					save.close();

					currentMenu = 4;
					freeAll();

					newG_keyframe = false;
					newG_keyframeDown = false;
					newG_keyframED = false;
					loadG_menuKey_down = false;
					loadG_menuKey_up = false;
					loadG_menuKey_upEd = false;

					Mix_HaltMusic();
					Mix_FreeMusic(menuMusic);
					menuMusic = NULL;
					_TGame.setScreenWH(SCREEN_WIDTH, SCREEN_HEIGHT);
					_TGame.startGame(dir, mRen, mWindow, VIRSION_NUMBER, font, true);
				}
			}

			//non-button menu 1 block
			if (newG_keyframe)
			{
				//change box pos
				newG_box.y = newG_index;
				//increment index
				int posFromEnd = AVE_findM({ newG_box.x, newG_box.y }, { newG_box.x, newG_neededPos });
				newG_boxVelosity = newG_multFact*posFromEnd;

				newG_index -= newG_boxVelosity;

				if (newG_boxVelosity != 0)
				{
					newG_seedTxtPos.x = newG_box.x + (newG_box.w - newG_seedTxt.getWidth()) / 2;
					newG_seedTxtPos.y = newG_box.y + 30 + 50 + 5;

					newG_menuBox.setMenuDiment({ newG_box.x, newG_box.y, newG_box.w, newG_box.h });
					newG_saveName.setPos({ newG_box.x + (newG_box.w - (newG_box.w - 40)) / 2, newG_box.y + 30, newG_box.w - 40, 50 });
					newG_seed.setPos({ newG_box.x + (newG_box.w - (newG_box.w - 40)) / 2, newG_seedTxtPos.y + newG_seedTxt.getHeight() + 5, newG_box.w - 40, 50 });
					newG_createGameButton.setPos(newG_box.x + (newG_box.w - newG_createGameButton.getWidth()) / 2, newG_box.y + newG_box.h - newG_createGameButton.getHeight() - 15);

					newG_backButton.setPos((newG_box.x + newG_box.w), newG_box.y);

					newG_newGameTextPos.x = newG_box.x + (newG_box.w - newG_newGameText.getWidth()) / 2;
					newG_newGameTextPos.y = newG_box.y + 4;

					newG_difficultyButton.setPos(newG_box.x + (newG_box.w - (newG_difficultyButton.getDiment1().w + newG_difficultyButton.getDiment2().w + newG_difficultyButton.getDiment3().w)) / 2, newG_box.y + (newG_box.h / 2) + 15);
				}
				else
				{
					newG_keyframe = false;
				}
			}
			//keyframe new game menu down
			else if (newG_keyframeDown)
			{
				newG_box.y = newG_index;

				int posFromEnd = AVE_findM({ newG_box.x, newG_box.y }, { newG_box.x, newG_neededPos });
				newG_boxVelosity = newG_multFact * posFromEnd;

				newG_index += newG_boxVelosity;

				newG_seedTxtPos.x = newG_box.x + (newG_box.w - newG_seedTxt.getWidth()) / 2;
				newG_seedTxtPos.y = newG_box.y + 30 + 50 + 5;

				newG_menuBox.setMenuDiment({ newG_box.x, newG_box.y, newG_box.w, newG_box.h });
				newG_saveName.setPos({ newG_box.x + (newG_box.w - (newG_box.w - 40)) / 2, newG_box.y + 30, newG_box.w - 40, 50 });
				newG_seed.setPos({ newG_box.x + (newG_box.w - (newG_box.w - 40)) / 2, newG_seedTxtPos.y + newG_seedTxt.getHeight() + 5, newG_box.w - 40, 50 });
				newG_createGameButton.setPos(newG_box.x + (newG_box.w - newG_createGameButton.getWidth()) / 2, newG_box.y + newG_box.h - newG_createGameButton.getHeight() - 15);

				newG_backButton.setPos((newG_box.x + newG_box.w), newG_box.y);

				newG_newGameTextPos.x = newG_box.x + (newG_box.w - newG_newGameText.getWidth()) / 2;
				newG_newGameTextPos.y = newG_box.y + 4;

				newG_difficultyButton.setPos(newG_box.x + (newG_box.w - (newG_difficultyButton.getDiment1().w + newG_difficultyButton.getDiment2().w + newG_difficultyButton.getDiment3().w)) / 2, newG_box.y + (newG_box.h / 2) + 15);
			}
			// load menu up keyframe
			if (loadG_menuKey_up)
			{
				loadG_box.y = loadG_menuKey_index;

				int posFromEnd = AVE_findM({ loadG_box.x, loadG_box.y }, { loadG_box.x, loadG_menuKey_neededPos });
				loadG_menuKey_boxVel = loadG_menuKey_multFact * posFromEnd;

				loadG_menuKey_index -= loadG_menuKey_boxVel;

				if (loadG_menuKey_boxVel != 0)
				{
					loadG_menuBox.setMenuDiment(loadG_box);

					loadG_menuDownButton.setPos((loadG_box.x + loadG_box.w), loadG_box.y);

					for (int fileNum = 0; fileNum < loadG_saveNames.size(); fileNum++)
					{
						loadG_buttons[fileNum].setPos(loadG_box.x + 10, loadG_box.y + (fileNum * loadG_buttons[fileNum].getHeight()));
					}

					if (loadG_saveNames.size() <= 5)
					{
						loadG_loadBar.bar.setMenuDiment({ loadG_box.x + loadG_box.w - 30, loadG_box.y, 30, loadG_box.h });
					}
					else
					{
						loadG_loadBar.bar.setMenuDiment({ loadG_box.x + loadG_box.w - 30, loadG_box.y, 30, loadG_box.h - 45 * (loadG_saveNames.size() - 5) });
					}
					SDL_Point __topRef = { loadG_box.x + loadG_box.w - loadG_loadBar.bar.getDiment().w, loadG_box.y };
					SDL_Point __botRef = { loadG_box.x + loadG_box.w, loadG_box.y + loadG_box.h - loadG_loadBar.bar.getDiment().h };
					loadG_loadBar.setLim(__topRef, __botRef);
				}
				else 
				{
					loadG_menuKey_up = false;
				}
			}
			// load menu down keyframe
			else if (loadG_menuKey_down)
			{
				loadG_box.y = loadG_menuKey_index;

				int posFromEnd = AVE_findM({ loadG_box.x, loadG_box.y }, { loadG_box.x, loadG_menuKey_neededPos });
				loadG_menuKey_boxVel = loadG_menuKey_multFact * posFromEnd;

				loadG_menuKey_index += loadG_menuKey_boxVel;

				loadG_menuBox.setMenuDiment(loadG_box);

				loadG_menuDownButton.setPos((loadG_box.x + loadG_box.w), loadG_box.y);

				for (int fileNum = 0; fileNum < loadG_saveNames.size(); fileNum++)
				{
					loadG_buttons[fileNum].setPos(loadG_box.x + 10, loadG_box.y + (fileNum * loadG_buttons[fileNum].getHeight()));
				}

				if (loadG_saveNames.size() <= 5)
				{
					loadG_loadBar.bar.setMenuDiment({ loadG_box.x + loadG_box.w - 30, loadG_box.y, 30, loadG_box.h });
				}
				else
				{
					loadG_loadBar.bar.setMenuDiment({ loadG_box.x + loadG_box.w - 30, loadG_box.y, 30, loadG_box.h - 45 * (loadG_saveNames.size() - 5) });
				}
				SDL_Point __topRef = { loadG_box.x + loadG_box.w - loadG_loadBar.bar.getDiment().w, loadG_box.y };
				SDL_Point __botRef = { loadG_box.x + loadG_box.w, loadG_box.y + loadG_box.h - loadG_loadBar.bar.getDiment().h };
				loadG_loadBar.setLim(__topRef, __botRef);
			}

			//set pos of buttons
			if (!loadG_menuKey_up)
			{
				int loadG_button_yPos = AvMath::rnd::roundUp(-loadG_loadBar.bar.getDiment().y, loadG_buttons[0].getHeight());

				for (int i = 0; i < loadG_saveNames.size(); i++)
				{
					loadG_buttons[i].setPos(loadG_buttons[i].getX(), (loadG_buttons[i].getHeight() * i) + (loadG_box.y + loadG_button_yPos) + (loadG_buttons[0].getHeight()  * 2) + 2);
				}
			}
		}
		else if (currentMenu == 0)
		{
			if (button[3].getDown())
			{
				stop = true;
			}
			else if (button[0].getDown())
			{
				changeMenu(1);
				//Mix_PlayChannel(-1, clickS, 0);
			}
			else if (button[1].getDown())
			{
				changeMenu(2);
				//Mix_PlayChannel(-1, clickS, 0);
			}
			else if (button[2].getDown())
			{
				changeMenu(3);
				//Mix_PlayChannel(-1, clickS, 0);
			}
		}

		else if (currentMenu == 3)
		{
			if (vSyncToggle.getUpdate())
			{
				if (vSyncToggle.getToggled())
				{
					vSyncEnabled = true;
				}
				else if (!vSyncToggle.getToggled())
				{
					vSyncEnabled = false;
				}
			}

			if (optsSav.getDown())
			{
				musicVol = optionsMS.getValue();
				soundVol = optionsSS.getValue();
				vSyncEnabled = vSyncToggle.getToggled();

				Mix_VolumeMusic(musicVol);

				getFlieO();
			}

			if (optionsBack.getDown())
			{
				changeMenu(0);
			}
		}

		else if (currentMenu == 2)
		{
			extrasBack.handleEvent(&e);
			extrasCredits.handleEvent(&e);
			if (!runCredits)
			{
				if (extrasBack.getDown())
				{
					changeMenu(0);
				}
				if (extrasCredits.getDown())
				{
					runCredits = true;
					creditTime = SDL_GetTicks();
					creditXpos = extras.getDiment().h - 200;
				}
			}
			else
			{
				creditTexture.createBlank(mRen, mWindow, extras.getDiment().w - 400, extras.getDiment().h - 200, SDL_TEXTUREACCESS_TARGET);
				creditTexture.setAsRenderTarget(mRen);
				SDL_SetRenderDrawColor(mRen, 0, 0, 0, 0);
				SDL_RenderClear(mRen);
				SDL_SetRenderDrawColor(mRen, 255, 255, 255, 255);

				//credit creation
				LTexture text;
				text.loadFromTtf("Lead programmer", font, { 255, 255, 255, 255 }, mRen);
				text.render(0, creditXpos, mRen);
				text.free();

				text.loadFromTtf("Herry Biscuit", font, { 255, 255, 255, 255 }, mRen);
				text.render((extras.getDiment().w - 400) - text.getWidth(), creditXpos, mRen);
				text.free();

				text.loadFromTtf("Lead musician", font, { 255, 255, 255, 255 }, mRen);
				text.render(0, text.getHeight() + 15 + creditXpos, mRen);
				text.free();

				text.loadFromTtf("SubZER0", font, { 255, 255, 255, 255 }, mRen);
				text.render((extras.getDiment().w - 400) - text.getWidth(), text.getHeight() + 15 + creditXpos, mRen);
				text.free();

				SDL_SetRenderTarget(mRen, NULL);
				creditTexture.setBlendMode(SDL_BLENDMODE_BLEND);

				creditXpos--;

				if (SDL_GetTicks() - creditTime >= 9000)
				{
					runCredits = false;
				}
			}
		}
	//}
}

void game::g_render()
{
	//update hook
	for (int i = 0; i < updateHooks.size(); i++)
	{
		lua_getglobal(L, updateHooks.at(i).functionN.c_str());
		lua_pcall(L, 0, 0, 0);
	}

	if (!minimized)
	{
		if (currentMenu == 4)
		{
			_TGame.render(mRen, mWindow);
		}
		else if (currentMenu == 1)
		{
			background.render(0, 0, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			virsNumb.render((SCREEN_WIDTH - virsNumb.getWidth()), 0, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			newGameButton.render(mRen);
			newGameButtonTxt.render(newGameButton.getX() + (newGameButton.getWidth() - newGameButtonTxt.getWidth()) / 2, newGameButton.getY() + (newGameButton.getHeight() - newGameButtonTxt.getHeight()) / 2, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			loadGameButton.render(mRen);
			loadGameButtonTxt.render(loadGameButton.getX() + (loadGameButton.getWidth() - loadGameButtonTxt.getWidth()) / 2, loadGameButton.getY() + (loadGameButton.getHeight() - loadGameButtonTxt.getHeight()) / 2, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			gameBackButton.render(mRen);
			gameBackButtonTxt.render(gameBackButton.getX() + (gameBackButton.getWidth() - gameBackButtonTxt.getWidth()) / 2, gameBackButton.getY() + (gameBackButton.getHeight() - gameBackButtonTxt.getHeight()) / 2, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			newG_menuBox.render(mRen);
			newG_saveName.render(mRen);
			newG_seed.render(mRen);
			newG_newGameText.render(newG_newGameTextPos.x, newG_newGameTextPos.y, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			newG_seedTxt.render(newG_seedTxtPos.x, newG_seedTxtPos.y, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			newG_createGameButton.render(mRen);
			newG_backButton.render(mRen);
			newG_difficultyButton.render(mRen);

			newG_difficultyButtonTxt[0].render(newG_difficultyButton.getDiment1().x + (newG_difficultyButton.getDiment1().w - newG_difficultyButtonTxt[0].getWidth()) / 2, newG_difficultyButton.getDiment1().y + (newG_difficultyButton.getDiment1().h - newG_difficultyButtonTxt[0].getHeight()) / 2, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			newG_difficultyButtonTxt[1].render(newG_difficultyButton.getDiment2().x + (newG_difficultyButton.getDiment2().w - newG_difficultyButtonTxt[1].getWidth()) / 2, newG_difficultyButton.getDiment2().y + (newG_difficultyButton.getDiment2().h - newG_difficultyButtonTxt[1].getHeight()) / 2, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			newG_difficultyButtonTxt[2].render(newG_difficultyButton.getDiment3().x + (newG_difficultyButton.getDiment3().w - newG_difficultyButtonTxt[2].getWidth()) / 2, newG_difficultyButton.getDiment3().y + (newG_difficultyButton.getDiment3().h - newG_difficultyButtonTxt[2].getHeight()) / 2, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);

			loadG_menuBox.render(mRen);
			loadG_menuDownButton.render(mRen);

			scrollingW.free();
			if (loadG_saveNames.size() > 0)
			{
				scrollingW.createBlank(mRen, mWindow, loadG_buttons[0].getWidth(), 330, SDL_TEXTUREACCESS_TARGET);
			}
			else
			{
				scrollingW.createBlank(mRen, mWindow, 1, 330, SDL_TEXTUREACCESS_TARGET);
			}
			scrollingW.setAsRenderTarget(mRen);
			SDL_SetRenderDrawColor(mRen, 0, 0, 0, 0);
			SDL_RenderClear(mRen);
			for (int i = 0; i < loadG_saveNames.size(); i++)
			{
				//if (loadG_buttons[i].getY() >= loadG_box.y && loadG_buttons[i].getY() <= loadG_box.y + loadG_box.h)
				//{
					loadG_buttons[i].setPos(0, (i * loadG_buttons[i].getHeight()) - ((loadG_loadBar.bar.getDiment().y - loadG_box.y) * /*1.66*/2));
					loadG_buttons[i].render(mRen);
					loadG_tex[i].render(loadG_buttons[i].getX() + (loadG_buttons[i].getWidth() - loadG_tex[i].getWidth()) / 2, loadG_buttons[i].getY() + ((loadG_buttons[i].getHeight() - loadG_tex[i].getHeight()) / 2), mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
				//}
			}

			SDL_SetRenderTarget(mRen, NULL);

			scrollingW.setBlendMode(SDL_BLENDMODE_BLEND);
			scrollingW.render(loadG_box.x + 10, loadG_box.y + 10, mRen);

			if (loadG_saveNames.size() > 0)
				loadG_loadBar.render(mRen);
		}
		else if (currentMenu == 2)
		{
			background.render(0, 0, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			virsNumb.render((SCREEN_WIDTH - virsNumb.getWidth()), 0, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			extras.render(mRen);
			extrasBack.render(mRen);
			extrasBackTxt.render(extrasBack.getX() + ((extrasBack.getWidth() - extrasBackTxt.getWidth()) / 2), extrasBack.getY() + ((extrasBack.getHeight() - extrasBackTxt.getHeight()) / 2), mRen);

			extrasCredits.render(mRen);
			extrasCreditsTxt.render(extrasCredits.getX() + ((extrasCredits.getWidth() - extrasCreditsTxt.getWidth()) / 2), extrasCredits.getY() + ((extrasCredits.getHeight() - extrasCreditsTxt.getHeight()) / 2), mRen);
		
			if (runCredits)
			{
				creditTexture.render((SCREEN_WIDTH - (extras.getDiment().w - 400)) / 2, (SCREEN_HEIGHT - (extras.getDiment().h - 200)) / 2 - 15, mRen);
			}
		}
		else if (currentMenu == 3)
		{
			background.render(0, 0, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			virsNumb.render((SCREEN_WIDTH - virsNumb.getWidth()), 0, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			options.render(mRen);
			vSyncToggle.render(mRen);
			vSyncTxt.render(vSyncToggle.button.getX() - vSyncTxt.getWidth() - 12, vsyncTxtRenderPos.y + ((vSyncToggle.button.getHeight() - vSyncTxt.getHeight()) / 2), mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			soundOTxt.render(soundSPos.x, soundSPos.y, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			optionsMSTxt.render(musicSPos.x, musicSPos.y, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			optsBackTxt.render((optionsBack.getWidth() - optsBackTxt.getWidth()) / 2 + optionsBack.getX(), (optionsBack.getHeight() - optsBackTxt.getHeight()) / 2 + optionsBack.getY(), mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			optsSavTxt.render((optsSav.getWidth() - optsSavTxt.getWidth()) / 2 + optsSav.getX(), (optsSav.getHeight() - optsSavTxt.getHeight()) / 2 + optsSav.getY(), mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			optionsSS.render(mRen);
			optionsMS.render(mRen);
			optionsBack.render(mRen);
			optsSav.render(mRen);
		}
		else if (currentMenu == 0)
		{
			background.render(0, 0, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			virsNumb.render((SCREEN_WIDTH - virsNumb.getWidth()), 0, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);

			button[0].render(mRen);
			text[0].render(((button[0].getWidth() - text[0].getWidth()) / 2) + button[0].getX(), ((button[0].getHeight() - text[0].getHeight()) / 2) + button[0].getY(), mRen, NULL, NULL, NULL, SDL_FLIP_NONE);

			button[1].render(mRen);
			text[1].render(((button[1].getWidth() - text[1].getWidth()) / 2) + button[1].getX(), ((button[1].getHeight() - text[1].getHeight()) / 2) + button[1].getY(), mRen, NULL, NULL, NULL, SDL_FLIP_NONE);

			button[2].render(mRen);
			text[2].render(((button[2].getWidth() - text[2].getWidth()) / 2) + button[2].getX(), ((button[2].getHeight() - text[2].getHeight()) / 2) + button[2].getY(), mRen, NULL, NULL, NULL, SDL_FLIP_NONE);

			button[3].render(mRen);
			text[3].render(((button[3].getWidth() - text[3].getWidth()) / 2) + button[3].getX(), ((button[3].getHeight() - text[3].getHeight()) / 2) + button[3].getY(), mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
			
			//testS.render(mRen);

			menu_title.render(25, 25, mRen, NULL, NULL, NULL, SDL_FLIP_NONE);
		}
		else if (currentMenu = 4)
		{
			_TGame.render(mRen, mWindow);
		}

		if (displayFps)
		{
			fpsCounter.render(0, 0, mRen);
		}

		//render hook
		for (int i = 0; i < renderHooks.size(); i++)
		{
			lua_getglobal(L, renderHooks.at(i).functionN.c_str());
			lua_pcall(L, 0, 0, 0);
		}
	}
}

bool game::g_getStop()
{
	return stop;
}

bool game::g_getMini()
{
	return minimized;
}

void game::getFileI()
{
	file.close();
	file.open("cfg/options.cfg", ios::in | ios::app | ios::binary);
	if (file.is_open())
	{
		file >> soundVol >> musicVol >> vSyncEnabled;
	}
	else
	{
		cout << "unable to open options file reseting to default!" << endl;
		soundVol = 100;
		musicVol = 100;
		vSyncEnabled = true;
	}
	file.close();
}

void game::getFlieO()
{
	file.close();
	file.open("cfg/options.cfg" , ios::trunc | ios::out | ios::binary);

	stringstream ss;
	ss << soundVol << endl << musicVol << endl << vSyncEnabled << endl;

	if (file.is_open())
	{
		file << ss.str();
	}
	else
	{
		cout << "unable to open options file!" << endl;
	}
	file.close();
}

void game::g_preRender()
{
	if (!minimized)
	{
		SDL_SetRenderDrawColor(mRen, 255, 255, 255, 255);
		SDL_RenderClear(mRen);
	}
}

void game::g_aftRender()
{
	if (!minimized)
	{
		SDL_RenderPresent(mRen);
	}
	++countedFrames;
}

//used to clear and allocate memory
void game::changeMenu(int menu)
{
	if (menu == 0)
	{
		button[0].setSound("data/wav/hover1.wav", soundVol);
		button[0].setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
		button[0].loadFromfile("data/img/newbutton.png", mRen);
		button[0].setPos(35, (SCREEN_HEIGHT - button[0].getHeight()) - 15);
		text[0].loadFromTtf("Start Game", font, { 255, 255, 255, 255 }, mRen);

		button[1].setSound("data/wav/hover1.wav", soundVol);
		button[1].setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
		button[1].loadFromfile("data/img/newbutton.png", mRen);
		button[1].setPos((button[1].getWidth() * 1) + 105, (SCREEN_HEIGHT - button[1].getHeight()) - 15);
		text[1].loadFromTtf("Extras", font, { 255, 255, 255, 255 }, mRen);

		button[2].setSound("data/wav/hover1.wav", soundVol);
		button[2].setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
		button[2].loadFromfile("data/img/newbutton.png", mRen);
		button[2].setPos((button[2].getWidth() * 2) + 175, (SCREEN_HEIGHT - button[2].getHeight()) - 15);
		text[2].loadFromTtf("Options", font, { 255, 255, 255, 255 }, mRen);

		button[3].setSound("data/wav/hover1.wav", soundVol);
		button[3].setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
		button[3].loadFromfile("data/img/newbutton.png", mRen);
		button[3].setPos((button[3].getWidth() * 3) + 245, (SCREEN_HEIGHT - button[3].getHeight()) - 15);
		text[3].loadFromTtf("Quit", font, { 255, 255, 255, 255 }, mRen);

		titleFont = TTF_OpenFont("data/ERASMD.TTF", 80);
		menu_title.loadFromTtf("Galactic void", titleFont, { 255, 255, 255, 255 }, mRen);
		//TTF_CloseFont(titleFont);

		background.loadFromFile("data/img/background.png", mRen);

		version_Font = TTF_OpenFont("data/ERASDEMI.TTF", 31);
		virsNumb.loadFromTtf(VIRSION_NUMBER, version_Font, { 255, 255, 255, 255 }, mRen);
		TTF_CloseFont(version_Font);

		options.free();
		optionsMSTxt.free();
		soundOTxt.free();
		vSyncTxt.free();
		vSyncToggle.free();
		optionsSS.free();
		optionsMS.free();
		optionsBack.free();
		optsBackTxt.free();
		optsSavTxt.free();
		optsSav.free();
		newGameButton.free();
		newGameButtonTxt.free();
		loadGameButton.free();
		loadGameButtonTxt.free();
		gameBackButton.free();
		gameBackButtonTxt.free();
		newG_saveName.box.free();
		newG_seed.box.free();
		newG_seedTxt.free();
		newG_menuBox.free();
		newG_backButton.free();
		extras.free();
		newG_createGameButton.free();
		newG_difficultyButton.free();

		loadG_saveNames.clear();
		loadG_menuBox.free();
		loadG_menuDownButton.free();
		loadG_loadBar.free();

		for (int i = 0; i < 3; i++)
		{
			newG_difficultyButtonTxt[i].free();
		}

		for (int i = 0; i < loadG_saveNames.size(); i++)
		{
			loadG_tex[i].free();
		}
		for (int i = 0; i < loadG_saveNames.size(); i++)
		{
			loadG_buttons[i].free();
		}


		currentMenu = 0;
	}
	else if (menu == 1)
	{
		newG_font = TTF_OpenFont("data/ERASMD.TTF", 18);
		newG_newgamefont = TTF_OpenFont("data/ERASMD.TTF", 20);


		if (loadG_saveNames.size() > 0)
		{
			if (!scrollingW.createBlank(mRen, mWindow, loadG_buttons[0].getWidth(), 330, SDL_TEXTUREACCESS_TARGET))
			{
				cout << "Unable to create target texture! " << SDL_GetError() << endl;
			}
		}
		else
		{
			if (!scrollingW.createBlank(mRen, mWindow, 1, 330, SDL_TEXTUREACCESS_TARGET))
			{
				cout << "Unable to create target texture! " << SDL_GetError() << endl;
			}
		}

		newGameButton.setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
		newGameButton.loadFromfile("data/img/newbutton.png", mRen);
		newGameButton.setPos((SCREEN_WIDTH - newGameButton.getWidth()) / 2 - newGameButton.getWidth() - 70, (SCREEN_HEIGHT - newGameButton.getHeight()) - 15);
		newGameButtonTxt.loadFromTtf("New Game", font, { 255, 255, 255, 255 }, mRen);

		loadGameButton.setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
		loadGameButton.loadFromfile("data/img/newbutton.png", mRen);
		loadGameButton.setPos((SCREEN_WIDTH - loadGameButton.getWidth()) / 2, (SCREEN_HEIGHT - loadGameButton.getHeight()) - 15);
		loadGameButtonTxt.loadFromTtf("Load Game", font, { 255, 255, 255, 255 }, mRen);

		gameBackButton.setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
		gameBackButton.loadFromfile("data/img/newbutton.png", mRen);
		gameBackButton.setPos((SCREEN_WIDTH - gameBackButton.getWidth()) / 2 + gameBackButton.getWidth() + 70, (SCREEN_HEIGHT - gameBackButton.getHeight()) - 15);
		gameBackButtonTxt.loadFromTtf("Back", font, { 255, 255, 255, 255 }, mRen);

		newG_box = { newGameButton.getX(), SCREEN_HEIGHT, 250, 350 };

		newG_menuBox.setScreenWH(SCREEN_WIDTH, SCREEN_HEIGHT);
		newG_menuBox.setClips({ 0, 0, 17, 16 }, { 0, 17, 17, 2 });
		newG_menuBox.setMenuDiment({ newG_box.x, newG_box.y, newG_box.w, newG_box.h });
		newG_menuBox.loadTextures("data/img/menutxt2.png", "data/img/menutxt2.png", mRen);

		newG_saveName.setDefaultString(" ");
		newG_saveName.loadStartText(" ", newG_font, mRen);
		newG_saveName.setCharLim(18);
		newG_saveName.setPos({ newG_box.x + (newG_box.w - (newG_box.w - 40)) / 2, newG_box.y + 30, newG_box.w - 40, 50 });
		newG_saveName.box.setScreenWH(SCREEN_WIDTH, SCREEN_HEIGHT);
		newG_saveName.box.setClips({ 0, 0, 17, 16 }, { 0, 17, 17, 2 });
		newG_saveName.box.loadTextures("data/img/menutxt2.png", "data/img/menutxt2.png", mRen);

		newG_seedTxt.loadFromTtf("Seed", newG_newgamefont, { 255, 255, 255, 255 }, mRen);
		newG_seedTxtPos.x = newG_box.x + (newG_box.w - newG_seedTxt.getWidth()) / 2;
		newG_seedTxtPos.y = newG_box.y + 30 + 50 + 5;

		newG_seed.setDefaultString(" ");
		newG_seed.loadStartText(" ", newG_font, mRen);
		newG_seed.setCharLim(17);
		newG_seed.setPos({ newG_box.x + (newG_box.w - (newG_box.w - 40)) / 2, newG_seedTxtPos.y + newG_seedTxt.getHeight() + 5, newG_box.w - 40, 50 });
		newG_seed.box.setScreenWH(SCREEN_WIDTH, SCREEN_HEIGHT);
		newG_seed.box.setClips({ 0, 0, 17, 16 }, { 0, 17, 17, 2 });
		newG_seed.box.loadTextures("data/img/menutxt2.png", "data/img/menutxt2.png", mRen);

		newG_newGameText.loadFromTtf("Create a new game", newG_newgamefont, { 255, 255, 255, 255 }, mRen);
		newG_newGameTextPos.x = newG_box.x + (newG_box.w - newG_newGameText.getWidth()) / 2;
		newG_newGameTextPos.y = newG_box.y + 2;

		newG_createGameButton.setPos(newG_box.x + (newG_box.w - newG_createGameButton.getWidth()) / 2, newG_box.y + newG_box.h - newG_createGameButton.getHeight() - 15);
		newG_createGameButton.setClips({ 0, 0 * 52, 139, 52 }, { 0, 1 * 52, 139, 52 }, { 0, 2 * 52, 139, 52 }, { 0, 1 * 52, 139, 52 });
		newG_createGameButton.loadFromfile("data/img/smallerbutton.png", mRen);
		newG_createGameButtonTxt.loadFromTtf("create game", font, { 255, 255, 255, 255 }, mRen);

		newG_backButton.setClips({ 0, 0, 25, 50 }, { 0, 1 * 50, 25, 50 }, { 0, 2 * 50, 25, 50 }, { 0, 1 * 50, 25, 50 });
		newG_backButton.loadFromfile("data/img/newG_backButton.png", mRen);
		newG_backButton.setPos((newG_box.x + newG_box.w), newG_box.y);

		newG_difficultyButton.setMaxButtons(3);
		newG_difficultyButton.setClips({ 0, 0 * 50, 70, 50 }, { 0, 1 * 50, 70, 50 }, { 0, 2 * 50, 70, 50 }, { 0, 1 * 50, 70, 50 });
		newG_difficultyButton.loadTextures("data/img/difbutton.png", mRen);
		newG_difficultyButton.setPos(newG_box.x + (newG_box.w - (newG_difficultyButton.getDiment1().w + newG_difficultyButton.getDiment2().w + newG_difficultyButton.getDiment3().w)) / 2, newG_box.y + (newG_box.h / 2) + 15);

		newG_difficultyFont = TTF_OpenFont("data/ERASMD.TTF", 15);

		newG_difficultyButtonTxt[0].loadFromTtf("Easy", newG_difficultyFont, { 255, 255, 255, 255 }, mRen);
		newG_difficultyButtonTxt[1].loadFromTtf("Normal", newG_difficultyFont, { 255, 255, 255, 255 }, mRen);
		newG_difficultyButtonTxt[2].loadFromTtf("Hard", newG_difficultyFont, { 255, 255, 255, 255 }, mRen);

		TTF_CloseFont(newG_difficultyFont);

		TTF_Font *loadFont = TTF_OpenFont("data/ERASMD.TTF", 23);
		loadG_box = { loadGameButton.getX(), SCREEN_HEIGHT, 250, 350 };

		//get savefiles
		WIN32_FIND_DATA fd;
		HANDLE hFind = FindFirstFile(R"(sav\*)", &fd);
		int fileNum = 0;
		cout << "Registered save files:" << endl;
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				string dir = fd.cFileName;
				if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(dir == "." || dir == ".."))
				{
					loadG_saveNames.push_back(fd.cFileName);

					cout << '\t' << dir << endl;

					loadG_buttons[fileNum].setClips({ 0, 0 * 70, 200, 70 }, { 0, 1 * 70, 200, 70 }, { 0, 2 * 70, 200, 70 }, { 0, 1 * 70, 200, 70 });
					loadG_buttons[fileNum].loadFromfile("data/img/LongButton.png", mRen);
					loadG_buttons[fileNum].setPos(loadG_box.x + 10, loadG_box.y + (fileNum * loadG_buttons[fileNum].getHeight()));
					loadG_tex[fileNum].loadFromTtf(dir, loadFont, { 255, 255, 255, 255 }, mRen);

					fileNum++;
				}
			} while (FindNextFile(hFind, &fd));
			FindClose(hFind);
		}

		TTF_CloseFont(loadFont);

		loadG_menuBox.setScreenWH(SCREEN_WIDTH, SCREEN_HEIGHT);
		loadG_menuBox.setClips({ 0, 0, 17, 16 }, { 0, 17, 17, 2 });
		loadG_menuBox.setMenuDiment(loadG_box);
		loadG_menuBox.loadTextures("data/img/menutxt2.png", "data/img/menutxt2.png", mRen);

		loadG_menuDownButton.setClips({ 0, 0, 25, 50 }, { 0, 1 * 50, 25, 50 }, { 0, 2 * 50, 25, 50 }, { 0, 1 * 50, 25, 50 });
		loadG_menuDownButton.loadFromfile("data/img/newG_backButton.png", mRen);
		loadG_menuDownButton.setPos((loadG_box.x + loadG_box.w), loadG_box.y);

		if (loadG_saveNames.size() > 0)
		{
			loadG_loadBar.bar.setScreenWH(SCREEN_WIDTH, SCREEN_HEIGHT);
			loadG_loadBar.bar.setClips({ 0, 0, 17, 16 }, { 0, 17, 17, 2 });
			if (loadG_saveNames.size() <= 5)
			{
				loadG_loadBar.bar.setMenuDiment({ loadG_box.x + loadG_box.w - 30, loadG_box.y, 30, loadG_box.h });
			}
			else
			{
				loadG_loadBar.bar.setMenuDiment({ loadG_box.x + loadG_box.w - 30, loadG_box.y, 30, loadG_box.h - 45 * (loadG_saveNames.size() - 5)/*20*/ });
			}
			loadG_loadBar.bar.loadTextures("data/img/menutxt2.png", "data/img/menutxt2.png", mRen);
			SDL_Point __topRef = { loadG_box.x + loadG_box.w - loadG_loadBar.bar.getDiment().w, loadG_box.y };
			SDL_Point __botRef = { loadG_box.x + loadG_box.w, loadG_box.y + loadG_box.h - loadG_loadBar.bar.getDiment().h };
			loadG_loadBar.setLim(__topRef, __botRef);
		}

		loadG_menuKey_upEd = false;
		loadG_menuKey_downEd = true;
		newG_keyframEDDown = true;
		newG_keyframED = false;

		options.free();
		optionsMSTxt.free();
		soundOTxt.free();
		vSyncTxt.free();
		vSyncToggle.free();
		optionsSS.free();
		optionsMS.free();
		optionsBack.free();
		optsBackTxt.free();
		optsSavTxt.free();
		optsSav.free();
		extras.free();
		menu_title.free();
		for (int i = 0; i < 4; i++)
		{
			button[i].free();
		}
		for (int i = 0; i < 4; i++)
		{
			text[i].free();
		}

		currentMenu = 1;
	}
	else if (menu == 2)
	{
		extras.setScreenWH(SCREEN_WIDTH, SCREEN_HEIGHT);
		extras.setClips({ 0, 0, 17, 16 }, { 0, 17, 17, 2 });
		extras.setMenuDiment({ 50, 50, SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100 });
		extras.loadTextures("data/img/menutxt2.png", "data/img/menutxt2.png", mRen);

		extrasBack.setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
		extrasBack.loadFromfile("data/img/newbutton.png", mRen);
		extrasBack.setPos(extras.getDiment().x + ((extras.getDiment().w - extrasBack.getWidth()) / 2) + (extrasBack.getWidth() / 2) + 5, extras.getDiment().y + extras.getDiment().h - extrasBack.getHeight() - 15);
		extrasBackTxt.loadFromTtf("Back", font, { 255, 255, 255, 255 }, mRen);

		extrasCredits.setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
		extrasCredits.loadFromfile("data/img/newbutton.png", mRen);
		extrasCredits.setPos(extras.getDiment().x + ((extras.getDiment().w - extrasCredits.getWidth()) / 2) - (extrasCredits.getWidth() / 2) - 5, extras.getDiment().y + extras.getDiment().h - extrasCredits.getHeight() - 15);
		extrasCreditsTxt.loadFromTtf("Credits", font, { 255, 255, 255, 255 }, mRen);

		loadG_saveNames.clear();

		options.free();
		optionsMSTxt.free();
		soundOTxt.free();
		vSyncTxt.free();
		vSyncToggle.free();
		optionsSS.free();
		optionsMS.free();
		optionsBack.free();
		optsBackTxt.free();
		optsSavTxt.free();
		optsSav.free();
		newGameButton.free();
		newGameButtonTxt.free();
		loadGameButton.free();
		loadGameButtonTxt.free();
		gameBackButton.free();
		gameBackButtonTxt.free();
		newG_saveName.box.free();
		newG_seed.box.free();
		newG_seedTxt.free();
		newG_menuBox.free();
		newG_backButton.free();
		newG_createGameButton.free();
		newG_difficultyButton.free();
		menu_title.free();
		loadG_menuBox.free();
		loadG_menuDownButton.free();
		loadG_loadBar.free();

		for (int i = 0; i < loadG_saveNames.size(); i++)
		{
			loadG_tex[i].free();
		}
		for (int i = 0; i < loadG_saveNames.size(); i++)
		{
			loadG_buttons[i].free();
		}


		for (int i = 0; i < 3; i++)
		{
			newG_difficultyButtonTxt[i].free();
		}

		for (int i = 0; i < 4; i++)
		{
			button[i].free();
			text[i].free();
		}

		currentMenu = 2;
	}
	else if (menu == 3)
	{
		options.setScreenWH(SCREEN_WIDTH, SCREEN_HEIGHT);
		options.setClips({ 0, 0, 17, 16 }, { 0, 17, 17, 2 });
		options.setMenuDiment({ 50, 50, SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100 });
		options.loadTextures("data/img/menutxt2.png", "data/img/menutxt2.png", mRen);

		soundSPos.x = options.getDiment().x + 100;
		soundSPos.y = options.getDiment().y + 100;

		TTF_CloseFont(font);
		font = TTF_OpenFont("data/ERASMD.TTF", 45);

		vSyncTxt.loadFromTtf("vSync enabled", font, { 255, 255, 255, 255 }, mRen);
		soundOTxt.loadFromTtf("Sound volume", font, { 255, 255, 255, 255 }, mRen);
		optionsMSTxt.loadFromTtf("Music volume", font, { 255, 255, 255, 255 }, mRen);

		TTF_CloseFont(font);
		font = TTF_OpenFont("data/ERASMD.TTF", 31);

		musicSPos.x = (options.getDiment().x + options.getDiment().w) - optionsMSTxt.getWidth() - 100;
		musicSPos.y = (options.getDiment().y) + optionsMSTxt.getHeight() + 50;
		SDL_Rect musicSliderRenderPos = { ((optionsMSTxt.getWidth() - 200) / 2) + musicSPos.x, musicSPos.y + optionsMSTxt.getHeight(), 200, 32 };

		vSyncToggle.button.setClips({ 0, 0 * 32, 32, 32 }, { 0, 1 * 32, 32, 32 }, { 0, 2 * 32, 32, 32 }, { 0, 1 * 32, 32, 32 });
		vSyncToggle.button.loadFromfile("data/img/toggleBT.png", mRen);

		vsyncTxtRenderPos.y = options.getDiment().y + ((options.getDiment().h - vSyncTxt.getHeight()) / 2);
		vsyncTxtRenderPos.x = options.getDiment().x + ((options.getDiment().w - (vSyncTxt.getWidth() + vSyncToggle.button.getWidth() + 12)) / 2);

		vSyncToggle.button.setPos(vsyncTxtRenderPos.x + (vSyncTxt.getWidth() + (12 + 6)), vsyncTxtRenderPos.y);
		vSyncToggle.setClips({ 0, 0, 32, 32 }, { 0, 32, 32, 32 });
		vSyncToggle.loadTextures("data/img/toggleButton.png", mRen);
		vSyncToggle.setToggled(vSyncEnabled);

		optionsSS.dash.setClips({ 32, 0 * 12, 6, 12 }, { 32, 1 * 12, 6, 12 }, { 32, 2 * 12, 6, 12 }, { 32, 1 * 12, 6, 12 });
		optionsSS.dash.loadFromfile("data/img/slider.png", mRen);
		optionsSS.setPos({ ((soundOTxt.getWidth() - 200) / 2) + soundSPos.x, soundSPos.y + soundOTxt.getHeight(), 200, 32 });
		optionsSS.setSValue(soundVol);
		optionsSS.setClips({ 0, 0, 32, 32 }, { 0, 32, 32, 4 });
		optionsSS.setTextures("data/img/slider.png", "data/img/slider.png", mRen);

		optionsMS.dash.setClips({ 32, 0 * 12, 6, 12 }, { 32, 1 * 12, 6, 12 }, { 32, 2 * 12, 6, 12 }, { 32, 1 * 12, 6, 12 });
		optionsMS.dash.loadFromfile("data/img/slider.png", mRen);
		optionsMS.setPos(musicSliderRenderPos);
		optionsMS.setSValue(musicVol);
		optionsMS.setClips({ 0, 0, 32, 32 }, { 0, 32, 32, 4 });
		optionsMS.setTextures("data/img/slider.png", "data/img/slider.png", mRen);

		optionsBack.setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
		optionsBack.loadFromfile("data/img/newbutton.png", mRen);
		optionsBack.setPos((options.getDiment().w - optionsBack.getWidth()) / 2 + options.getDiment().x + (optionsBack.getWidth() / 2) + 12, (options.getDiment().y + options.getDiment().h) - optionsBack.getHeight() - 25);
		optsBackTxt.loadFromTtf("Back", font, { 255, 255, 255, 255 }, mRen);

		optsSav.setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
		optsSav.loadFromfile("data/img/newbutton.png", mRen);
		optsSav.setPos((options.getDiment().w - optsSav.getWidth()) / 2 + options.getDiment().x - (optsSav.getWidth() / 2) - 12, (options.getDiment().y + options.getDiment().h) - optsSav.getHeight() - 25);
		optsSavTxt.loadFromTtf("Save", font, { 255, 255, 255, 255 }, mRen);
		
		for (int i = 0; i < 4; i++)
		{
			button[i].free();
		}
		for (int i = 0; i < 4; i++)
		{
			text[i].free();
		}

		for (int i = 0; i < loadG_saveNames.size(); i++)
		{
			loadG_tex[i].free();
		}
		for (int i = 0; i < loadG_saveNames.size(); i++)
		{
			loadG_buttons[i].free();
		}

		newG_difficultyButton.free();

		for (int i = 0; i < 3; i++)
		{
			newG_difficultyButtonTxt[i].free();
		}

		loadG_saveNames.clear();

		menu_title.free();
		newGameButton.free();
		newGameButtonTxt.free();
		loadGameButton.free();
		loadGameButtonTxt.free();
		gameBackButton.free();
		gameBackButtonTxt.free();
		newG_saveName.box.free();
		newG_seed.box.free();
		newG_seedTxt.free();
		newG_menuBox.free();
		extras.free();
		newG_backButton.free();
		newG_createGameButton.free();
		loadG_menuBox.free();
		loadG_menuDownButton.free();
		loadG_loadBar.free();

		currentMenu = 3;
	}
	else
	{
		cout << "invalid menu!" << endl;
	}
}

void game::freeAll()
{
	background.free();
	options.free();
	optionsMSTxt.free();
	soundOTxt.free();
	vSyncTxt.free();
	vSyncToggle.free();
	optionsSS.free();
	optionsMS.free();
	optionsBack.free();
	optsBackTxt.free();
	optsSavTxt.free();
	optsSav.free();
	newGameButton.free();
	newGameButtonTxt.free();
	loadGameButton.free();
	loadGameButtonTxt.free();
	gameBackButton.free();
	gameBackButtonTxt.free();
	newG_saveName.box.free();
	newG_seed.box.free();
	newG_seedTxt.free();
	newG_menuBox.free();
	newG_backButton.free();
	extras.free();
	newG_createGameButton.free();
	newG_difficultyButton.free();

	for (int i = 0; i < 3; i++)
	{
		newG_difficultyButtonTxt[i].free();
	}
	for (int i = 0; i < 4; i++)
	{
		button[i].free();
	}
	for (int i = 0; i < 4; i++)
	{
		text[i].free();
	}

	/*if (clickS != NULL)
	{
		Mix_FreeChunk(clickS);
		clickS = NULL;
	}*/
}

bool game::getFastLoop()
{
	return _TGame.getFastLoop();
}
