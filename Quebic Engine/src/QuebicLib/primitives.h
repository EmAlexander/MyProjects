#ifndef __PRIMITIVES_H_INCLUDED
#define __PRIMITIVES_H_INCLUDED

#include "window.h"
#include <SDL.h>

namespace quebic
{
	namespace primitives
	{
		#ifndef __QRECT_DEFINED
		#define __QRECT_DEFINED
			typedef SDL_Rect QRect;
		#endif //__QRECT_DEFINED

		#ifndef __Qpoint_DEFINED
		#define __Qpoint_DEFINED
					typedef SDL_Point Qpoint;
		#endif //__Qpoint_DEFINED

		typedef SDL_Color QColor;

		struct QpointP{ double x; double y; double w; double h; };

		void setP2MScale(double w, double h);

		//renders a filled square on the specified screen
		void renderFilledSquare(quebic::window::GWindow* mwin, QRect rect, QColor color);

		//renders a empty square on the specified screen
		//opengl not enabled:
		//	thickness is how thick the sides are anything above 1 "can" cause frame drop
		//opengl enabled
		//	thickness is not supported
		void renderSquare(quebic::window::GWindow* mwin, QRect rect, QColor color, int thickness = 1);

		//renders a empty circle on specifed screen
		//NOTE: thickness is glitchy i don't reccomned useing it above 10
		//does not work with opengl
		void renderCircle(quebic::window::GWindow* mwin, int centerX, int centerY, double radius, QColor color, int thickness = 1);

		//renders a filled circle
		//opengl not enabled:
		//	causes high cpu
		//	DEVNOTE: needs to be optimized
		//opengl enabled:
		//	px to m cord is a bit off when scaleing screen.
		//	it would be best to not allow resizing if you are useing this function
		void renderFilledCircle(quebic::window::GWindow* mwin, int centerX, int centerY, int radius, QColor color);
	}
}

#endif //__PRIMITIVES_H_INCLUDED