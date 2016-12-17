#include "gPlayer.h"

gPlayer::gPlayer()
{
	speed = 0;
	reload = false;
}

gPlayer::~gPlayer()
{
}

void gPlayer::startShootTimer()
{
	time = SDL_GetTicks();
}

bool checkTheColsion(SDL_Rect a, SDL_Rect b)
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

void gPlayer::handleEvents(SDL_Event*e, int camX, int camY, bool paused, bulletInfo binf, bool acceptUserInput)
{
	if (e->type == SDL_MOUSEBUTTONDOWN)
	{
		if (e->button.button == SDL_BUTTON_LEFT && !paused && acceptUserInput)
		{
			shoot = true;
		}
	}
	else if (e->type == SDL_MOUSEBUTTONUP)
	{
		if (e->button.button == SDL_BUTTON_LEFT && !paused && acceptUserInput)
		{
			shoot = false;
		}
	}

	if (paused)
	{
		shoot = false;
	}

	switch (e->type)
	{
	case SDL_KEYDOWN:
		switch (e->key.keysym.sym)
		{
		case SDLK_r:
			if (bulletsInclip < binf.clipSize && !reload)
			{
				reloadTimer = SDL_GetTicks();
				reload = true;
			}
			break;
		case SDLK_w:
			//slowSpeed = false;
			if (!paused && acceptUserInput)
			{
				speed = 10;
				//slowVel = false;
			}
			break;
		case SDLK_z:
			//TODO: slow vel
			/*if (!paused && acceptUserInput)
			{
				slowVel = true;
			}*/
			break;
		}
		break;
	case SDL_KEYUP:
		switch (e->key.keysym.sym)
		{
		case SDLK_w:
			speed = 0;
			break;
		}
		break;
	}
}

void gPlayer::render(SDL_Renderer* mren, int camX, int camY, b2Body* bodRef, double _m2p)
{
	SDL_Point centerOfP = { playerTxt.getWidth() / 2, playerTxt.getHeight() / 2 };
	playerTxt.render((bodRef->GetWorldCenter().x * _m2p) - (playerTxt.getWidth() / 2) - camX, (bodRef->GetWorldCenter().y * _m2p) - (playerTxt.getHeight() / 2) - camY, mren, NULL, bodRef->GetAngle() * (180 / M_PI), &centerOfP, SDL_FLIP_VERTICAL);

	for (int i = 0; i < playersProjectiles.size(); i++)
	{
		SDL_SetRenderDrawColor(mren, 0, 255, 0, 255);
		SDL_Rect _ref = { playersProjectiles.at(i).x - camX, playersProjectiles.at(i).y - camY, 20, 20 };
		SDL_RenderFillRect(mren, &_ref);
		SDL_SetRenderDrawColor(mren, 255, 255, 255, 255);
	}
}

