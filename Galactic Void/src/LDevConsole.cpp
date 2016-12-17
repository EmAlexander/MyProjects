#include "LDevConsole.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

LDevConsole::LDevConsole()
{
	showConsole = false;
}

LDevConsole::~LDevConsole()
{
	free();
}

void LDevConsole::printToConsole(string str, SDL_Color color, SDL_Renderer* mren)
{
	devCLog.push_back(str);
	devCColors.push_back(color);
	if (devCLog.size() > maxMsgs)
		devCLog.erase(devCLog.begin());

	if (devCColors.size() > maxMsgs)
		devCColors.erase(devCColors.begin());

	LTexture refTexture;

	refTexture.loadFromTtf(str, consoleFont, color, mren);
	SDL_Rect refRect = { 0, 0, refTexture.getWidth(), refTexture.getHeight() };
	
	devCRects.push_back(refRect);

	if (devCRects.size() > maxMsgs)
		devCRects.erase(devCRects.begin());

	refTexture.free();
}

void LDevConsole::setMaxMsg(unsigned int max)
{
	maxMsgs = max;
}

void LDevConsole::setConsoleRect(SDL_Rect __ref, int scrH, int scrW, SDL_Renderer* mren)
{
	consoleFont = TTF_OpenFont("data/ERASMD.TTF", 15);

	consoleRect = __ref;

	devConsole.setScreenWH(scrW, scrH);
	devConsole.setClips({ 0, 0, 17, 16 }, { 0, 17, 17, 2 });
	devConsole.setMenuDiment(__ref);
	devConsole.loadTextures("data/img/menutxt2.png", "data/img/menutxt2.png", mren);

	inputBox.setDefaultString(" ");
	inputBox.loadStartText(" ", consoleFont, mren);
	inputBox.setCharLim(53);
	SDL_Rect devCInputRef = { __ref.x, __ref.y + __ref.h + 5, __ref.w, 50 };
	inputBox.setPos(devCInputRef);
	inputBox.box.setScreenWH(scrW, scrH);
	inputBox.box.setClips({ 0, 0, 17, 16 }, { 0, 17, 17, 2 });
	inputBox.box.loadTextures("data/img/menutxt2.png", "data/img/menutxt2.png", mren);
}

void LDevConsole::free()
{
	for (int i = 0; i < 10; i++)
	{
		SDL_DestroyTexture(devCTexts[i]);
	}
}

void LDevConsole::updateConsole(SDL_Renderer* mren, SDL_Window* winref)
{
	if (showConsole)
	{
		for (int i = 0; i < devCLog.size(); i++)
		{
			if (devCTexts[i] != NULL)
				SDL_DestroyTexture(devCTexts[i]);

			SDL_Surface* text = TTF_RenderText_Solid(consoleFont, devCLog.at(i).c_str(), devCColors.at(i));
			if (text == NULL)
			{
				cout << "Unable to load text! " << TTF_GetError() << endl;
			}
			else
			{
				SDL_Surface* formattedSurface = SDL_ConvertSurface(text, SDL_GetWindowSurface(winref)->format, NULL);
				if (formattedSurface == NULL)
				{
					cout << "Unable to load text! " << SDL_GetError() << endl;
				}
				else
				{
					devCTexts[i] = SDL_CreateTextureFromSurface(mren, formattedSurface);
				}
				SDL_FreeSurface(formattedSurface);
			}
			SDL_FreeSurface(text);
		}
	}
}

void LDevConsole::render(SDL_Renderer* mren)
{
	if (showConsole)
	{
		for (int i = 0; i < devCLog.size(); i++)
		{
			SDL_Rect cpyRef = { consoleRect.x + devConsole.getRenderClip1().w, consoleRect.y + (devCRects.at(i).h * i), devCRects.at(i).w, devCRects.at(i).h };
			SDL_RenderCopy(mren, devCTexts[i], NULL, &cpyRef);
		}
		devConsole.render(mren);
		inputBox.render(mren);
	}
}

