/*
Version: 0.7
Last changed: 6/30/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#include "TGamePlay.h"
//config stuffs

string _planetNames[7] = { "Erph"/*meh*/, "Jahkoughf"/*subzero*/, "Xonon 5"/*sporky*/, "Frostius"/*frost fangs*/, "Corythus"/*throwing stitch*/, 
						"Tunusia", "nergal"/*professor_fart*/ };

string _starNames[20] = { "Zec-12", "Espon", "Parada 2209", "Acrux-83b", "Proxima Novus", "Lambda Geminus", "Lambda Zealae",
"Pi Odysseus", "Remada 369", "Faua Daigh", "Epsilon Izari", "M33b", "M33a", "M96", "M08", "Entras",
"Tatuine 674a", "Zelone Ion", "Acrux-83a", "John's star" };

SDL_Color sunColors[5] = { { 72, 84, 255, 255 }, { 139, 150, 255, 255 }, { 255, 245, 103, 255 }, { 255, 169, 82, 255 }, { 254, 101, 69, 255 } };

SDL_Color planetColors[5] = { { 47, 153, 0, 255 }, { 0, 192, 190, 255 }, { 0, 131, 222, 255 }, { 194, 194, 194, 255 }, { 197, 0, 0, 255 } };

int ammountOfSColor = 5;
int ammountOfSNames = 20;
int ammountOfPNames = 7;
int ammountOfPColor = 5;

b2Body* TGamePlay::addWRect(int x, int y, int w, int h, b2BodyType type)
{
	b2BodyDef bodydef;
	bodydef.position.Set(x*P2M, y*P2M);

	bodydef.type = type;
	b2Body* body = world->CreateBody(&bodydef);

	b2PolygonShape shape;
	shape.SetAsBox(P2M*w / 2, P2M*h / 2);

	b2FixtureDef fixturedef;
	fixturedef.shape = &shape;
	fixturedef.density = 1.0;
	fixturedef.friction = 1.0;
	body->CreateFixture(&fixturedef);

	return body;
}


SDL_Texture* CreateCircle(int radius, SDL_Renderer* mren, SDL_Window* win, int* w, int* h)
{
	int x = radius;
	int y = 0;
	int xChange = 1 - (radius << 1);
	int yChange = 0;
	int radiusError = 0;
	SDL_Surface* sf = NULL;
	SDL_Texture* txt = NULL;
	void* pixels;
	int pitch = 32;
	int mHeight = 0;
	int mWidth = 0;
	int x0 = radius;
	int y0 = radius;

	sf = SDL_CreateRGBSurface(0, radius * 2 + 1, radius * 2 + 1, pitch, 0, 0, 0, 0);
	txt = SDL_CreateTexture(mren, SDL_GetWindowPixelFormat(win), SDL_TEXTUREACCESS_STREAMING, sf->w, sf->h);
	mWidth = sf->w;
	mHeight = sf->h;
	SDL_FreeSurface(sf);

	SDL_LockTexture(txt, NULL, &pixels, &pitch);
	memcpy(pixels, sf->pixels, pitch * sf->h);

	Uint32* pixels32 = (Uint32*)pixels;
	int pixelCount = (pitch / 4) * mHeight;
	Uint32 color = SDL_MapRGBA(SDL_GetWindowSurface(win)->format, 0xFF, 0x00, 0x00, 0xFF);
	Uint32 seColor = SDL_MapRGBA(SDL_GetWindowSurface(win)->format, 0xFF, 0xFF, 0xFF, 0x00);

	for (int i = 0; i < pixelCount; ++i)
	{
		pixels32[i] = seColor;
	}

	while (x >= y)
	{
		for (int i = x0 - x; i <= x0 + x; i++)
		{
			pixels32[((y0 + y) * mWidth) + i] = color;
			pixels32[((y0 - y) * mWidth) + i] = color;
			//SDL_RenderDrawPoint(mren,i, y0 + y);
			//SDL_RenderDrawPoint(mren, i, y0 - y);
		}
		for (int i = x0 - y; i <= x0 + y; i++)
		{
			pixels32[((y0 + x) * mWidth) + i] = color;
			pixels32[((y0 - x) * mWidth) + i] = color;
			//SDL_RenderDrawPoint(mren, i, y0 + x);
			//SDL_RenderDrawPoint(mren, i, y0 - x);
		}

		y++;
		radiusError += yChange;
		yChange += 2;
		if (((radiusError << 1) + xChange) > 0)
		{
			x--;
			radiusError += xChange;
			xChange += 2;
		}
	}

	SDL_UnlockTexture(txt);

	*w = mWidth;
	*h = mHeight;

	return txt;
}

bool checkColsion(SDL_Rect a, SDL_Rect b)
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

void TGamePlay::genSolor(sunDat* sunddd, objStruct* txts, SDL_Renderer* mren, SDL_Window* winref, bool startS)
{
	int selection = rand() % ammountOfSNames;
	int colorSelection = rand() % ammountOfSColor;
	objStruct sun;

	//gen sun data
	sunddd->name = _starNames[selection];
	sunddd->color = sunColors[colorSelection];
	if (startS)
	{
		sunddd->x = rand() % SCREEN_WIDTH;
		sunddd->y = rand() % SCREEN_HEIGHT;
	}
	sunddd->raidus = rand() % (40 - 10) + 10;
	sunddd->scale = sunddd->raidus * 3;
	sunddd->ammountOfPlanets = rand() % (5 - 1) + 1;

	//gen planets
	for (int ___i___ = 0; ___i___ < sunddd->ammountOfPlanets; ___i___++)
	{
		//string names[4] = { "Erph"/*meh*/, "Jahkoughf"/*subzero*/, "Xonon 5"/*sporky*/, "Frostius"/*frost fangs*/ };
		planetDat tempPlanet;
		//create txt info
		tempPlanet.planetRaidus = rand() % (20 - 5) + 5;
		tempPlanet.scale = tempPlanet.planetRaidus * 3;
		tempPlanet.truePos.x = 0;
		tempPlanet.truePos.y = 0;
		int selection = rand() % ammountOfPNames;
		tempPlanet.name = _planetNames[selection];

		int colorSz = rand() % ammountOfPColor;
		tempPlanet.color = planetColors[colorSz];

		//create orbit info
		tempPlanet.centerPos.x = sunddd->x + sunddd->raidus;
		tempPlanet.centerPos.y = sunddd->y + sunddd->raidus;
		//set planet at random pos on orbit
		tempPlanet.angle = 0 + ((double)rand() / RAND_MAX) * (1 - 0);
		int minRaid = ((sunddd->scale / 2) + (tempPlanet.scale / 2)) + 5;
		//create a orbit raidi
		tempPlanet.orbitRaidus = rand() % (500 - minRaid) + minRaid;
		tempPlanet.orbitSpeed = 0.05 / tempPlanet.orbitRaidus;

		//push planet to sun's planets(save it)
		sunddd->planets[___i___] = tempPlanet;

		//fail safe
		if (txts->ctxt2[___i___] != NULL)
		{
			SDL_DestroyTexture(txts->ctxt2[___i___]);
			txts->ctxt2[___i___] = NULL;
		}

		int mh2, mw2;
		txts->ctxt2[___i___] = CreateCircle(sunddd->raidus, mren, winref, &mw2, &mh2);

		AdvTexture txt;
		txt.loadFromFile("data/img/starHue.png", mren, winref);
		txt.setColor(sunddd->planets[___i___].color.r, sunddd->planets[___i___].color.g, sunddd->planets[___i___].color.b);
		txt.lockTexture();
		void* pixels = txt.getPixels();
		void* circlePix;

		int pitch = 32;
		SDL_LockTexture(txts->ctxt2[___i___], NULL, &circlePix, &pitch);
		int pixelCount = (pitch / 4) * mh2;

		Uint32* txtpixels32 = (Uint32*)pixels;
		Uint32* circlePixpixels32 = (Uint32*)circlePix;

		for (int i = 0; i < pixelCount; i++)
		{
			Uint32 refColor = SDL_MapRGB(SDL_GetWindowSurface(winref)->format, 0xFF, 0x00, 0x00);
			Uint32 transparent = SDL_MapRGBA(SDL_GetWindowSurface(winref)->format, 0, 0, 0, 0);
			if (circlePixpixels32[i] == refColor)
			{
				circlePixpixels32[i] = txtpixels32[(((i + 1) / mw2) * mw2) + i];
			}
			else
			{
				circlePixpixels32[i] = transparent;
			}
		}

		SDL_UnlockTexture(txts->ctxt2[___i___]);
		SDL_SetTextureColorMod(txts->ctxt2[___i___], sunddd->planets[___i___].color.r, sunddd->planets[___i___].color.g, sunddd->planets[___i___].color.b);
		txt.free();
	}

	//create star Txt
	int mw, mh;
	txts->ctxt = CreateCircle(sunddd->raidus, mren, winref, &mw, &mh);

	AdvTexture txt;
	txt.loadFromFile("data/img/starHue.png", mren, winref);
	txt.setColor(sunddd->color.r, sunddd->color.g, sunddd->color.b);
	txt.lockTexture();
	void* pixels = txt.getPixels();
	void* circlePix;

	int pitch = 32;
	SDL_LockTexture(txts->ctxt, NULL, &circlePix, &pitch);
	int pixelCount = (pitch / 4) * mh;

	Uint32* txtpixels32 = (Uint32*)pixels;
	Uint32* circlePixpixels32 = (Uint32*)circlePix;

	//[ ( y * surface->w ) + x ]
	//cut the circle out of the base texture
	for (int i = 0; i < pixelCount; i++)
	{
		Uint32 refColor = SDL_MapRGB(SDL_GetWindowSurface(winref)->format, 0xFF, 0x00, 0x00);
		Uint32 transparent = SDL_MapRGBA(SDL_GetWindowSurface(winref)->format, 0, 0, 0, 0);
		if (circlePixpixels32[i] == refColor)
		{
			circlePixpixels32[i] = txtpixels32[(((i + 1) / mw) * mw) + i];
		}
		else
		{
			circlePixpixels32[i] = transparent;
		}
	}

	SDL_UnlockTexture(txts->ctxt);
	SDL_SetTextureColorMod(txts->ctxt, sunddd->color.r, sunddd->color.g, sunddd->color.b);
	txt.free();

	sunddd->solorBox.x = sunddd->x - 600 + (sunddd->raidus * 2);
	sunddd->solorBox.y = sunddd->y - 600 + (sunddd->raidus * 2);
	sunddd->solorBox.w = (600 * 2);
	sunddd->solorBox.h = (600 * 2);

	sunddd->generated = true;
}