void gPlayer::move(b2Body* plyC, int camX, int camY, double rotSpeed, bool pause, int camW, int camH, bulletInfo binf, bool weaponChanged, double _p2m, double _m2p, bool acceptUserInput)
{
	if (!pause)
	{
		if (weaponChanged)
		{
			reload = false;
		}

		//reload logic
		if (reload)
		{
			if (ammoPool <= 0 && bulletsInclip <= 0)
			{
				reload = false;
			}
			else if (SDL_GetTicks() - reloadTimer >= binf.reloadTime)
			{
				if (ammoPool <= binf.clipSize)
				{
					int bulletsNeededToR = binf.clipSize - bulletsInclip;

					if (ammoPool >= bulletsNeededToR)
					{
						ammoPool -= bulletsNeededToR;
						bulletsInclip += bulletsNeededToR;
					}
					else
					{
						bulletsInclip += ammoPool;
						ammoPool = 0;
					}
				}
				else
				{
					int bulletsNeededToR = binf.clipSize - bulletsInclip;

					ammoPool -= bulletsNeededToR;
					bulletsInclip += bulletsNeededToR;
				}
				reload = false;
			}
		}

		//shoot logic
		if (SDL_GetTicks() - time >= /*shoot delay in ticks*/binf.bulletDelay && !reload && shoot)
		{
			if (bulletsInclip > 0)
			{
				time = SDL_GetTicks();

				for (int i = 0; i < binf.bulletsPerShot; i++)
				{
					_projectile tempP = { 0, 0, 0, 0 };

					//select a random direction based on npc rotaion
					double f = (double)rand() / RAND_MAX;
					double shootOffest = (AvMath::toDegrees(plyC->GetAngle()) - binf.bulletSpread) + f * ((AvMath::toDegrees(plyC->GetAngle()) + binf.bulletSpread) - (AvMath::toDegrees(plyC->GetAngle()) - binf.bulletSpread));
					
					tempP.x = plyC->GetWorldPoint(b2Vec2(0, 1)).x * _m2p;
					tempP.y = plyC->GetWorldPoint(b2Vec2(0, 1)).y * _m2p;
					tempP.xVel = 0;
					tempP.yVel = 0;
					tempP.storedVX = ply_velX;
					tempP.storedVY = ply_VelY;
					tempP.storedRot = -((shootOffest * M_PI) / 180);
					tempP.damage = binf.damagePerHit;
					tempP.width = binf.width;
					tempP.height = binf.height;

					playersProjectiles.push_back(tempP);
				}

				bulletsInclip--;
			}
			if (bulletsInclip <= 0)
			{
				reload = true;
				reloadTimer = SDL_GetTicks();
			}
		}

		for (int i = 0; i < playersProjectiles.size(); i++)
		{
			playersProjectiles.at(i).xVel = playersProjectiles.at(i).storedVX + (double)20/*bullet speed in pps*/ * (double)sin((double)playersProjectiles.at(i).storedRot);
			playersProjectiles.at(i).yVel = playersProjectiles.at(i).storedVY + (double)20/*bullet speed in pps*/ * (double)cos((double)playersProjectiles.at(i).storedRot);

			playersProjectiles.at(i).x += playersProjectiles.at(i).xVel;
			playersProjectiles.at(i).y += playersProjectiles.at(i).yVel;

			if (!checkTheColsion({ camX, camY, camW, camH }, { playersProjectiles.at(i).x, playersProjectiles.at(i).y, 20, 20 }))
			{
				playersProjectiles.erase(playersProjectiles.begin() + i);
			}
		}

		int __x, __y;

		//get mousepos and direction to face mouse
		SDL_GetMouseState(&__x, &__y);

		__x += camX;
		__y += camY;

		moveForward(speed, plyC, { camX, camY }, _p2m);
		ply_Pos.x = (plyC->GetWorldCenter().x * _m2p);
		ply_Pos.y = (plyC->GetWorldCenter().y * _m2p);

		ply_Pos.w = playerTxt.getWidth();
		ply_Pos.h = playerTxt.getHeight();

		if (!slowKill)
		{
			if (health > 0)
			{
				//health--;
			}
		}
	}
}

SDL_Rect gPlayer::getPlyDiment()
{
	return{ ply_Pos.x, ply_Pos.y, playerTxt.getWidth(), playerTxt.getHeight() };
}

void gPlayer::setPlyPos(int x, int y, double P2m, b2Body* plyC)
{
	ply_Pos.x = (x);
	ply_Pos.y = (y);

	plyC->SetTransform(b2Vec2(x * P2m, y*P2m), plyC->GetAngle());
}

void gPlayer::setPlyVel(double xV, double yV)
{
	ply_velX = xV;
	ply_VelY = yV;
}

SDL_Point gPlayer::getVel()
{
	return{ ply_velX, ply_VelY };
}

void gPlayer::checkColosion(SDL_Rect __ref, SDL_Rect* deathRect)
{

}

int gPlayer::getHealth()
{
	return health;
}

void gPlayer::setHealth(int hp)
{
	health = hp;
}

void gPlayer::free(SDL_Point spawn)
{
	speed = 0;
}

vector<_projectile> gPlayer::getPlayersProjectiles()
{
	return playersProjectiles;
}

void gPlayer::checkBulletC(vector<_projectile> projectiles)
{
	if (health >= 0)
	{
		for (int i = 0; i < projectiles.size(); i++)
		{
			if (checkTheColsion({ projectiles.at(i).x, projectiles.at(i).y, projectiles.at(i).width, projectiles.at(i).height }, getPlyDiment()))
			{
				health -= projectiles.at(i).damage;
			}
		}
	}
}

int gPlayer::getAmmoInClip()
{
	return bulletsInclip;
}

int gPlayer::getAmmoPool()
{
	return ammoPool;
}

bool gPlayer::getReload()
{
	return reload;
}

void gPlayer::pauseTimers()
{
	pauseTicks = SDL_GetTicks() - time;
	reloadPauseTicks = SDL_GetTicks() - reloadTimer;
}

void gPlayer::unPauseTimers()
{
	time = SDL_GetTicks() - pauseTicks;
	reloadTimer = SDL_GetTicks() - reloadPauseTicks;
}

int gPlayer::getReloadStartTime()
{
	return reloadTimer;
}

void gPlayer::setAmmoPool(int _pool)
{
	ammoPool = _pool;
}

void gPlayer::setAmmoInPool(int __clip)
{
	bulletsInclip = __clip;
}

int gPlayer::getPauseTicks()
{
	return reloadPauseTicks;
}