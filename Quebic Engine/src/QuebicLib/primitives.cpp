#include "primitives.h"
#include <gl\glu.h>

double P2MW = 250;
double P2MH = 100;

void quebic::primitives::renderFilledSquare(quebic::window::GWindow* mwin, QRect rect, QColor color)
{
	if(!mwin->getIsOpengl())
	{
		SDL_SetRenderDrawColor(mwin->getRenderer(), color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(mwin->getRenderer(), &rect);
		SDL_SetRenderDrawColor(mwin->getRenderer(), 255, 255, 255, 255);
	}
	else
	{
		QpointP convertedRect = 
		{
			(rect.x - mwin->getWidth()) * (1.0 / mwin->getWidth()),
			-(rect.y - mwin->getHeight()) * (1.0 / mwin->getHeight()),
			rect.w * (1.0 / P2MW),
			rect.h * (1.0 / P2MH)
		};

		double r = color.r * (double)((double)1 / (double)255);
		double g = color.g * (double)((double)1 / (double)255);
		double b = color.b * (double)((double)1 / (double)255);
		double a = color.a * (double)((double)1 / (double)255);

		glBegin(GL_QUADS);
		glColor4f(r, g, b, a);
			glVertex3f(convertedRect.x, convertedRect.y, 0);
			glVertex3f(convertedRect.x + convertedRect.w, convertedRect.y, 0);
			glVertex3f(convertedRect.x + convertedRect.w, convertedRect.y -+ convertedRect.h, 0);
			glVertex3f(convertedRect.x, convertedRect.y - convertedRect.h, 0);
		glEnd();
	}
}

void quebic::primitives::renderSquare(quebic::window::GWindow* mwin, QRect rect, QColor color, int thickness)
{
	if (!mwin->getIsOpengl())
	{
		for (int i = 0; i < thickness; i++)
		{
			SDL_SetRenderDrawColor(mwin->getRenderer(), color.r, color.g, color.b, color.a);
			SDL_Rect _ref = { rect.x + i, rect.y + i, rect.w - (i * 2), rect.h - (i * 2) };
			SDL_RenderDrawRect(mwin->getRenderer(), &_ref);
			SDL_SetRenderDrawColor(mwin->getRenderer(), 255, 255, 255, 255);
		}
	}
	else
	{
		QpointP convertedRect =
		{
			(rect.x - mwin->getWidth()) * (1.0 / mwin->getWidth()),
			-(rect.y - mwin->getHeight()) * (1.0 / mwin->getHeight()),
			rect.w * (1.0 / P2MW),
			rect.h * (1.0 / P2MH)
		};

		double r = color.r * (double)((double)1 / (double)255);
		double g = color.g * (double)((double)1 / (double)255);
		double b = color.b * (double)((double)1 / (double)255);
		double a = color.a * (double)((double)1 / (double)255);

		glBegin(GL_LINE_LOOP);
			glColor4f(r, g, b, a);
			glVertex3f(convertedRect.x, convertedRect.y, 0);
			glVertex3f(convertedRect.x + convertedRect.w, convertedRect.y, 0);
			glVertex3f(convertedRect.x + convertedRect.w, convertedRect.y - +convertedRect.h, 0);
			glVertex3f(convertedRect.x, convertedRect.y - convertedRect.h, 0);
		glEnd();
	}
}

void quebic::primitives::renderCircle(quebic::window::GWindow* mwin, int centerX, int centerY, double radius, QColor color, int thickness)
{
	SDL_SetRenderDrawColor(mwin->getRenderer(), color.r, color.g, color.b, color.a);
	if (thickness != 0)
		radius -= (thickness / 2);
	double error = (double)-radius;
	double x = (double)radius - 0.5;
	double y = (double)0.5;
	double cx = centerX - 0.5;
	double cy = centerY - 0.5;

	while (x >= y)
	{
		SDL_Rect _ref = { (int)(cx + x), (int)(cy + y), thickness, thickness };
		SDL_RenderFillRect(mwin->getRenderer(), &_ref);
		_ref = { (int)(cx + y), (int)(cy + x), thickness, thickness };
		SDL_RenderFillRect(mwin->getRenderer(), &_ref);

		if (x != 0)
		{
			_ref = { (int)(cx - x), (int)(cy + y), thickness, thickness };
			SDL_RenderFillRect(mwin->getRenderer(), &_ref);
			_ref = { (int)(cx + y), (int)(cy - x), thickness, thickness };
			SDL_RenderFillRect(mwin->getRenderer(), &_ref);
		}

		if (y != 0)
		{
			_ref = { (int)(cx + x), (int)(cy - y), thickness, thickness };
			SDL_RenderFillRect(mwin->getRenderer(), &_ref);
			_ref = { (int)(cx - y), (int)(cy + x), thickness, thickness };
			SDL_RenderFillRect(mwin->getRenderer(), &_ref);
		}

		if (x != 0 && y != 0)
		{
			_ref = { (int)(cx - x), (int)(cy - y), thickness, thickness };
			SDL_RenderFillRect(mwin->getRenderer(), &_ref);
			_ref = { (int)(cx - y), (int)(cy - x), thickness, thickness };
			SDL_RenderFillRect(mwin->getRenderer(), &_ref);
		}

		error += y;
		++y;
		error += y;

		if (error >= 0)
		{
			--x;
			error -= x;
			error -= x;
		}
	}

	SDL_SetRenderDrawColor(mwin->getRenderer(), 255, 255, 255, 255);
}

void quebic::primitives::renderFilledCircle(quebic::window::GWindow* mwin, int centerX, int centerY, int radius, QColor color)
{
	if (!mwin->getIsOpengl())
	{
		SDL_SetRenderDrawColor(mwin->getRenderer(), color.r, color.g, color.b, color.a);
		int x = radius;
		int y = 0;
		int xChange = 1 - (radius << 1);
		int yChange = 0;
		int radiusError = 0;

		while (x >= y)
		{
			for (int i = centerX - x; i <= centerX + x; i++)
			{
				SDL_RenderDrawPoint(mwin->getRenderer(), i, centerY + y);
				SDL_RenderDrawPoint(mwin->getRenderer(), i, centerY - y);
			}
			for (int i = centerX - y; i <= centerX + y; i++)
			{
				SDL_RenderDrawPoint(mwin->getRenderer(), i, centerY + x);
				SDL_RenderDrawPoint(mwin->getRenderer(), i, centerY - x);
			}

			y++;
			radiusError += yChange;
			yChange += 2;
			if (((radiusError << 1) + xChange) > 0)
			{
				x--;
				radiusError += xChange;
				xChange += 2;
			}
		}
		SDL_SetRenderDrawColor(mwin->getRenderer(), 255, 255, 255, 255);
	}
	else
	{
		double newRaidusw = radius * (1.0 / P2MW);
		double newRaidush = radius * (1.0 / P2MH);

		double r = color.r * (double)((double)1 / (double)255);
		double g = color.g * (double)((double)1 / (double)255);
		double b = color.b * (double)((double)1 / (double)255);
		double a = color.a * (double)((double)1 / (double)255);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef((centerX - mwin->getWidth()) * (1.0 / mwin->getWidth()), -(centerY - mwin->getHeight()) * (1.0 / mwin->getHeight()), 0.0f);
		static const int circle_points = 100;
		static const float angle = 2.0f * 3.1416f / circle_points;
		
		glBegin(GL_POLYGON);
			glColor4f(r, g, b, a);
			double angle1 = 0.0;
			glVertex2d(newRaidusw * cos(0.0), newRaidush * sin(0.0));
			int i;
			for (i = 0; i<circle_points; i++)
			{
				glVertex2d(newRaidusw * cos(angle1), newRaidush *sin(angle1));
				angle1 += angle;
			}
		glEnd();
		glPopMatrix();
	}
}

void quebic::primitives::setP2MScale(double w, double h)
{
	P2MH = h;
	P2MW = w;
}