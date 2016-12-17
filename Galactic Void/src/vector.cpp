/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#include <iostream>
#include "vector.h"

using namespace std;

float AVE_findM(AVE_vector v1, AVE_vector v2)
{
	float _x = (v1.x - v2.x) * (v1.x - v2.x);
	float _y = (v1.y - v2.y) * (v1.y - v2.y);

	float returnfloat = sqrt(_x + _y);
	return returnfloat;
}

float AVE_findSlope(AVE_vector v1, AVE_vector v2, int SCREEN_W)
{
	float _y = v2.y - v1.y;
	float _x = v2.x - v1.x;

	if (v2.x == SCREEN_W / 2)
	{
		return 0;
	}

	float returnInt = _y / _x;
	return returnInt;
}

AVE_vector operator-(AVE_vector v1, AVE_vector v2)
{
	AVE_vector returnV;

	returnV.x = v1.x - v2.x;
	returnV.y = v1.y - v2.y;

	return returnV;
}

AVE_vector operator+(AVE_vector v1, AVE_vector v2)
{
	AVE_vector returnV;

	returnV.x = v1.x + v2.x;
	returnV.y = v1.y + v2.y;

	return returnV;
}