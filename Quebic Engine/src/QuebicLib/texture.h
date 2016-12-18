#ifndef __TEXTURE_H_INCLUDED
#define __TEXTURE_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

#include "window.h"

namespace quebic
{
	namespace primitives
	{
		#ifndef __QRECT_DEFINED
		#define __QRECT_DEFINED
				typedef SDL_Rect QRect;
		#endif //__QRECT_DEFINED
	}
	namespace texture
	{
		class GTexture
		{
		public:
			GTexture();
			~GTexture();

			bool loadFromFile(std::string file, quebic::window::GWindow* mwin);
			void render(int x, int y, quebic::window::GWindow* mwin, primitives::QRect* clip = NULL);

			void free();
			int getWidth();
			int getHeight();
		private:
			SDL_Texture* mTexture;

			int mHeight;
			int mWidth;
		};
	}
}

#endif //__TEXTURE_H_INCLUDED