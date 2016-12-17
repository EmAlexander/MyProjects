#ifndef __GPLAYER_H_INCLUDED
#define __GPLAYER_H_INCLUDED

#include "LTexture.h"
#include "vector.h"
#include "avMath.h"
#include "LTimer.h"
#include "bulletInfo.h"

#include <Box2D\Box2D.h>
#include <math.h>
#include <vector>

struct _projectile
{
	double x;
	double y;
	double xVel;
	double yVel;
	double storedRot;
	double storedVX;
	double storedVY;
	double damage;

	int width;
	int height;
};

class gPlayer
{
public:
	gPlayer();
	~gPlayer();

	//player init
	void player();
	void handleEvents(SDL_Event*e, int camX, int camY, bool paused, bulletInfo binf, bool acceptUserInput = true);
	void render(SDL_Renderer* mren, int camX, int camY, b2Body* bodRef, double _m2p);
	void move(b2Body* plyC, int camX, int camY, double rotSpeed, bool pause, int camW, int camH, bulletInfo binf, bool weaponChanged, double _p2m, double _m2p, bool acceptUserInput = true);
	void setScrnWH(int screenW, int screenH);
	void setPlyPos(int x, int y, double P2m, b2Body* plyC);
	void setPlyVel(double xV, double yV);
	int getHealth();
	void setHealth(int hp);

	void checkBulletC(vector<_projectile> projectiles);

	void checkColosion(SDL_Rect __ref, SDL_Rect* deathRect = NULL);
	void free(SDL_Point spawn);

	vector<_projectile> getPlayersProjectiles();

	void pauseTimers();
	void unPauseTimers();

	void startShootTimer();

	SDL_Point getVel();
	SDL_Rect getPlyDiment();
	LTexture playerTxt;

	void setAmmoPool(int _pool);
	void setAmmoInPool(int __clip);

	int getAmmoInClip();
	int getAmmoPool();
	bool getReload();

	int getReloadStartTime();
	int getPauseTicks();
private:
	//bool slowVel;
	//bool slowSpeed;
	double speed;
	//double angle;
	//double AAngle;
	SDL_Rect ply_Pos;
	double ply_velX, ply_VelY;
	//double __PX, __PY;

	//double ply_roationR;
	//double ply_renderR;

	int SCREEN_HEIGHT, SCREEN_WIDTH;

	//bool colsion = false;
	bool slowKill = true;

	double health;
	double shield;

	vector<_projectile> playersProjectiles;

	double time;
	bool reload;
	bool shoot = false;
	double reloadTimer;
	int bulletsInclip = 0;
	int ammoPool = 0;

	int pauseTicks;

	int reloadPauseTicks;

	SDL_Point colidedPnt;

	struct hackedPoint
	{
		float x;
		float y;
	};

	void moveForward(double speed, b2Body* bbod, SDL_Point cam, double _P2M)
	{
		//cout << ply_velX << "," << ply_VelY << endl;

		//get mouse pos
		int x, y;
		SDL_GetMouseState(&x, &y);

		//set clicked point
		b2Vec2 clickedPoint = b2Vec2((x + cam.x) * _P2M, (y + cam.y) * _P2M);

		//get angle
		float bodyAngle = bbod->GetAngle();
		b2Vec2 toTarget = clickedPoint - bbod->GetPosition();
		float desiredAngle = atan2f(-toTarget.x, toTarget.y);

		//get total rotaion
		float totalRotation = desiredAngle - bodyAngle;

		//uneffectient rotaion fix
		while (totalRotation < (-180 * M_PI) / 180) totalRotation += (360 * M_PI) / 180;
		while (totalRotation >(180 * M_PI) / 180) totalRotation -= (360 * M_PI) / 180;

		//rotate slowly
		float change = (10/*rotaion speed*/ * M_PI) / 180;
		float newAngle = bodyAngle + min(change, max(-change, totalRotation));

		//apply rotaion
		bbod->SetTransform(bbod->GetPosition(), newAngle);
		bbod->SetAngularVelocity(0);
		bbod->SetAwake(true);

		hackedPoint direction;

		//first get the direction the entity is pointed
		direction.x = (float)sin(bbod->GetAngle()) * speed;
		direction.y = (float)cos(bbod->GetAngle()) * speed;

		bbod->ApplyForce(b2Vec2(-direction.x, direction.y), bbod->GetWorldCenter(), true);
	}
};

#endif //__GPLAYER_H_INCLUDED