TGamePlay::TGamePlay()
{
	//srand(time(0));
	rotSpeed = 1;
	ammountOfSuns = 0;
	spawnPoint = { LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2 };

	for (int y = 0; y < chuncksY; y++)
	{
		for (int x = 0; x < chuncksX; x++)
		{
			worldChuncks[x][y].chunckRet.x = x * 256;
			worldChuncks[x][y].chunckRet.y = y * 256;
		}
	}

	for (int i = 0; i < maxSuns; i++)
	{
		sunData[i].generated = false;
	}
}

TGamePlay::~TGamePlay()
{
	if (world != NULL)
	{
		b2Free(world);
		world = NULL;
	}

	file.close();
	plyFile.close();
	for (int i = 0; i < ammountOfSuns; i++)
	{
		if (sunObjs[i].ctxt != NULL)
		{
			SDL_DestroyTexture(sunObjs[i].ctxt);
		}
		for (int o = 0; o < sunData[i].ammountOfPlanets; o++)
		{
			if (sunObjs[i].ctxt2[o] != NULL)
			{
				SDL_DestroyTexture(sunObjs[i].ctxt2[o]);
				sunObjs[i].ctxt2[o] = NULL;
			}
		}
	}
	freeAll();
}

void TGamePlay::genChunck(chunck* ch)
{
	if (!ch->generated)
	{
		ch->chunckRet.w = ch->CHUNCK_WIDTH;
		ch->chunckRet.h = ch->CHUNCK_WIDTH;

		for (int i = 0; i < ch->MAX_STARS; i++)
		{
			//when rendering add the chunck's pos to the star's pos
			//generates stars pos relative to chuncks pos
			ch->stars[i].posX = rand() % ch->CHUNCK_WIDTH;
			ch->stars[i].posY = rand() % ch->CHUNCK_WIDTH;

			int colorS = rand() % 100;

			if (colorS >= 0 && colorS <= 94)
			{
				ch->stars[i].color = { 255, 255, 255, 255 };
			}
			else if (colorS == 95)
			{
				ch->stars[i].color = { 170, 0, 0, 255 };
			}
			else if (colorS == 96)
			{
				ch->stars[i].color = { 0, 66, 200, 255 };
			}
			else if (colorS == 97)
			{
				ch->stars[i].color = { 150, 0, 0, 255 };
			}
			else if (colorS == 98)
			{
				ch->stars[i].color = { 221, 213, 0, 255 };
			}
			else if (colorS == 99)
			{
				ch->stars[i].color = { 211, 119, 0, 255 };
			}

			int scaleS = rand() % 3 + 2;
			ch->stars[i].size = scaleS;

			ch->stars[i].color.a = rand() % (255 - 50) + 50;
		}
		ch->generated = true;
	}
}

void TGamePlay::setScreenWH(int screenW, int screenH)
{
	SCREEN_HEIGHT = screenH;
	SCREEN_WIDTH = screenW;
}

