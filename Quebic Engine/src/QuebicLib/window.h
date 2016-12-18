#ifndef __window_H_INCLUDED
#define __window_H_INCLUDED

#include "SDL.h"
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <iostream>
#include <string>

namespace quebic
{
	namespace window
	{
		#define WINPOS_UNDEFINED SDL_WINDOWPOS_UNDEFINED
		#define WINPOS_CENTERED SDL_WINDOWPOS_CENTERED

		//a class for creating and handleing windows
		class GWindow
		{
		public:
			GWindow();
			//automatically destroys window and renderer
			~GWindow();

			//creates window and renderer with prams
			//quebic::gl::initGl() must be called before you init a window with opengl
			bool init(std::string windowName, int winPosX, int winPosY, int winDimentW, int winDimentH, bool resizable = false, bool opengl = false, bool vsync = true);

			//handles window events
			//such as exiting, minimizing, etc...
			void handleEvents(SDL_Event e);

			//returns the renderer
			//NOTE: mainly used for external rendering
			SDL_Renderer* getRenderer();

			//gets the height of the window
			//NOTE: this updates when window is resized
			//can be used for relative rendering
			int getHeight();
			//gets the width of the window
			//NOTE: this updates when window is resized
			//can be used for relative rendering
			int getWidth();

			//opens the window if it has been closed
			void focus();

			//closes and removes the window
			//NOTE: you will not be able to use your window after this
			//unless you init again
			void destroyWindow();

			//if window can detect mouse events
			bool hasMouseFocus();
			//if window can detect key events
			bool hasKeyboardFocus();
			//if the window is minimized
			bool isMinimized();
			//if the window has been closed
			bool isShown();

			//clears the renderer
			//do before renderering
			void clearRenderer(GLbitfield clearType = GL_COLOR_BUFFER_BIT);

			//renders everything that is rendered to the screen
			//do after renderering
			void renderContents();

			//returns true if opengl is being used in that window
			bool getIsOpengl();

			//sets as current window to init opengl polys
			void setAsCurrent();

			//sets the window size
			void resizeWindow(int w, int h);
		private:
			bool isOpengl;

			SDL_GLContext gContext;

			SDL_Window* mWindow;
			SDL_Renderer* mRenderer;
			int mWindowID;

			//Window dimensions
			int mWidth;
			int mHeight;

			//Window focus
			bool mMouseFocus;
			bool mKeyboardFocus;
			bool mFullScreen;
			bool mMinimized;
			bool mShown;
		};
	}
}
#endif //__window_H_INCLUDED