void LDevConsole::runCommand(string command, SDL_Renderer* mren)
{
	if (command.length() != 0)
	{
		vector<string> x = split(command, ' ');
		std::transform(x.at(0).begin(), x.at(0).end(), x.at(0).begin(), ::tolower);
		if (x.at(0) == "print")
		{
			if (x.size() >= 2)
			{
				int ammountOfQu = 0;
				string _ref = command;
				for (int i = 0; i < _ref.length(); i++)
				{
					if (_ref[i] == '\"')
					{
						if (ammountOfQu >= 1)
						{
							_ref.erase(_ref.begin() + i, _ref.end());
						}
						else
						{
							_ref.erase(_ref.begin() + i);
						}
						ammountOfQu++;
					}
				}
				_ref.erase(_ref.begin(), _ref.begin() + 6);

				if (ammountOfQu == 2)
				{
					printToConsole(_ref, { 191, 191, 191, 255 }, mren);
				}
				else if (ammountOfQu <= 0)
				{
					printToConsole(x.at(1), { 191, 191, 191, 255 }, mren);
				}
			}
		}
		if (x.at(0) == "setplypos")
		{
			if (x.size() >= 3)
			{
				consoleInfo.x = atoi(x.at(1).c_str());
				consoleInfo.y = atoi(x.at(2).c_str());
				printToConsole("Setting players pos: " + x.at(1) + " " + x.at(2), { 191, 191, 191, 255 }, mren);
				updatePlayerCommand = true;
			}
		}
		if (x.at(0) == "showchuncks")
		{
			showChuncks = !showChuncks;
			printToConsole("Toggleing chuncks!", { 191, 191, 191, 255 }, mren);
		}
		if (x.at(0) == "showsolorchuncks")
		{
			showSolorChuncks = !showSolorChuncks;
			printToConsole("Toggleing solor chuncks!", { 191, 191, 191, 255 }, mren);
		}
		if (x.at(0) == "setcurrentammo")
		{
			if (x.size() >= 2)
			{
				ammoPool = atoi(x.at(1).c_str());
				ammoPoolUpdate = true;
				printToConsole("Setting ammo pool to: " + to_string(ammoPool), { 191, 191, 191, 255 }, mren);
			}
		}
		if (x.at(0) == "iddqd")
		{
			printToConsole("This ain\'t doom!", { 191, 191, 191, 255 }, mren);
		}
	}
}

void LDevConsole::handleEvents(SDL_Event* e, SDL_Renderer* mren, lua_State* ___l)
{
	updatePlayerCommand = false;
	inputBox.handleEvents(e, mren, consoleFont);

	if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_BACKQUOTE:
			showConsole = !showConsole;
			if (!showConsole)
				inputBox.setSelected(false);
			break;
		case SDLK_RETURN:
			printToConsole("> " + inputBox.getText(), { 255, 255, 255, 255 }, mren);
			runCommand(inputBox.getText(), mren);

			for (int i = 0; i < commandHooks.size(); i++)
			{
				lua_getglobal(___l, commandHooks.at(i).functionN.c_str());

				lua_pushstring(___l, inputBox.getText().c_str());

				lua_pcall(___l, 1, 0, 0);
			}

			inputBox.setText(" ");
			break;
		}
	}
}

bool LDevConsole::getConsoleOpen()
{
	return showConsole;
}

bool LDevConsole::getupdatePlayerCommand()
{
	return updatePlayerCommand;
}

SDL_Point LDevConsole::getPlyPos()
{
	return{ consoleInfo.x, consoleInfo.y };
}

bool LDevConsole::getChunckOn()
{
	return showChuncks;
}

bool LDevConsole::getSolorOn()
{
	return showSolorChuncks;
}

bool LDevConsole::getUpdateAmmo()
{
	return ammoPoolUpdate;
}

int LDevConsole::getAmmoPool()
{
	return ammoPool;
}

void LDevConsole::setUpdateAmmo(bool update)
{
	ammoPoolUpdate = update;
}