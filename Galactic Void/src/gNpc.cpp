#include "gNpc.h"

bool gNpc::checkColsion(SDL_Rect a, SDL_Rect b)
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

void gNpc::moveFoward(b2Body* npcCRef, double _speed, SDL_Point cam, float p2m,float m2p, SDL_Point target)
{
	cout << hp << endl;
	if (hp >= 1)
	{
		//set clicked point
		b2Vec2 clickedPoint = b2Vec2((target.x) * p2m, (target.y) * p2m);

		//get angle
		float bodyAngle = npcCRef->GetAngle();
		b2Vec2 toTarget = clickedPoint - npcCRef->GetPosition();
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
		npcCRef->SetTransform(npcCRef->GetPosition(), newAngle);
		npcCRef->SetAngularVelocity(0);
		npcCRef->SetAwake(true);

		hackedPoint direction;

		//first get the direction the entity is pointed
		direction.x = (float)sin(npcCRef->GetAngle()) * _speed;
		direction.y = (float)cos(npcCRef->GetAngle()) * _speed;

		npcCRef->ApplyForce(b2Vec2(-direction.x, direction.y), npcCRef->GetWorldCenter(), true);
	}
}

gNpc::gNpc()
{
	speed = 0;
	x = 0;
	y = 0;
	isDead = false;
	reload = false;
}

gNpc::~gNpc()
{
}

double time = SDL_GetTicks();

void gNpc::update(SDL_Rect wayPointPos, SDL_Rect plyPos, bool _update, bool attackWayPoint, int screenW, int screenH, bulletInfo bInf, b2Body* npcC, SDL_Rect cam, float p2m, float m2p, npcAgroTypes npcType, npcRoleTypes role, npcFactionRole faction)
{
	if (_update)
	{
		if (!(hp >= 0))
		{
			isDead = true;
		}

		double _distance = AVE_findM({ x, y }, { plyPos.x, plyPos.y });
		double wayPointDistance = AVE_findM({ x, y }, { wayPointPos.x, wayPointPos.y });
		
		SDL_Point targetSet = { 0, 0 };

		if (hp >= 0)
		{
			if (!agroed)
			{
				targetSet = {wayPointPos.x, wayPointPos.y};
			}
			else
			{
				targetSet = { plyPos.x, plyPos.y };
			}
		}

		renderR = AAngle;

		roationR = -((renderR * M_PI) / 180);

		moveFoward(npcC, speed, { cam.x, cam.y }, p2m, m2p, targetSet);
		x = npcC->GetWorldCenter().x * m2p;
		y = npcC->GetWorldCenter().y * m2p;

		speed = 10;

		if (_distance <= 600 && npcType == NPC_ENEMY)
		{
			agroed = true;
		}
		else
		{
			agroed = false;
		}

		if (wayPointDistance <= 100)
		{
			atWaypoint = true;
		}
		else
		{
			atWaypoint = false;
		}

		if (agroed && !isDead)
		{
			if (reload)
			{
				if (SDL_GetTicks() - reloadTimer >= bInf.reloadTime)
				{
					bulletsShot = 0;
					reload = false;
				}
			}

			if (SDL_GetTicks() - time >= /*shoot delay in ticks*/bInf.bulletDelay && !reload)
			{
				time = SDL_GetTicks();
				
				for (int i = 0; i < bInf.bulletsPerShot; i++)
				{
					//select a random direction based on npc rotaion
					double f = (double)rand() / RAND_MAX;
					double shootOffest = (AvMath::toDegrees(npcC->GetAngle()) - bInf.bulletSpread) + f * ((AvMath::toDegrees(npcC->GetAngle()) + bInf.bulletSpread) - (AvMath::toDegrees(npcC->GetAngle()) - bInf.bulletSpread));

					//shoot
					_projectile tempP = { 0, 0, 0, 0, 0, 0, 0 };

					SDL_Point trackPoint = AvMath::PolyGon::rotate_point({ x + (texture.getWidth() / 2), y + (texture.getHeight() / 2) }, angle, { x + (texture.getWidth() / 2), y + (texture.getHeight() / 4) - 10 });

					tempP.x = npcC->GetWorldPoint(b2Vec2(0, 1)).x * m2p;
					tempP.y = npcC->GetWorldPoint(b2Vec2(0, 1)).y * m2p;
					tempP.xVel = 0;
					tempP.yVel = 0;
					tempP.storedRot = -((shootOffest * M_PI) / 180);
					tempP.storedVX = xVel;
					tempP.storedVY = yVel;
					tempP.height = bInf.height;
					tempP.width = bInf.width;

					tempP.damage = bInf.damagePerHit;

					npcProjectile.push_back(tempP);
				}

				bulletsShot++;
				if (bulletsShot >= bInf.clipSize)
				{
					reload = true;
					reloadTimer = SDL_GetTicks();
				}
				else
				{
					reload = false;
				}
			}
		}

		for (int i = 0; i < npcProjectile.size(); i++)
		{
			//update projectiles
			npcProjectile.at(i).xVel = npcProjectile.at(i).storedVX + (double)20/*bullet speed in pps*/ * (double)sin((double)npcProjectile.at(i).storedRot);
			npcProjectile.at(i).yVel = npcProjectile.at(i).storedVY + (double)20/*bullet speed in pps*/ * (double)cos((double)npcProjectile.at(i).storedRot);

			npcProjectile.at(i).x += npcProjectile.at(i).xVel;
			npcProjectile.at(i).y += npcProjectile.at(i).yVel;

			if (AVE_findM({ x, y }, { npcProjectile.at(i).x, npcProjectile.at(i).y }) >= screenW + (screenW / 4))
			{
				npcProjectile.erase(npcProjectile.begin() + i);
			}
		}
	}
}

