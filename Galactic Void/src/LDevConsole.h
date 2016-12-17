#ifndef __LDEVCONSOLE_H_INCLUDED
#define __LDEVCONSOLE_H_INCLUDED

#include "luaFunctions.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "AdvTexture.h"
#include "LTextBox.h"

using namespace std;

struct consoleSuggest
{
	int x;
	int y;
};

class LDevConsole
{
public:
	LDevConsole();
	~LDevConsole();

	void handleEvents(SDL_Event* e, SDL_Renderer* mren, lua_State* ___l);
	void render(SDL_Renderer* mren);
	void printToConsole(string str, SDL_Color color, SDL_Renderer* mren);
	void setMaxMsg(unsigned int max);
	void setConsoleRect(SDL_Rect __ref, int scrH, int scrW, SDL_Renderer* mren);
	void updateConsole(SDL_Renderer* mren, SDL_Window* winref);
	bool getConsoleOpen();
	void runCommand(string command, SDL_Renderer* mren);
	bool getupdatePlayerCommand();
	SDL_Point getPlyPos();
	bool getChunckOn();
	bool getSolorOn();
	void free();

	bool getUpdateAmmo();
	int getAmmoPool();
	void setUpdateAmmo(bool update);
private:
	vector<string> devCLog;
	vector<SDL_Color> devCColors;
	//AdvTexture devCLogTxt;
	SDL_Texture* devCTexts[20];
	vector<SDL_Rect> devCRects;
	LMenu devConsole;
	LTextBox inputBox;
	unsigned int maxMsgs = 10;
	SDL_Rect consoleRect;
	bool showConsole;
	TTF_Font* consoleFont;
	consoleSuggest consoleInfo;
	bool updatePlayerCommand = false;

	bool ammoPoolUpdate = false;
	int ammoPool = 0;

	bool showSolorChuncks = false;
	bool showChuncks = false;
};

#endif //__LDEVCONSOLE_H_INCLUDED