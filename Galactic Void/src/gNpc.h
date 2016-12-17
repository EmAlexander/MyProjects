#ifndef __gNpc_H_INCLUDED
#define __gNpc_H_INCLUDED

#include <SDL.h>
#include <cstdlib>
#include "LTexture.h"
#include "vector.h"
#include "avMath.h"
#include "bulletInfo.h"
#include "gPlayer.h"

enum npcAgroTypes{ 
	//does not attack player
	NPC_FRENDLY, 
	//only attacks player when attacked
	NPC_NEUTRAL, 
	//always attacks player
	NPC_ENEMY 
};

enum npcRoleTypes{
	NPC_ROLE_CIVILIAN,
	NPC_ROLE_PIRATE,
	NPC_ROLE_TRADER,
	NPC_ROLE_BOUNTYHUNTER
};

enum npcFactionRole{
	NPC_FACTION_EMPIRE,
	NPC_FACTION_REBEL,
	NPC_FACTION_INDEPENDENT
};

class gNpc
{
public:
	gNpc();
	~gNpc();

	LTexture texture;

	void setHP(double hp_);

	void update(SDL_Rect wayPointPos, SDL_Rect plyPos, bool _update, bool attackWayPoint, int screenW, int screenH, bulletInfo bInf, b2Body* npcC, SDL_Rect cam, float p2m,float m2p, npcAgroTypes npcType = NPC_NEUTRAL, npcRoleTypes role = NPC_ROLE_CIVILIAN, npcFactionRole faction = NPC_FACTION_INDEPENDENT);

	vector<_projectile> getNpcP();

	void setBulletType(bulletInfo bfi);

	double getRep();
	bool getAgroPlayer();
	bool getAtWapoint();

	SDL_Rect getNpcRect();

	void free();

	void render(SDL_Renderer* mren, SDL_Rect cam, b2Body* npcB, float m2p);

	bulletInfo getBulletInf();

	void checkBulletC(vector<_projectile> p);

	bool getDead();
	void setDead(bool dead);
private:
	struct hackedPoint
	{
		float x;
		float y;
	};

	bool checkColsion(SDL_Rect a, SDL_Rect b);

	void moveFoward(b2Body* npcCRef, double _speed, SDL_Point cam, float p2m, float m2p, SDL_Point target);

	bool agroed;
	bool atWaypoint;

	int hp, shields;
	double angle, AAngle;
	double roationR;
	double renderR;
	double speed;
	double x, y, xVel, yVel;
	//double __XP, __YP;
	bool isDead;

	int bulletsShot;
	bool reload;
	double reloadTimer;

	bulletInfo _bulletInfo;

	double rep;

	vector<_projectile> npcProjectile;
};

#endif //__gNpc_H_INCLUDED