void gNpc::render(SDL_Renderer* mren, SDL_Rect cam, b2Body* npcB, float m2p)
{
	SDL_Point centerOfP = { texture.getWidth() / 2, texture.getHeight() / 2 };
	texture.render((npcB->GetWorldCenter().x * m2p) - (texture.getWidth() / 2) - cam.x, (npcB->GetWorldCenter().y * m2p) - (texture.getHeight() / 2) - cam.y, mren, NULL, npcB->GetAngle() * (180 / M_PI), &centerOfP, SDL_FLIP_VERTICAL);

	for (int i = 0; i < npcProjectile.size(); i++)
	{
		SDL_SetRenderDrawColor(mren, 255, 0, 0, 255);
		
		SDL_Rect refRect = { npcProjectile.at(i).x - cam.x, npcProjectile.at(i).y - cam.y, 20, 20 };
		SDL_RenderFillRect(mren, &refRect);

		SDL_SetRenderDrawColor(mren, 255, 255, 255, 255);
	}
}

vector<_projectile> gNpc::getNpcP()
{ 
	return npcProjectile;
}

bool gNpc::getAgroPlayer()
{
	return agroed;
}

double gNpc::getRep()
{
	return rep;
}

bool gNpc::getAtWapoint()
{
	return atWaypoint;
}

SDL_Rect gNpc::getNpcRect()
{
	return{ x, y, texture.getWidth(), texture.getHeight() };
}

void gNpc::free()
{
	npcProjectile.clear();
}

void gNpc::setBulletType(bulletInfo bfi)
{
	_bulletInfo = bfi;
}

bulletInfo gNpc::getBulletInf()
{
	return _bulletInfo;
}

void gNpc::checkBulletC(vector<_projectile> p)
{
	if (hp >= 0)
	{
		for (int i = 0; i < p.size(); i++)
		{
			if (checkColsion({ p.at(i).x, p.at(i).y, p.at(i).width, p.at(i).height }, { x, y, texture.getWidth(), texture.getHeight() }))
			{
				hp -= p.at(i).damage;
			}
		}
	}
}

bool gNpc::getDead()
{
	return isDead;
}

void gNpc::setDead(bool dead)
{
	isDead = dead;
}

void gNpc::setHP(double hp_)
{
	hp = hp_;
}