#include "QMath.h"

double quebic::math::conversions::toRadians(double degrees)
{
	return (degrees * M_PI) / 180;
}

double quebic::math::conversions::toDegrees(double radians)
{
	return radians * (180 / M_PI);
}

double quebic::math::round::roundUp(int num, int m)
{
	if (m == 0)
		return num;

	int r = num % m;
	if (r == 0)
	{
		return num;
	}

	return num + m - r;
}


quebic::primitives::Qpoint quebic::math::polyTransformation::rotatePoint(quebic::primitives::Qpoint point, quebic::primitives::Qpoint orgin, double angle)
{
	float s = sin(angle);
	float c = cos(angle);

	point.x -= orgin.x;
	point.y -= orgin.y;

	float xnew = point.x * c + point.y * s;
	float ynew = -point.x * s + point.y * c;

	point.x = xnew + orgin.x;
	point.y = ynew + orgin.y;
	return point;
}

quebic::primitives::Qpoint quebic::math::polyTransformation::movePoint(quebic::primitives::Qpoint point, quebic::primitives::Qpoint translation)
{
	point.x += translation.x;
	point.y += translation.y;

	return point;
}

void quebic::math::rand::seedRand(unsigned int seed)
{
	srand(seed);
}

double quebic::math::rand::genRandomNum(double low, double high)
{
	double f = (double)std::rand() / RAND_MAX;
	return low + f * (high - low);
}

int quebic::math::round::convertAndRound(double num)
{
	return floor(num + 0.5);
}