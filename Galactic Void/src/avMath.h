#ifndef __AVMATH_H_INCLUDED
#define __AVMATH_H_INCLUDED

#include <math.h>
#include <SDL.h>

#ifndef PI
#define PI 3.14159265358979323846264338327950288
#endif

namespace AvMath
{
	bool isNeg(int number);

	bool isPos(int number);

	double toDegrees(double radians);

	double toRadians(double degrees);

	namespace Circle
	{
		double areaOfCircle(double radius);
		
		double cirOfCircle(double radius);
	}

	namespace PolyGon
	{
		SDL_Point rotate_point(SDL_Point orgin, float __angle, SDL_Point p);
	}

	namespace Rect
	{
		double areaOfRect(double w, double h);
	}

	namespace rnd
	{
		int roundUp(int numToRound, int multiple);
	}
}

#endif //__AVMATH_H_INCLUDED