/*
Version: 0.2
Last changed: 6/29/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#ifndef __TGAMEPLAY_H_INCLUDED
#define __TGAMEPLAY_H_INCLUDED

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <direct.h>
#include <sstream>
#include <vector>
#include <Box2D\Box2D.h>

#include "luaFunctions.h"
#include "LTexture.h"
#include "LButton.h"
#include "LMenu.h"
#include "gPlayer.h"
#include "LSlider.h"
#include "gGameObject.h"
#include "AdvTexture.h"
#include "LToggleButton.h"
#include "LDevConsole.h"
#include "gNpc.h"

using namespace std;

const int ammountOfWeapons = 3;

class TGamePlay
{
public:
	TGamePlay();
	~TGamePlay();

	void setScreenWH(int screenW, int screenH);
	//game init
	void startGame(string sav, SDL_Renderer* mren,SDL_Window* winref, string vNum, TTF_Font* ref_font, bool newG);
	//render stuffs
	void render(SDL_Renderer* mren, SDL_Window* winref);
	//key presses and things
	void handleEvents(SDL_Event* e, SDL_Renderer* mren, SDL_Window* winref, lua_State* _____L);
	//update
	void update(SDL_Renderer* mren,SDL_Window* winref);

	void freeAll();

	bool getFastLoop();

	bool getBTT();
	void setBBT(bool BBT);
	void setPause(bool __paused);
private:
	struct intColor
	{
		int r;
		int g;
		int b;
		int a;
	};
	struct starData
	{
		int posX;
		int posY;
		int size;
		intColor color;
	};
	struct objStruct
	{
		/*int x, y;
		//AdvTexture txt;*/
		SDL_Texture* ctxt;
		SDL_Texture* ctxt2[5];
		/*string name;
		SDL_Color color;*/
	};

	struct planetDat
	{
		//texture info
		SDL_Point truePos;
		int scale;
		int planetRaidus;
		SDL_Color color;
		string name;

		//orbit info
		SDL_Point centerPos;
		double angle;
		int orbitRaidus;
		double orbitSpeed;
	};

	struct sunDat
	{
		int x, y;
		int raidus;
		int scale;
		int ammountOfPlanets;
		string name;
		SDL_Color color;
		planetDat planets[5];
		SDL_Rect solorBox;
		bool generated = false;
	};

	struct chunck
	{
		SDL_Rect chunckRet;
		int MAX_STARS = 32;
		int CHUNCK_WIDTH = 256;
		bool generated = false;
		starData stars[32];
	};

	struct __bulletTypes
	{
		int ammoInClip;
		int ammoPool;
		bulletInfo bInf;
	};

	struct npcWCbox
	{
		gNpc npc;
		b2Body* body;
	};

	void genChunck(chunck* ch);
	void genSolor(sunDat* sunddd, objStruct* txts, SDL_Renderer* mren, SDL_Window* winref, bool startS);

	int chuncksX = 80;
	int chuncksY = 80;
	chunck worldChuncks[80][80];

	string VIRSION_NUM;

	int SCREEN_HEIGHT, SCREEN_WIDTH;
	const int LEVEL_WIDTH = 5120*2;
	const int LEVEL_HEIGHT = 5120*2;
	SDL_Rect cam;
	int game_dif;
	string game_name;
	bool game_vSync;
	int game_soundVol;
	int game_musicVol;
	string game_seed;
	LTexture virsion_Txt;

	fstream file;
	fstream plyFile;

	double currentLoad = 0, maxLoad = 0, maxLoadW = 100;

	//starData bgStarPos[382];
	//vector<starData> starVect;
	objStruct sun;
	//vector<objStruct> sunObjs;
	objStruct sunObjs[100];
	int maxSuns = 100;
	sunDat sunData[100];
	int ammountOfSuns;
	bool genSuns = true;

	//int currentStar = 0;
	bool worldGen = false;

	//HUD
	TTF_Font* HUD_Font;
	TTF_Font* HUD_Font_big;
	LTexture HUD_Ply_Hp;
	LTexture HUD_gameover_txt;
	LTexture HUD_ammo;
	LTexture HUD_reload;
	//AdvTexture HUD_minimap;

	bool paused = false;
	LTexture pauseTxt;
	TTF_Font* pauseF;
	TTF_Font* pauseFSmall;
	LButton pauseButton_quitTMM;
	LTexture pauseButton_quitTMMTxt;
	LButton pauseButton_save;
	LTexture pauseButton_saveTxt;
	LMenu pauseMenu;
	LMenu pauseTxtMenu;
	bool backToMM = false;

	double rotSpeed;
	bool fastLoop = false;

	SDL_Point spawnPoint;
	bool gameover = false;
	int gOverC = 255;

	bool options = false;
	LButton option_button;
	LTexture option_button_txt;
	LButton option_button_back;
	LButton option_button_apply;
	LTexture option_button_apply_txt;
	LTexture option_button_back_txt;
	LSlider options_slider_music;
	LTexture options_slider_music_txt;
	LSlider options_slider_sound;
	LTexture options_slider_sound_txt;
	LToggleButton option_button_vsync;
	LTexture option_button_vsync_txt;

	bool genDat_showChuncks = false;
	bool genDat_showSolorChuncks = false;

	AdvTexture screenTexture;

	LDevConsole devC;

	double zoom = 0;

	bulletInfo bType_MachineGunB;
	bulletInfo bType_Lazer;

	//vector<_projectile> allProjectiles;

	TTF_Font* version_Font;

	//gNpc testEn;

	__bulletTypes weapons[ammountOfWeapons];
	int ammoTypePools[ammountOfWeapons];
	int weaponSelection = 0;

	AdvTexture reloadCircle;

	bool stopReload = false;

	b2World* world = new b2World(b2Vec2(0.0f, 0.0f));

	float M2P = 20;
	float P2M = 1 / M2P;

	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	b2Body* addWRect(int x, int y, int w, int h, b2BodyType type);
	b2Body* plyCBox;
	//b2Body* testNpc;

	const int npcTotal = 100;
	int currentNpcs = 0;
	npcWCbox npcs[100];
};

#endif //__TGAMEPLAY_H_INCLUDED