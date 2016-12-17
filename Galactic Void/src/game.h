/*
Version: 0.8
Last changed: 6/17/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <iostream>
#include <lua.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <Windows.h>
#include <direct.h>

#include "AdvTexture.h"
#include "SDL.h"
#include "LButton.h"
#include "LTexture.h"
#include "LMenu.h"
#include "LToggleButton.h"
#include "LTimer.h"
#include "LSlider.h"
#include "vector.h"
#include "LTextBox.h"
#include "LSButtonSet.h"
#include "TGamePlay.h"
#include "LVertScrollBar.h"

#include "modbase.hpp"

using namespace std;

class game
{
public:
	game();
	~game();

	//init game
	bool g_init(const char* wName, int xPos, int yPos, int tags);
	//load first textures
	bool g_loadMedia();
	//returns stop for game loop
	bool g_getStop();
	//gets if minimized
	bool g_getMini();
	//handles events
	void g_handleEvents();
	//updates
	void g_update();

	//renders
	void g_preRender();
	void g_render();
	void g_aftRender();

	void g_startTimer();

	void getFlieO();

	void freeAll();

	bool getFastLoop();
private:
	TGamePlay _TGame;

	const string VIRSION_NUMBER = "INDEV 0.10.6";

	//screen w/h used for render caculations
	int SCREEN_WIDTH = 1280;
	int SCREEN_HEIGHT = 720;

	//create lua state for modding
	lua_State* L;

	vector<string> modFiles;

	//main window
	SDL_Window* mWindow;
	//main render
	SDL_Renderer* mRen;
	//main event que
	SDL_Event e;
	//square font
	TTF_Font* font;
	
	/*
	0 = main menu
	1 = main game
	2 = extras menu
	3 = options menu
	4 = main game
	*/
	int currentMenu = 0;

	void changeMenu(int menu);

	/*main menu block*/
	//buttons
	LButton button[4];
	//text on buttons
	LTexture text[4];

	//LVertScrollBar testS;
	//title
	LTexture menu_title;
	TTF_Font* titleFont;

	/*main game block*/
	LButton newGameButton;
	LTexture newGameButtonTxt;
	LButton loadGameButton;
	LTexture loadGameButtonTxt;
	LButton gameBackButton;
	LTexture gameBackButtonTxt;

	//virsion number
	LTexture virsNumb;
	//main menu background
	LTexture background;

	/*options menu block*/
	LMenu options;
	LToggleButton vSyncToggle;
	LTexture vSyncTxt;
	LButton optionsBack;
	LTexture optsBackTxt;
	LButton optsSav;
	LTexture optsSavTxt;
	LSlider optionsSS;
	LTexture soundOTxt;
	AVE_vector soundSPos;
	LSlider optionsMS;
	LTexture optionsMSTxt;
	AVE_vector musicSPos;
	AVE_vector vsyncTxtRenderPos;

	/*new game menu block*/
	bool newG_keyframe = false;
	int newG_index = 0;
	SDL_Rect newG_box;
	int newG_neededPos;
	int newG_boxVelosity;
	bool newG_keyframED = false;
	bool newG_keyframEDDown = true;
	bool newG_keyframeDown = false;
	double newG_multFact;
	LMenu newG_menuBox;
	LTextBox newG_saveName;
	LTexture newG_seedTxt;
	AVE_vector newG_seedTxtPos;
	LTextBox newG_seed;
	TTF_Font* newG_font;
	TTF_Font* newG_newgamefont;
	LTexture newG_newGameText;
	AVE_vector newG_newGameTextPos;
	LButton newG_createGameButton;
	LTexture newG_createGameButtonTxt;
	LButton newG_backButton;

	/*load game menu block*/
	vector<string> loadG_saveNames;
	LButton loadG_buttons[255];
	LTexture loadG_tex[255];
	SDL_Rect loadG_box;
	LMenu loadG_menuBox;
	LVertScrollBar loadG_loadBar;
	bool loadG_menuKey_up = false;
	bool loadG_menuKey_upEd = false;
	bool loadG_menuKey_down = false;
	bool loadG_menuKey_downEd = true;
	int loadG_menuKey_index = 0;
	double loadG_menuKey_multFact;
	double loadG_menuKey_boxVel;
	int loadG_menuKey_neededPos;
	LButton loadG_menuDownButton;

	LSButtonSet newG_difficultyButton;
	LTexture newG_difficultyButtonTxt[3];
	TTF_Font* newG_difficultyFont;

	/*extras menu block*/
	LMenu extras;
	LButton extrasMod;
	LButton extrasCredits;
	LTexture extrasCreditsTxt;
	LButton extrasBack;
	LTexture extrasBackTxt;
	bool runCredits = false;
	int creditTime;
	AdvTexture creditTexture;
	int creditXpos = 0;

	SDL_Surface *windowIco;

	fstream file;

	bool stop;
	bool minimized = false;

	bool vSyncEnabled;
	int soundVol;
	int musicVol;

	//Mix_Chunk *clickS = NULL;
	Mix_Music *menuMusic = NULL;

	TTF_Font* version_Font;

	AdvTexture scrollingW;

	void getFileI();

	LTimer fpsTimer;
	int countedFrames = 0;
	LTexture fpsCounter;
	bool displayFps = false;
};

#endif