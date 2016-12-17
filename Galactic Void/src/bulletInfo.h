#ifndef __BULLETINFO_H_INCLUDED
#define __BULLETINFO_H_INCLUDED

#include <iostream>
#include <string>

enum bulletTypes{ BULLET_METAL, BULLET_PLASMA, BULLET_EMP };

struct bulletInfo
{
	double bulletSpread;
	double bulletDelay;
	double damagePerHit;

	int clipSize;
	double reloadTime;

	int bulletsPerShot;

	int width;
	int height;

	bulletTypes type;
};

#endif//__BULLETINFO_H_INCLUDED