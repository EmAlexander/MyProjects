#include "avMath.h"

bool AvMath::isNeg(int number)
{
	if (number < 0)
	{
		return true;
	}
	else if (number > 0)
	{
		return false;
	}
	else
	{
		return false;
	}
}

bool AvMath::isPos(int number)
{
	if (number < 0)
	{
		return false;
	}
	else if (number > 0)
	{
		return true;
	}
	else
	{
		return true;
	}
}

double AvMath::toDegrees(double radians)
{
	return radians * (180 / PI);
}

double AvMath::toRadians(double degrees)
{
	return (degrees * PI) / 180;
}

double AvMath::Circle::areaOfCircle(double radius)
{
	return PI * (radius * radius);
}

double AvMath::Circle::cirOfCircle(double radius)
{
	return 2 * PI * radius;
}

double AvMath::Rect::areaOfRect(double w, double h)
{
	return w * h;
}

int AvMath::rnd::roundUp(int numToRound, int multiple)
{
	if (multiple == 0)
		return numToRound;

	int remainder = numToRound % multiple;
	if (remainder == 0)
	{
		return numToRound;
	}

	return numToRound + multiple - remainder;
}


SDL_Point AvMath::PolyGon::rotate_point(SDL_Point orgin, float __angle, SDL_Point p)
{
	float s = sin(__angle);
	float c = cos(__angle);

	// translate point back to origin:
	p.x -= orgin.x;
	p.y -= orgin.y;

	// rotate point
	float xnew = p.x * c + p.y * s;
	float ynew = -p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + orgin.x;
	p.y = ynew + orgin.y;
	return p;
}