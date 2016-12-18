#ifndef __QMATH_H_INCLUDED
#define __QMATH_H_INCLUDED

#include <SDL.h>
#include <math.h>
#include <cstdlib>

namespace quebic
{
	namespace primitives
	{
		#ifndef __Qpoint_DEFINED
		#define __Qpoint_DEFINED
				typedef SDL_Point Qpoint;
		#endif //__Qpoint_DEFINED
	}

	namespace math
	{
		namespace conversions
		{
			/*converts input to radians*/
			double toRadians(double degrees);

			/*converts input to degrees*/
			double toDegrees(double radians);
		}

		namespace round
		{
			/*rounds num the the closest multiple of "m" and returns*/
			double roundUp(int num, int m);

			/*
			converts a double or float into a int with rounding
			example: 
			round(0.5) returns 1 
			round(0.4) returns 0
			*/
			int convertAndRound(double num);
		}

		namespace polyTransformation
		{
			/*
			returns a point rotated around the orgin by the angle
			NOTE: angle must be in radians
			*/
			primitives::Qpoint rotatePoint(primitives::Qpoint point, primitives::Qpoint orgin, double angle);

			/*
			returns a point with the applyed translation
			*/
			primitives::Qpoint movePoint(primitives::Qpoint point, primitives::Qpoint translation);
		}

		namespace rand
		{
			/*seeds random number generator*/
			void seedRand(unsigned int seed);

			/*generates random number in double form*/
			double genRandomNum(double low, double high);
		}
	}
}

#endif//__QMATH_H_INCLUDED