void TGamePlay::startGame(string sav, SDL_Renderer* mren, SDL_Window* winref, string vNum, TTF_Font* ref_font, bool newG)
{
	ply.startShootTimer();
	
	worldGen = false;
	genSuns = true;
	//currentStar = 0;
	currentLoad = 0;
	VIRSION_NUM = vNum;
	game_name = sav;
	//temp values
	fstream saveFile;
	string numCon;
	fstream fileSIn;

	fileSIn.open("cfg/options.cfg", ios::in | ios::app | ios::binary);
	if (fileSIn.is_open())
	{
		fileSIn >> game_soundVol >> game_musicVol >> game_vSync;
	}
	else
	{
		cout << "unable to open options file reseting to default!" << endl;
		game_soundVol = 100;
		game_musicVol = 100;
		game_vSync = true;
	}
	fileSIn.close();

	//open savefile data
	saveFile.open("sav/" + sav + "/data.sav", ios::in);

	if (saveFile.is_open())
	{
		//get data
		getline(saveFile, game_name);
		getline(saveFile, numCon);
		getline(saveFile, game_seed);

		//convert data
		game_dif = atoi(numCon.c_str());
	}
	else
	{
		saveFile.close();
		saveFile.open("sav/" + sav + "/data.sav", ios::trunc | ios::out);
		cout << "Unable to open savefile! (it might be corrupt!) generating new data file!" << endl;
		string seed;
		seed = "1234567891234567";
		saveFile << sav << endl << 0 << endl << seed << endl;
		game_dif = 0;
		game_seed = seed;
	}

	//use seed
	unsigned int randSeed = 0;
	for (int rs = 0; rs < game_seed.length(); rs++)
	{
		randSeed += int(game_seed[rs]);
	}
	srand(randSeed);

	//close save file
	saveFile.close();

	//load textures
	if (!screenTexture.createBlank(mren, winref, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_TEXTUREACCESS_TARGET))
	{
		cout << "Unable to create target texture! " << SDL_GetError() << endl;
	}

	if (world != NULL)
	{
		b2Free(world);
		world = NULL;
	}

	world = new b2World(b2Vec2(0, 0));

	//HUD_minimap.createBlank(mren, winref, 200, 200, SDL_TEXTUREACCESS_TARGET);

	pauseF = TTF_OpenFont("data/ERASMD.TTF", 40);
	pauseFSmall = TTF_OpenFont("data/ERASMD.TTF", 20);
	ply.playerTxt.loadFromFile("data/img/pointTXT.png", mren);
	plyCBox = addWRect(ply.getPlyDiment().x, ply.getPlyDiment().y, ply.getPlyDiment().w, ply.getPlyDiment().h, b2_dynamicBody);

	version_Font = TTF_OpenFont("data/ERASDEMI.TTF", 31);

	virsion_Txt.loadFromTtf(VIRSION_NUM, version_Font, { 255, 255, 255, 255 }, mren);

	TTF_CloseFont(version_Font);

	pauseTxt.loadFromTtf("Paused", pauseF, { 255, 255, 255, 255 }, mren);

	HUD_Font = TTF_OpenFont("data/ERASMD.TTF", 20);
	HUD_Font_big = TTF_OpenFont("data/ERASMD.TTF", 40);
	HUD_Ply_Hp.loadFromTtf("100", HUD_Font, { 255, 0, 0, 255 }, mren);
	HUD_gameover_txt.loadFromTtf("Game Over!", HUD_Font_big, { 255, 255, 255, 255 }, mren);
	HUD_ammo.loadFromTtf(to_string(ply.getAmmoInClip()) + "/" + to_string(ply.getAmmoPool()), HUD_Font_big, { 255, 255, 0, 255 }, mren);
	HUD_reload.loadFromTtf("Reloading", HUD_Font, { 255, 255, 255, 255 }, mren);

	pauseMenu.setScreenWH(SCREEN_WIDTH, SCREEN_HEIGHT);
	pauseMenu.setClips({ 0, 0, 17, 16 }, { 0, 17, 17, 2 });
	pauseMenu.setMenuDiment({ (SCREEN_WIDTH - (SCREEN_WIDTH / 2)) / 2, (SCREEN_HEIGHT - (SCREEN_HEIGHT / 2)) / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	pauseMenu.loadTextures("data/img/menutxt2.png", "data/img/menutxt2.png", mren);

	pauseButton_quitTMM.setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
	pauseButton_quitTMM.loadFromfile("data/img/newbutton.png", mren);
	pauseButton_quitTMM.setPos(pauseMenu.getDiment().x + (pauseMenu.getDiment().w - pauseButton_quitTMM.getWidth()) / 2 + (pauseButton_quitTMM.getWidth() / 2) + 5, pauseMenu.getDiment().y + pauseMenu.getDiment().h - pauseButton_quitTMM.getHeight() - 15);
	pauseButton_quitTMMTxt.loadFromTtf("Back to Main Menu", pauseFSmall, { 255, 255, 255, 255 }, mren);

	pauseTxtMenu.setScreenWH(SCREEN_WIDTH, SCREEN_HEIGHT);
	pauseTxtMenu.setClips({ 0, 0, 17, 16 }, { 0, 17, 17, 2 });
	pauseTxtMenu.setMenuDiment({ (SCREEN_WIDTH - pauseMenu.getDiment().w) / 2, pauseMenu.getDiment().y - (pauseTxt.getHeight()) - 2, pauseMenu.getDiment().w, pauseTxt.getHeight() });
	pauseTxtMenu.loadTextures("data/img/menutxt2.png", "data/img/menutxt2.png", mren);

	pauseButton_save.setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
	pauseButton_save.loadFromfile("data/img/newbutton.png", mren);
	pauseButton_save.setPos(pauseMenu.getDiment().x + (pauseMenu.getDiment().w - pauseButton_save.getWidth()) / 2 - (pauseButton_save.getWidth() / 2) - 5, pauseMenu.getDiment().y + pauseMenu.getDiment().h - pauseButton_save.getHeight() - 15);
	pauseButton_saveTxt.loadFromTtf("Save game", pauseFSmall, { 255, 255, 255, 255 }, mren);

	option_button.setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
	option_button.loadFromfile("data/img/newbutton.png", mren);
	option_button.setPos(pauseMenu.getDiment().x + ((pauseMenu.getDiment().w - option_button.getWidth()) / 2), pauseButton_save.getY() - option_button.getHeight() - 15);
	option_button_txt.loadFromTtf("Options", pauseFSmall, { 255, 255, 255, 255 }, mren);

	option_button_back.setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
	option_button_back.loadFromfile("data/img/newbutton.png", mren);
	option_button_back.setPos(pauseMenu.getDiment().x + ((pauseMenu.getDiment().w - option_button_back.getWidth()) / 2) + (option_button_back.getWidth() / 2)+5, pauseMenu.getDiment().y + pauseMenu.getDiment().h - option_button_back.getHeight() - 15);
	option_button_back_txt.loadFromTtf("Back", pauseFSmall, { 255, 255, 255, 255 }, mren);

	option_button_apply.setClips({ 0, 0 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 }, { 0, 2 * 93, 250, 93 }, { 0, 1 * 93, 250, 93 });
	option_button_apply.loadFromfile("data/img/newbutton.png", mren);
	option_button_apply.setPos(pauseMenu.getDiment().x + ((pauseMenu.getDiment().w - option_button_apply.getWidth()) / 2) - (option_button_apply.getWidth() / 2)-5, pauseMenu.getDiment().y + pauseMenu.getDiment().h - option_button_apply.getHeight() - 15);
	option_button_apply_txt.loadFromTtf("Apply", pauseFSmall, { 255, 255, 255, 255 }, mren);

	options_slider_music.dash.setClips({ 32, 0 * 12, 6, 12 }, { 32, 1 * 12, 6, 12 }, { 32, 2 * 12, 6, 12 }, { 32, 1 * 12, 6, 12 });
	options_slider_music.dash.loadFromfile("data/img/slider.png", mren);
	options_slider_music.setPos({ pauseMenu.getDiment().x + ((pauseMenu.getDiment().w - 200) / 2) + (200 / 2) + 48, (SCREEN_HEIGHT - 32) / 2 - (32 * 2), 200, 32 });
	options_slider_music.setSValue(game_musicVol);
	options_slider_music.setClips({ 0, 0, 32, 32 }, { 0, 32, 32, 4 });
	options_slider_music.setTextures("data/img/slider.png", "data/img/slider.png", mren);
	options_slider_music_txt.loadFromTtf("Music Volume", pauseFSmall, {255,255,255,255}, mren);

	options_slider_sound.dash.setClips({ 32, 0 * 12, 6, 12 }, { 32, 1 * 12, 6, 12 }, { 32, 2 * 12, 6, 12 }, { 32, 1 * 12, 6, 12 });
	options_slider_sound.dash.loadFromfile("data/img/slider.png", mren);
	options_slider_sound.setPos({ pauseMenu.getDiment().x + ((pauseMenu.getDiment().w - 200) / 2) - (200 / 2) - 48, (SCREEN_HEIGHT - 32) / 2 - (32 * 2), 200, 32 });
	options_slider_sound.setSValue(game_soundVol);
	options_slider_sound.setClips({ 0, 0, 32, 32 }, { 0, 32, 32, 4 });
	options_slider_sound.setTextures("data/img/slider.png", "data/img/slider.png", mren);
	options_slider_sound_txt.loadFromTtf("Sound Volume", pauseFSmall, { 255, 255, 255, 255 }, mren);

	option_button_vsync_txt.loadFromTtf("vSync enabled", pauseFSmall, { 255, 255, 255, 255 }, mren);
	option_button_vsync.button.setClips({ 0, 0 * 32, 32, 32 }, { 0, 1 * 32, 32, 32 }, { 0, 2 * 32, 32, 32 }, { 0, 1 * 32, 32, 32 });
	option_button_vsync.button.loadFromfile("data/img/toggleBT.png", mren);
	option_button_vsync.button.setPos((SCREEN_WIDTH - option_button_vsync.button.getWidth()) / 2 + (option_button_vsync_txt.getWidth() / 2), (SCREEN_HEIGHT - option_button_vsync.button.getHeight()) / 2);
	option_button_vsync.setClips({ 0, 0, 32, 32 }, { 0, 32, 32, 32 });
	option_button_vsync.loadTextures("data/img/toggleButton.png", mren);
	option_button_vsync.setToggled(game_vSync);

	//testEn.texture.loadFromFile("data/img/pointTXT.png", mren);
	//testNpc = addWRect(0, 0, testEn.texture.getWidth(), testEn.texture.getHeight(), b2_dynamicBody);

	npcs[0].npc.texture.loadFromFile("data/img/pointTXT.png", mren);
	npcs[0].npc.setHP(100);
	npcs[0].body = addWRect(0, 0, npcs[0].npc.texture.getWidth(), npcs[0].npc.texture.getHeight(), b2_dynamicBody);

	currentNpcs = 1;

	devC.setMaxMsg(12);
	devC.setConsoleRect({ 0, 0, 500, 225 }, SCREEN_HEIGHT, SCREEN_WIDTH, mren);

	TTF_CloseFont(pauseF);
	TTF_CloseFont(pauseFSmall);

	bType_MachineGunB.bulletsPerShot = 1;
	bType_MachineGunB.clipSize = 100;
	bType_MachineGunB.reloadTime = 3000;
	bType_MachineGunB.bulletDelay = 50;
	bType_MachineGunB.bulletSpread = 20;
	bType_MachineGunB.damagePerHit = 1.5;
	bType_MachineGunB.height = 20;
	bType_MachineGunB.width = 20;
	bType_MachineGunB.type = BULLET_METAL;

	bType_Lazer.bulletsPerShot = 1;
	bType_Lazer.clipSize = 100;
	bType_Lazer.reloadTime = 3000;
	bType_Lazer.bulletDelay = 50;
	bType_Lazer.bulletSpread = 20;
	bType_Lazer.damagePerHit = 1.5;
	bType_Lazer.height = 20;
	bType_Lazer.width = 20;
	bType_Lazer.type = BULLET_PLASMA;

	ammoTypePools[BULLET_METAL] = 400;
	ammoTypePools[BULLET_PLASMA] = 60;
	ammoTypePools[BULLET_EMP] = 10;

	weapons[0].bInf = bType_MachineGunB;
	weapons[0].ammoInClip = bType_MachineGunB.clipSize;
	weapons[0].ammoPool = ammoTypePools[bType_MachineGunB.type];

	weapons[1].bInf = bType_Lazer;
	weapons[1].ammoInClip = bType_Lazer.clipSize;
	weapons[1].ammoPool = ammoTypePools[bType_Lazer.type];

	//testEn.setBulletType(bType_MachineGunB);
	npcs[0].npc.setBulletType(bType_MachineGunB);

	cam = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	if (newG)
	{
		ply.setPlyPos(spawnPoint.x, spawnPoint.y, P2M, plyCBox);

		ammountOfSuns = 1;
		//create pos file
		stringstream posFile;

		posFile << "sav/" << sav << "/pos";
		string posFileStr = posFile.str(), plyPosFile;

		_mkdir(posFileStr.c_str());
		posFileStr = posFile.str() + "/starDat.data";
		plyPosFile = posFile.str() + "/playerDat.data";

		ply.setHealth(100);

		file.open(posFileStr.c_str(), ios::out | ios::trunc);
		//plyFile.open(plyPosFile.c_str(), ios::out | ios::trunc);
		//plyFile << ply.getPlyDiment().x << " " << ply.getPlyDiment().y << " " << ply.getVel().x << " " << ply.getVel().y << " " << rotSpeed << " " << ply.getHealth() << endl;
		
		SDL_RWops* __file = SDL_RWFromFile(("sav/" + game_name + "/pos/playerDat.data").c_str(), "w+b");
		if (__file != NULL)
		{
			SDL_Rect posRef = ply.getPlyDiment();
			int hp = ply.getHealth();
			SDL_Point vel = ply.getVel();
			SDL_RWwrite(__file, &plyCBox->GetPosition().x, sizeof(plyCBox->GetPosition().x), 1);
			SDL_RWwrite(__file, &plyCBox->GetPosition().y, sizeof(plyCBox->GetPosition().y), 1);
			SDL_RWwrite(__file, &plyCBox->GetLinearVelocity().x, sizeof(plyCBox->GetLinearVelocity().x), 1);
			SDL_RWwrite(__file, &plyCBox->GetLinearVelocity().y, sizeof(plyCBox->GetLinearVelocity().y), 1);
			SDL_RWwrite(__file, &rotSpeed, sizeof(rotSpeed), 1);
			SDL_RWwrite(__file, &hp, sizeof(hp), 1);
			SDL_RWwrite(__file, &weaponSelection, sizeof(weaponSelection), 1);
			SDL_RWwrite(__file, &weapons, sizeof(weapons), 1);
			SDL_RWwrite(__file, &ammoTypePools, sizeof(ammoTypePools), 1);
			SDL_RWclose(__file);
		}
		else
		{
			cout << "Error: Unable to create file! SDL Error: " << SDL_GetError() << endl;
		}

		ply.setAmmoPool(weapons[0].ammoPool);
		ply.setAmmoInPool(weapons[0].ammoInClip);
		
		//plyFile.close();
	}
	else
	{
		//load pos file
		stringstream inputF;
		stringstream inputPlyF;
		string inputFTrans;

		inputF << "sav/" << sav << "/pos/starDat.data";
		inputPlyF << "sav/" << sav << "/pos/playerDat.data";
		inputFTrans = inputF.str();
		//("sav/" + game_name + "/pos/sunDat.data").c_str()
		SDL_RWops* fileOps = SDL_RWFromFile(("sav/" + game_name + "/pos/sunDat.data").c_str(), "r+b");
		if (fileOps != NULL)
		{
			SDL_RWread(fileOps, &sunData, sizeof(sunData), 1);
			SDL_RWread(fileOps, &ammountOfSuns, sizeof(ammountOfSuns), 1);
			SDL_RWclose(fileOps);
			//create textures(because of random crashes)
			for (int _i = 0; _i < ammountOfSuns; _i++)
			{
				if (sunObjs[_i].ctxt != NULL)
					SDL_DestroyTexture(sunObjs[_i].ctxt);

				for (int iii = 0; iii < sunData[_i].ammountOfPlanets; iii++)
				{
					if (sunObjs[_i].ctxt2[iii] != NULL)
					{
						SDL_DestroyTexture(sunObjs[_i].ctxt2[iii]);
						sunObjs[_i].ctxt2[iii] = NULL;
					}

					int mh2, mw2;
					sunObjs[_i].ctxt2[iii] = CreateCircle(sunData[_i].raidus, mren, winref, &mw2, &mh2);

					AdvTexture txt;
					txt.loadFromFile("data/img/starHue.png", mren, winref);
					txt.setColor(sunData[_i].planets[iii].color.r, sunData[_i].planets[iii].color.g, sunData[_i].planets[iii].color.b);
					txt.lockTexture();
					void* pixels = txt.getPixels();
					void* circlePix;

					int pitch = 32;
					SDL_LockTexture(sunObjs[_i].ctxt2[iii], NULL, &circlePix, &pitch);
					int pixelCount = (pitch / 4) * mh2;

					Uint32* txtpixels32 = (Uint32*)pixels;
					Uint32* circlePixpixels32 = (Uint32*)circlePix;

					for (int i = 0; i < pixelCount; i++)
					{
						Uint32 refColor = SDL_MapRGB(SDL_GetWindowSurface(winref)->format, 0xFF, 0x00, 0x00);
						Uint32 transparent = SDL_MapRGBA(SDL_GetWindowSurface(winref)->format, 0, 0, 0, 0);
						if (circlePixpixels32[i] == refColor)
						{
							circlePixpixels32[i] = txtpixels32[(((i + 1) / mw2) * mw2) + i];
						}
						else
						{
							circlePixpixels32[i] = transparent;
						}
					}

					SDL_UnlockTexture(sunObjs[_i].ctxt2[iii]);
					SDL_SetTextureColorMod(sunObjs[_i].ctxt2[iii], sunData[_i].planets[iii].color.r, sunData[_i].planets[iii].color.g, sunData[_i].planets[iii].color.b);
					txt.free();
				}

				int mw, mh;
				sunObjs[_i].ctxt = CreateCircle(sunData[_i].raidus, mren, winref, &mw, &mh);

				//create star Txt
				AdvTexture txt;
				txt.loadFromFile("data/img/starHue.png", mren, winref);
				txt.setColor(sunData[_i].color.r, sunData[_i].color.g, sunData[_i].color.b);
				txt.lockTexture();
				void* pixels = txt.getPixels();
				void* circlePix;

				int pitch = 32;
				SDL_LockTexture(sunObjs[_i].ctxt, NULL, &circlePix, &pitch);
				int pixelCount = (pitch / 4) * mh;

				Uint32* txtpixels32 = (Uint32*)pixels;
				Uint32* circlePixpixels32 = (Uint32*)circlePix;

				int off = rand() % (100 - 1) + 1;

				//[ ( y * surface->w ) + x ]
				//cut the circle out of the base texture
				for (int i = 0; i < pixelCount; i++)
				{
					Uint32 refColor = SDL_MapRGB(SDL_GetWindowSurface(winref)->format, 0xFF, 0x00, 0x00);
					Uint32 transparent = SDL_MapRGBA(SDL_GetWindowSurface(winref)->format, 0, 0, 0, 0);
					if (circlePixpixels32[i] == refColor)
					{
						circlePixpixels32[i] = txtpixels32[(((i + 1) / mw) * mw) + i];
					}
					else
					{
						circlePixpixels32[i] = transparent;
					}
				}

				SDL_UnlockTexture(sunObjs[_i].ctxt);
				SDL_SetTextureColorMod(sunObjs[_i].ctxt, sunData[_i].color.r, sunData[_i].color.g, sunData[_i].color.b);
				txt.free();
			}
			genSuns = false;
		}
		else
		{
			cout << "Error: Unable to read file! SDL Error: " << SDL_GetError() << endl;
			genSuns = true;
		}

		SDL_RWops* ____file = SDL_RWFromFile(("sav/" + game_name + "/pos/starDat.data").c_str(), "r+b");
		if (____file != NULL)
		{
			for (int y = 0; y < chuncksY; y++)
			{
				for (int x = 0; x < chuncksX; x++)
				{
					SDL_RWread(____file, &worldChuncks[x][y].generated, sizeof(worldChuncks[x][y].generated), 1);
					if (worldChuncks[x][y].generated)
					{
						SDL_RWread(____file, &worldChuncks[x][y].chunckRet, sizeof(worldChuncks[x][y].chunckRet), 1);
						SDL_RWread(____file, &worldChuncks[x][y].stars, sizeof(worldChuncks[x][y].stars), 1);
					}
				}
			}
			SDL_RWclose(____file);

			SDL_RWops* ____file = SDL_RWFromFile(("sav/" + game_name + "/pos/playerDat.data").c_str(), "r+b");
			if (____file != NULL)
			{
				SDL_Rect posRef;
				int hp;
				SDL_Point vel;

				float32 _posX;
				float32 _posY;
				float32 _vecX;
				float32 _vecY;

				SDL_RWread(____file, &_posX, sizeof(_posX), 1);
				SDL_RWread(____file, &_posY, sizeof(_posY), 1);
				SDL_RWread(____file, &_vecX, sizeof(_vecX), 1);
				SDL_RWread(____file, &_vecY, sizeof(_vecY), 1);

				plyCBox->SetTransform(b2Vec2(_posX, _posY), plyCBox->GetAngle());
				plyCBox->SetLinearVelocity(b2Vec2(_vecX, _vecY));
				SDL_RWread(____file, &rotSpeed, sizeof(rotSpeed), 1);
				SDL_RWread(____file, &hp, sizeof(hp), 1);
				SDL_RWread(____file, &weaponSelection, sizeof(weaponSelection), 1);
				SDL_RWread(____file, &weapons, sizeof(weapons), 1);
				SDL_RWread(____file, &ammoTypePools, sizeof(ammoTypePools), 1);

				ply.setAmmoPool(weapons[weaponSelection].ammoPool);
				ply.setAmmoInPool(weapons[weaponSelection].ammoInClip);

				ply.setHealth(hp);

				SDL_RWclose(____file);
			}
			else
			{
				cout << "Error: Unable to create file! SDL Error: " << SDL_GetError() << endl;
				ply.setPlyPos(spawnPoint.x, spawnPoint.y, P2M, plyCBox);
				ply.setPlyVel(0, 0);
				ply.setHealth(100);
			}

		}
		else
		{
			for (int x = 0; x < 5; x++)
			{
				for (int y = 0; y < 5; y++)
				{
					worldChuncks[x][y].generated = false;
				}
			}

			file.close();
			//create pos file
			stringstream posFile;

			posFile << "sav/" << sav << "/pos";
			string posFileStr = posFile.str();

			_mkdir(posFileStr.c_str());
			posFileStr = posFile.str() + "/starDat.data";

			file.open(posFileStr.c_str(), ios::out | ios::trunc);
			cout << "unable to load star pos file! generating new world!" << endl;
			worldGen = false;

			SDL_RWops* ____file = SDL_RWFromFile(("sav/" + game_name + "/pos/playerDat.data").c_str(), "r+b");
			if (____file != NULL)
			{
				SDL_Rect posRef = { 0, 0, 0, 0 };
				int hp;
				SDL_Point vel = { 0, 0 };

				float32 _posX;
				float32 _posY;
				float32 _vecX;
				float32 _vecY;

				SDL_RWread(____file, &_posX, sizeof(_posX), 1);
				SDL_RWread(____file, &_posY, sizeof(_posY), 1);
				SDL_RWread(____file, &_vecX, sizeof(_vecX), 1);
				SDL_RWread(____file, &_vecY, sizeof(_vecY), 1);

				plyCBox->SetTransform(b2Vec2(_posX, _posY), plyCBox->GetAngle());
				plyCBox->SetLinearVelocity(b2Vec2(_vecX, _vecY));

				SDL_RWread(____file, &rotSpeed, sizeof(rotSpeed), 1);
				SDL_RWread(____file, &hp, sizeof(hp), 1);
				SDL_RWread(____file, &weaponSelection, sizeof(weaponSelection), 1);
				SDL_RWread(____file, &weapons, sizeof(weapons), 1);
				SDL_RWread(____file, &ammoTypePools, sizeof(ammoTypePools), 1);

				ply.setHealth(hp);

				ply.setAmmoPool(weapons[weaponSelection].ammoPool);
				ply.setAmmoInPool(weapons[weaponSelection].ammoInClip);

				SDL_RWclose(____file);
			}
			else
			{
				cout << "Error: Unable to create file! SDL Error: " << SDL_GetError() << endl;
				ply.setPlyPos(spawnPoint.x, spawnPoint.y, P2M, plyCBox);
				ply.setPlyVel(0, 0);
				ply.setHealth(100);
			}
		}
	}
}

int rotation = 0;

void TGamePlay::render(SDL_Renderer* mren, SDL_Window* winref)
{
	if (worldGen && !genSuns)
	{
		screenTexture.setAsRenderTarget(mren);
		SDL_SetRenderDrawColor(mren, 0, 0, 0, 255);
		SDL_Rect bgRef = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderFillRect(mren, &bgRef);

		SDL_SetRenderDrawBlendMode(mren, SDL_BLENDMODE_BLEND);

		//render chuncks
		for (int y = AvMath::rnd::roundUp(cam.y, 256) * 0.001953125; y < AvMath::rnd::roundUp(cam.y + cam.h, 256) * 0.00390625; y++)
		{
			for (int x = AvMath::rnd::roundUp(cam.x, 256) * 0.001953125; x < AvMath::rnd::roundUp(cam.x + cam.w, 256) * 0.00390625; x++)
			{
				if (worldChuncks[x][y].generated && checkColsion(cam, worldChuncks[x][y].chunckRet))
				{
					//to fix access crashes
					if (x <= 0)
						x = 0;
					if (y <= 0)
						y = 0;

					for (int s = 0; s < worldChuncks[x][y].MAX_STARS; s++)
					{
						SDL_SetRenderDrawColor(mren, worldChuncks[x][y].stars[s].color.r, worldChuncks[x][y].stars[s].color.g, worldChuncks[x][y].stars[s].color.b, worldChuncks[x][y].stars[s].color.a);
						SDL_Rect renderR = { (worldChuncks[x][y].stars[s].posX + worldChuncks[x][y].chunckRet.x) - cam.x, (worldChuncks[x][y].stars[s].posY + worldChuncks[x][y].chunckRet.y) - cam.y, worldChuncks[x][y].stars[s].size, worldChuncks[x][y].stars[s].size };
						SDL_RenderFillRect(mren, &renderR);
					}
					if (genDat_showChuncks)
					{
						SDL_SetRenderDrawColor(mren, 255, 0, 0, 255);
						SDL_Rect chunckR;
						chunckR.x = worldChuncks[x][y].chunckRet.x + 1 - cam.x;
						chunckR.y = worldChuncks[x][y].chunckRet.y + 1 - cam.y;
						chunckR.w = worldChuncks[x][y].CHUNCK_WIDTH - 2;
						chunckR.h = worldChuncks[x][y].CHUNCK_WIDTH - 2;
						SDL_RenderDrawRect(mren, &chunckR);
						SDL_SetRenderDrawColor(mren, 255, 255, 255, 255);
					}
				}
			}
		}

		for (int i = 0; i < ammountOfSuns; i++)
		{
			SDL_Rect dest = { sunData[i].x - cam.x, sunData[i].y - cam.y, sunData[i].scale, sunData[i].scale };
			SDL_RenderCopy(mren, sunObjs[i].ctxt, NULL, &dest);
		}
		SDL_SetRenderDrawBlendMode(mren, SDL_BLENDMODE_NONE);

		for (int __i = 0; __i < ammountOfSuns; __i++)
		{
			for (int o = 0; o < sunData[__i].ammountOfPlanets; o++)
			{
				//SDL_SetRenderDrawColor(mren, 255, 0, 0, 255);
				SDL_Rect dref = { sunData[__i].planets[o].truePos.x - cam.x, sunData[__i].planets[o].truePos.y - cam.y, sunData[__i].planets[o].scale, sunData[__i].planets[o].scale };
				//SDL_RenderFillRect(mren, &dref);
				//SDL_SetRenderDrawColor(mren, 255, 255, 255, 255);
				SDL_RenderCopy(mren, sunObjs[__i].ctxt2[o], NULL, &dref);
			}
		}

		for (int _i = 0; _i < maxSuns; _i++)
		{
			if (genDat_showSolorChuncks)
			{
				SDL_SetRenderDrawColor(mren, 0, 0, 255, 255);
				SDL_Rect solorBoxref;
				solorBoxref.x = sunData[_i].solorBox.x - cam.x;
				solorBoxref.y = sunData[_i].solorBox.y - cam.y;
				solorBoxref.w = sunData[_i].solorBox.w;
				solorBoxref.h = sunData[_i].solorBox.h;
				SDL_RenderDrawRect(mren, &solorBoxref);
				SDL_SetRenderDrawColor(mren, 255, 255, 255, 255);
			}
		}

		ply.render(mren, cam.x, cam.y, plyCBox, M2P);
		//testEn.render(mren, cam, testNpc, M2P);
		npcs[0].npc.render(mren, cam, npcs[0].body, M2P);

		SDL_SetRenderTarget(mren, NULL);
		SDL_Point screenCenter = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		screenTexture.render(0 - (zoom * 1.77777777777777) , 0 - zoom, mren, SCREEN_WIDTH + ((zoom * 1.77777777777777) * 2), SCREEN_HEIGHT + (zoom * 2), NULL);

		devC.render(mren);

		virsion_Txt.render(SCREEN_WIDTH - virsion_Txt.getWidth(), 0, mren, NULL, NULL, NULL, SDL_FLIP_NONE);

		HUD_Ply_Hp.render(0, SCREEN_HEIGHT - HUD_Ply_Hp.getHeight(), mren, NULL, NULL, NULL, SDL_FLIP_NONE);
		HUD_ammo.render(SCREEN_WIDTH - HUD_ammo.getWidth(), SCREEN_HEIGHT - HUD_ammo.getHeight(), mren, NULL, NULL, NULL, SDL_FLIP_NONE);
		//HUD_minimap.render(SCREEN_WIDTH - HUD_minimap.getWidth() - 15, SCREEN_HEIGHT - HUD_minimap.getHeight() - 15, mren);
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (ply.getReload())
		{
			//reload animation
			double color = (double)cos((double)SDL_GetTicks() / (double)200) * (double)63.75 + (double)191.25;
			HUD_reload.loadFromTtf("Reloading", HUD_Font, { color, color, color, 255 }, mren);
			HUD_reload.render(x - (HUD_reload.getWidth() / 2), y + HUD_reload.getHeight(), mren);
			HUD_reload.setBlendMode(SDL_BLENDMODE_BLEND);

			if (!paused)
			{
				int cW = (SDL_GetTicks() - ply.getReloadStartTime()) * (50 / weapons[weaponSelection].bInf.reloadTime);

				if (cW != 0)
					reloadCircle.createBlank(mren, winref, cW, 50, SDL_TEXTUREACCESS_TARGET);
				else
					reloadCircle.createBlank(mren, winref, 1, 50, SDL_TEXTUREACCESS_TARGET);
				reloadCircle.setAsRenderTarget(mren);
				SDL_SetRenderDrawColor(mren, 0, 0, 0, 0);
				SDL_RenderClear(mren);
			}

			LTexture texture;
			texture.loadFromFile("data/img/circle.png", mren);

			if (!paused)
			{
				texture.render(0, 0, mren);

				SDL_SetRenderTarget(mren, NULL);
			}
			reloadCircle.setBlendMode(SDL_BLENDMODE_BLEND);
			reloadCircle.render(x - (texture.getWidth() / 2), y - (texture.getHeight() / 2), mren);
			texture.free();
		}

		if (gameover)
		{
			if (gOverC >= 64)
			{
				gOverC -= 5;
			}
			SDL_SetRenderDrawBlendMode(mren, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(mren, gOverC, gOverC, gOverC, gOverC);
			SDL_Rect ref = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			SDL_RenderFillRect(mren, &ref);
			HUD_gameover_txt.render((SCREEN_WIDTH - HUD_gameover_txt.getWidth()) / 2, (SCREEN_HEIGHT - HUD_gameover_txt.getHeight()) / 2, mren, NULL, NULL, NULL, SDL_FLIP_NONE);
			SDL_SetRenderDrawColor(mren, 255, 255, 255, 255);
		}

		if (!paused)
			world->Step(timeStep, velocityIterations, positionIterations);

		//sunObjs[0].txt.render(sunObjs[0].x - cam.x, sunObjs[0].y - cam.y, mren);
	}
	if (!worldGen)
	{
		//loading screen here
		SDL_SetRenderDrawColor(mren, 0, 255, 0, 255);

		SDL_Rect __loadRect = { 0, 0, currentLoad * (maxLoadW / maxLoad) + 20, 50 };
		SDL_RenderFillRect(mren, &__loadRect);

		SDL_SetRenderDrawColor(mren, 0, 0, 0, 255);

		SDL_Rect __endBar1 = { 0, 0, 20, 50 };
		SDL_RenderFillRect(mren, &__endBar1);
		SDL_Rect __endBar2 = { 100 + 20, 0, 20, 50 };
		SDL_RenderFillRect(mren, &__endBar2);

		SDL_SetRenderDrawColor(mren, 255, 255, 255, 255);
	}
	if (paused)
	{
		SDL_Rect bgPauseRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_SetRenderDrawBlendMode(mren, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(mren, 0, 0, 0, 255 / 2);

		SDL_RenderFillRect(mren, &bgPauseRect);

		SDL_SetRenderDrawColor(mren, 255, 255, 255, 255);
		SDL_SetRenderDrawBlendMode(mren, SDL_BLENDMODE_NONE);

		pauseTxt.render(pauseTxtMenu.getDiment().x + (pauseTxtMenu.getDiment().w - pauseTxt.getWidth()) / 2, pauseTxtMenu.getDiment().y + (pauseTxtMenu.getDiment().h - pauseTxt.getHeight()) / 2, mren, NULL, NULL, NULL, SDL_FLIP_NONE);

		pauseMenu.render(mren);
		if (!options)
		{
			pauseButton_quitTMM.render(mren);
			pauseButton_quitTMMTxt.render(pauseButton_quitTMM.getX() + (pauseButton_quitTMM.getWidth() - pauseButton_quitTMMTxt.getWidth()) / 2, pauseButton_quitTMM.getY() + (pauseButton_quitTMM.getHeight() - pauseButton_quitTMMTxt.getHeight()) / 2, mren, NULL, NULL, NULL, SDL_FLIP_NONE);

			pauseButton_save.render(mren);
			pauseButton_saveTxt.render(pauseButton_save.getX() + (pauseButton_save.getWidth() - pauseButton_saveTxt.getWidth()) / 2, pauseButton_save.getY() + (pauseButton_save.getHeight() - pauseButton_saveTxt.getHeight()) / 2, mren, NULL, NULL, NULL, SDL_FLIP_NONE);
		
			option_button.render(mren);
			option_button_txt.render(option_button.getX() + ((option_button.getWidth() - option_button_txt.getWidth()) / 2), option_button.getY() + ((option_button.getHeight() - option_button_txt.getHeight()) / 2), mren, NULL, NULL, NULL, SDL_FLIP_NONE);
		}

		pauseTxtMenu.render(mren);
		
		if (options)
		{
			options_slider_music.render(mren);
			option_button_back.render(mren);
			option_button_back_txt.render(option_button_back.getX() + ((option_button_back.getWidth() - option_button_back_txt.getWidth()) / 2), option_button_back.getY() + ((option_button_back.getHeight() - option_button_back_txt.getHeight()) / 2), mren, NULL, NULL, NULL, SDL_FLIP_NONE);
			option_button_apply.render(mren);
			option_button_apply_txt.render(option_button_apply.getX() + ((option_button_apply.getWidth() - option_button_apply_txt.getWidth()) / 2), option_button_apply.getY() + ((option_button_apply.getHeight() - option_button_apply_txt.getHeight()) / 2), mren, NULL, NULL, NULL, SDL_FLIP_NONE);
			options_slider_sound.render(mren);
			options_slider_music_txt.render(options_slider_music.getSliderPos().x + ((options_slider_music.getSliderPos().w - options_slider_music_txt.getWidth()) / 2), options_slider_music.getSliderPos().y + ((options_slider_music.getSliderPos().h - options_slider_music_txt.getHeight()) / 2) - options_slider_music_txt.getHeight() - 5, mren, NULL, NULL, NULL, SDL_FLIP_NONE);
			options_slider_sound_txt.render(options_slider_sound.getSliderPos().x + ((options_slider_sound.getSliderPos().w - options_slider_sound_txt.getWidth()) / 2), options_slider_sound.getSliderPos().y + ((options_slider_sound.getSliderPos().h - options_slider_sound_txt.getHeight()) / 2) - options_slider_sound_txt.getHeight() - 5, mren, NULL, NULL, NULL, SDL_FLIP_NONE);
			option_button_vsync.render(mren);
			option_button_vsync_txt.render(option_button_vsync.button.getX() + ((option_button_vsync.button.getWidth() - option_button_vsync_txt.getWidth()) / 2) - (option_button_vsync_txt.getWidth() / 2) - 25, option_button_vsync.button.getY() + 5, mren, NULL, NULL, NULL, SDL_FLIP_NONE);
		}
	}
}

void TGamePlay::handleEvents(SDL_Event* e, SDL_Renderer* mren, SDL_Window* winref, lua_State* _____L)
{
	if (worldGen && !genSuns)
	{
		if(!devC.getConsoleOpen())
			ply.handleEvents(e, cam.x, cam.y, paused, weapons[weaponSelection].bInf, !gameover);
		devC.handleEvents(e, mren, _____L);
		if (devC.getupdatePlayerCommand())
		{
			ply.setPlyPos(devC.getPlyPos().x, devC.getPlyPos().x, P2M, plyCBox);
		}
		genDat_showChuncks = devC.getChunckOn();
		genDat_showSolorChuncks = devC.getSolorOn();
		if (paused)
		{
			if (options)
			{
				options_slider_music.handleEvents(e);
				option_button_back.handleEvent(e);
				option_button_apply.handleEvent(e);
				options_slider_sound.handleEvents(e);
				option_button_vsync.handleEvents(e);
				if (option_button_back.getDown())
				{
					options = false;
					options_slider_music.setSValue(game_musicVol);
					options_slider_sound.setSValue(game_soundVol);
				}
				if (option_button_apply.getDown())
				{
					game_soundVol = options_slider_sound.getValue();
					game_musicVol = options_slider_music.getValue();
					game_vSync = option_button_vsync.getToggled();

					fstream fileooooo;

					fileooooo.open("cfg/options.cfg", ios::trunc | ios::out | ios::binary);

					stringstream ss;
					ss << game_soundVol << endl << game_musicVol << endl << game_vSync << endl;

					if (fileooooo.is_open())
					{
						fileooooo << ss.str();
					}
					else
					{
						cout << "unable to open options file!" << endl;
					}
					fileooooo.close();
				}
			}
			else
			{
				pauseButton_quitTMM.handleEvent(e);
				pauseButton_save.handleEvent(e);
				option_button.handleEvent(e);
				if (pauseButton_quitTMM.getDown())
				{
					backToMM = true;
				}
				else if (option_button.getDown())
				{
					options = true;
				}
				else if (pauseButton_save.getDown())
				{
					//fstream plyFile___;
					fstream datFile;
					//plyFile___.open("sav/" + game_name + "/pos/playerDat.data", ios::out | ios::trunc);
					datFile.open("sav/" + game_name + "/data.sav", ios::out | ios::trunc);

					//plyFile___ << ply.getPlyDiment().x << " " << ply.getPlyDiment().y << " " << ply.getVel().x << " " << ply.getVel().y << " " << rotSpeed << " " << ply.getHealth() << endl;
					datFile << game_name << endl << game_dif << endl << game_seed << endl;

					//plyFile___.close();
					datFile.close();

					SDL_RWops* ____file = SDL_RWFromFile(("sav/" + game_name + "/pos/starDat.data").c_str(), "w+b");
					if (____file != NULL)
					{
						for (int y = 0; y < chuncksY; y++)
						{
							for (int x = 0; x < chuncksX; x++)
							{
								SDL_RWwrite(____file, &worldChuncks[x][y].generated, sizeof(worldChuncks[x][y].generated), 1);
								if (worldChuncks[x][y].generated)
								{
									SDL_RWwrite(____file, &worldChuncks[x][y].chunckRet, sizeof(worldChuncks[x][y].chunckRet), 1);
									SDL_RWwrite(____file, &worldChuncks[x][y].stars, sizeof(worldChuncks[x][y].stars), 1);
								}
							}
						}
						SDL_RWclose(____file);
					}
					else
					{
						cout << "Error: Unable to create file! SDL Error: " << SDL_GetError() << endl;
					}

					SDL_RWops* __file = SDL_RWFromFile(("sav/" + game_name + "/pos/playerDat.data").c_str(), "w+b");
					if (__file != NULL)
					{
						SDL_Rect posRef = ply.getPlyDiment();
						int hp = ply.getHealth();
						SDL_Point vel = ply.getVel();
						SDL_RWwrite(__file, &plyCBox->GetPosition().x, sizeof(plyCBox->GetPosition().x), 1);
						SDL_RWwrite(__file, &plyCBox->GetPosition().y, sizeof(plyCBox->GetPosition().y), 1);
						SDL_RWwrite(__file, &plyCBox->GetLinearVelocity().x, sizeof(plyCBox->GetLinearVelocity().x), 1);
						SDL_RWwrite(__file, &plyCBox->GetLinearVelocity().y, sizeof(plyCBox->GetLinearVelocity().y), 1);
						SDL_RWwrite(__file, &rotSpeed, sizeof(rotSpeed), 1);
						SDL_RWwrite(__file, &hp, sizeof(hp), 1);
						SDL_RWwrite(__file, &weaponSelection, sizeof(weaponSelection), 1);
						SDL_RWwrite(__file, &weapons, sizeof(weapons), 1);
						SDL_RWwrite(__file, &ammoTypePools, sizeof(ammoTypePools), 1);
						SDL_RWclose(__file);
					}
					else
					{
						cout << "Error: Unable to create file! SDL Error: " << SDL_GetError() << endl;
					}

					SDL_RWops* file = SDL_RWFromFile(("sav/" + game_name + "/pos/sunDat.data").c_str(), "w+b");
					if (file != NULL)
					{
						SDL_RWwrite(file, &sunData, sizeof(sunData), 1);
						SDL_RWwrite(file, &ammountOfSuns, sizeof(ammountOfSuns), 1);
						SDL_RWclose(file);
					}
					else
					{
						cout << "Error: Unable to create file! SDL Error: " << SDL_GetError() << endl;
					}
				}
			}
		}

		//press any key thing stuff yeah
		if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
		{
			if (gameover)
			{
				gameover = false;
				gOverC = 255;
				ply.setPlyPos(spawnPoint.x, spawnPoint.y, P2M, plyCBox);
				ply.setPlyVel(0, 0);
				ply.setHealth(100);
			}

			if (!devC.getConsoleOpen())
			{
				switch (e->key.keysym.sym)
				{
				case SDLK_1:
					weaponSelection = 0;
					ply.setAmmoPool(ammoTypePools[weapons[weaponSelection].bInf.type]);
					ply.setAmmoInPool(weapons[weaponSelection].ammoInClip);
					stopReload = true;
					break;
				case SDLK_2:
					weaponSelection = 1;
					ply.setAmmoPool(ammoTypePools[weapons[weaponSelection].bInf.type]);
					ply.setAmmoInPool(weapons[weaponSelection].ammoInClip);
					stopReload = true;
					break;
				case SDLK_3:
					weaponSelection = 2;
					ply.setAmmoPool(ammoTypePools[weapons[weaponSelection].bInf.type]);
					ply.setAmmoInPool(weapons[weaponSelection].ammoInClip);
					stopReload = true;
					break;
				}
			}
		}

		//pause menu detection
		if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
		{
			switch (e->key.keysym.sym)
			{
			case SDLK_ESCAPE:
				paused = !paused;

				if (paused)
				{
					ply.pauseTimers();
				}
				else
				{
					ply.unPauseTimers();
				}

				break;
			}
		}
	}
}

void TGamePlay::update(SDL_Renderer* mren, SDL_Window* winref)
{
	maxLoad = 382;
	//star generation / loading function
	if (currentLoad <= 382 && !worldGen)
	{
		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 5; x++)
			{
				worldChuncks[x][y].chunckRet.x = x * 256;
				worldChuncks[x][y].chunckRet.y = y * 256;
				genChunck(&worldChuncks[x][y]);
			}
		}

		file.close();

		SDL_RWops* ____file = SDL_RWFromFile(("sav/" + game_name + "/pos/starDat.data").c_str(), "w+b");
		if (____file != NULL)
		{
			for (int y = 0; y < chuncksY; y++)
			{
				for (int x = 0; x < chuncksX; x++)
				{
					SDL_RWwrite(____file, &worldChuncks[x][y].generated, sizeof(worldChuncks[x][y].generated), 1);
					if (worldChuncks[x][y].generated)
					{
						SDL_RWwrite(____file, &worldChuncks[x][y].chunckRet, sizeof(worldChuncks[x][y].chunckRet), 1);
						SDL_RWwrite(____file, &worldChuncks[x][y].stars, sizeof(worldChuncks[x][y].stars), 1);
					}
				}
			}
			SDL_RWclose(____file);
		}
		else
		{
			cout << "Error: Unable to create file! SDL Error: " << SDL_GetError() << endl;
		}

		currentLoad = 383;
		worldGen = true;
	}
	else
	{
		//end of loading
		worldGen = true;
		fastLoop = false;
		file.close();
	}

	//generate suns
	if (worldGen && genSuns)
	{
		//gen precashe pos
		for (int i = 0; i < maxSuns; i++)
		{
			sunData[i].x = rand() % (chuncksX * 256) + 0;
			sunData[i].y = rand() % (chuncksY * 256) + 0;

			sunData[i].solorBox.x = sunData[i].x - 600 + (40 * 2);
			sunData[i].solorBox.y = sunData[i].y - 600 + (40 * 2);
			sunData[i].solorBox.w = (600 * 2);
			sunData[i].solorBox.h = (600 * 2);
			for (int o = 0; o < maxSuns; o++)
			{
				if (checkColsion(sunData[i].solorBox, sunData[o].solorBox) && i != o)
				{
					sunData[i].x = rand() % (chuncksX * 256) + 0;
					sunData[i].y = rand() % (chuncksY * 256) + 0;

					sunData[i].solorBox.x = sunData[i].x - 600 + (40 * 2);
					sunData[i].solorBox.y = sunData[i].y - 600 + (40 * 2);
					sunData[i].solorBox.w = (600 * 2);
					sunData[i].solorBox.h = (600 * 2);
				}
			}
		}

		//gen solor systems
		for (int _i = 0; _i < ammountOfSuns; _i++)
		{
			genSolor(&sunData[_i], &sunObjs[_i], mren, winref, true);
		}

		SDL_RWops* file = SDL_RWFromFile(("sav/" + game_name + "/pos/sunDat.data").c_str(), "w+b");
		if (file != NULL)
		{
			SDL_RWwrite(file, &sunData, sizeof(sunData), 1);
			SDL_RWwrite(file, &ammountOfSuns, sizeof(ammountOfSuns), 1);
			SDL_RWclose(file);
		}
		else
		{
			cout << "Error: Unable to create file! SDL Error: " << SDL_GetError() << endl;
		}

		genSuns = false;
	}

	//non-loading functions
	if (worldGen && !genSuns)
	{
		/*HUD_minimap.createBlank(mren, winref, 200, 200, SDL_TEXTUREACCESS_TARGET);
		HUD_minimap.setAsRenderTarget(mren);
		SDL_SetRenderDrawColor(mren, 0, 0, 0, 0);
		SDL_RenderClear(mren);
		SDL_SetRenderDrawColor(mren, 255, 255, 255, 255);

		//minimap stuff here

		SDL_Rect mmRef = { 0, 0, 200, 200 };
		SDL_SetRenderDrawColor(mren, 255, 0, 0, 255);
		SDL_RenderDrawRect(mren, &mmRef);

		SDL_SetRenderDrawColor(mren, 0, 255, 0, 255);
		SDL_RenderDrawPoint(mren, npcs[0].npc.getNpcRect().x * 0.5, npcs[0].npc.getNpcRect().y*0.5);

		SDL_SetRenderDrawColor(mren, 255, 255, 255, 255);

		SDL_SetRenderTarget(mren, NULL);
		HUD_minimap.setBlendMode(SDL_BLENDMODE_BLEND);*/

		weapons[weaponSelection].ammoInClip = ply.getAmmoInClip();
		weapons[weaponSelection].ammoPool = ply.getAmmoPool();
		ammoTypePools[weapons[weaponSelection].bInf.type] = ply.getAmmoPool();
		ply.setAmmoPool(ammoTypePools[weapons[weaponSelection].bInf.type]);

		if (devC.getUpdateAmmo())
		{
			ammoTypePools[weapons[weaponSelection].bInf.type] = devC.getAmmoPool();
			ply.setAmmoPool(ammoTypePools[weapons[weaponSelection].bInf.type]);
			devC.setUpdateAmmo(false);
		}

		ply.move(plyCBox, cam.x, cam.y, rotSpeed, paused, cam.w, cam.h, weapons[weaponSelection].bInf, stopReload, P2M, M2P, !gameover);
		stopReload = false;

		//testEn.update({ 200, 200, 0, 0 }, ply.getPlyDiment(), !paused, false, SCREEN_WIDTH, SCREEN_HEIGHT, bType_MachineGunB, testNpc, cam, P2M, M2P, NPC_ENEMY);
		npcs[0].npc.update({ 200, 200, 0, 0 }, ply.getPlyDiment(), !paused, false, SCREEN_WIDTH, SCREEN_HEIGHT, bType_MachineGunB, npcs[0].body, cam, P2M, M2P, NPC_ENEMY);

		HUD_Ply_Hp.free();
		HUD_Ply_Hp.loadFromTtf(to_string(ply.getHealth()), HUD_Font, { 255, 0, 0, 255 }, mren);
		HUD_ammo.free();
		HUD_ammo.loadFromTtf(to_string(ply.getAmmoInClip()) + "/" + to_string(ply.getAmmoPool()), HUD_Font_big, { 255, 255, 0, 255 }, mren);

		//center the camera on the player
		cam.x = (ply.getPlyDiment().x + ply.getPlyDiment().w / 2) - SCREEN_WIDTH / 2;
		cam.y = (ply.getPlyDiment().y + ply.getPlyDiment().w / 2) - SCREEN_HEIGHT / 2;

		//update planet pos
		if (!paused)
		{
			for (int i = 0; i < ammountOfSuns; i++)
			{
				for (int o = 0; o < sunData[i].ammountOfPlanets; o++)
				{
					sunData[i].planets[o].truePos.x = sunData[i].planets[o].centerPos.x + cos(sunData[i].planets[o].angle * (M_PI * 2)) * sunData[i].planets[o].orbitRaidus;
					sunData[i].planets[o].truePos.y = sunData[i].planets[o].centerPos.y + sin(sunData[i].planets[o].angle * (M_PI * 2)) * sunData[i].planets[o].orbitRaidus;
					sunData[i].planets[o].angle += sunData[i].planets[o].orbitSpeed;
					if (sunData[i].planets[o].angle > 1)
					{
						sunData[i].planets[o].angle = 0;
					}
				}
			}
		}

		for (int ____i = 0; ____i < ammountOfSuns; ____i++)
		{
			SDL_Rect deathR = { sunData[____i].x, sunData[____i].y, sunData[____i].scale, sunData[____i].scale };
			ply.checkColosion({ sunData[____i].x, sunData[____i].y, sunData[____i].scale, sunData[____i].scale }, &deathR);
		}

		if (ply.getHealth() <= 0)
		{
			gameover = true;
		}

		for (int y = AvMath::rnd::roundUp(cam.y, 256) * 0.001953125; y < AvMath::rnd::roundUp(cam.y + cam.h, 256) * 0.00390625; y++)
		{
			for (int x = AvMath::rnd::roundUp(cam.x, 256) * 0.001953125; x < AvMath::rnd::roundUp(cam.x + cam.w, 256) * 0.00390625; x++)
			{
				//to fix access crashes
				if (x <= 0)
					x = 0;
				if (y <= 0)
					y = 0;

				SDL_Rect camChunckGen = { cam.x - 200, cam.y - 200, cam.w + 400, cam.h + 400 };
				if (checkColsion(camChunckGen, worldChuncks[x][y].chunckRet) && !worldChuncks[x][y].generated)
				{
					genChunck(&worldChuncks[x][y]);
				}
			}
		}

		/*for (int i = 0; i < 100; i++)
		{
		SDL_Rect camChunckGen = { cam.x - 200, cam.y - 200, cam.w + 400, cam.h + 400 };
		if (checkColsion(camChunckGen, sunData[i].solorBox) && !sunData[i].generated)
		{
		genSolor(&sunData[i], &sunObjs[i], mren, winref, false);
		ammountOfSuns++;
		}
		}*/

		devC.updateConsole(mren, winref);

		/*allProjectiles.clear();
		for (int i = 0; i < ply.getPlayersProjectiles().size(); i++)
		{
			allProjectiles.push_back(ply.getPlayersProjectiles().at(i));
		}
		for (int i = 0; i < testEn.getNpcP().size(); i++)
		{
			allProjectiles.push_back(testEn.getNpcP().at(i));
		}*/

		//ply.checkBulletC(allProjectiles, 2);

		/*ply.checkBulletC(testEn.getNpcP());
		testEn.checkBulletC(ply.getPlayersProjectiles());*/

		ply.checkBulletC(npcs[0].npc.getNpcP());
		npcs[0].npc.checkBulletC(ply.getPlayersProjectiles());

		//keep the camera in bounds
		if (cam.x < 0)
		{
			cam.x = 0;
		}
		if (cam.y < 0)
		{
			cam.y = 0;
		}
		if (cam.x > LEVEL_WIDTH - cam.w)
		{
			cam.x = LEVEL_WIDTH - cam.w;
		}
		if (cam.y > LEVEL_HEIGHT - cam.h)
		{
			cam.y = LEVEL_HEIGHT - cam.h;
		}
	}
}

bool TGamePlay::getFastLoop()
{
	return fastLoop;
}

void TGamePlay::freeAll()
{
	if (world != NULL)
	{
		b2Free(world);
		world = NULL;
	}

	currentNpcs = 0;

	pauseTxt.free();
	pauseButton_quitTMM.free();
	pauseButton_quitTMMTxt.free();
	pauseMenu.free();
	virsion_Txt.free();
	ply.playerTxt.free();
	ply.free(spawnPoint);
	pauseTxtMenu.free();
	pauseButton_save.free();
	pauseButton_saveTxt.free();

	HUD_gameover_txt.free();
	HUD_Ply_Hp.free();

	option_button.free();
	option_button_txt.free();
	option_button_back.free();
	option_button_back_txt.free();
	option_button_apply.free();
	option_button_apply_txt.free();
	options_slider_sound.free();
	options_slider_sound_txt.free();
	options_slider_music_txt.free();
	option_button_vsync.free();
	option_button_vsync_txt.free();

	for (int i = 0; i < ammountOfSuns; i++)
	{
		SDL_DestroyTexture(sunObjs[i].ctxt);
		for (int o = 0; o < sunData[i].ammountOfPlanets; o++)
		{
			if (sunObjs[i].ctxt2[o] != NULL)
			{
				SDL_DestroyTexture(sunObjs[i].ctxt2[o]);
				sunObjs[i].ctxt2[o] = NULL;
			}
		}
	}

	for (int y = 0; y < chuncksY; y++)
	{
		for (int x = 0; x < chuncksX; x++)
		{
			worldChuncks[x][y].generated = false;
		}
	}

	//starVect.clear();
}

bool TGamePlay::getBTT()
{
	return backToMM;
}

void TGamePlay::setBBT(bool BBT)
{
	backToMM = BBT;
}

void TGamePlay::setPause(bool __paused)
{
	paused = __paused;
}