#include "window.h"

quebic::window::GWindow::GWindow()
{
}

quebic::window::GWindow::~GWindow()
{
	if (mWindow != NULL)
	{
		SDL_DestroyWindow(mWindow);
		mWindow = NULL;
	}

	if (mRenderer != NULL)
	{
		if (mShown)
			SDL_DestroyRenderer(mRenderer);
		mRenderer = NULL;
	}
}

bool quebic::window::GWindow::init(std::string windowName, int winPosX, int winPosY, int winDimentW, int winDimentH, bool resizable, bool opengl, bool vsync)
{
	Uint32 flags = SDL_WINDOW_SHOWN;
	if (resizable)
		flags += SDL_WINDOW_RESIZABLE;

	if (!opengl)
	{
		//Create window
		mWindow = SDL_CreateWindow(windowName.c_str(), winPosX, winPosY, winDimentW, winDimentH, flags);

		if (mWindow != NULL)
		{
			mMouseFocus = true;
			mKeyboardFocus = true;
			isOpengl = false;
			mWidth = winDimentW;
			mHeight = winDimentH;

			//Create renderer for window
			if (vsync)
			{
				mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			}
			else
			{
				mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
			}

			if (mRenderer == NULL)
			{
				std::cout << "Renderer could not be created! SDL Error: %s\n" << SDL_GetError() << std::endl;
				SDL_DestroyWindow(mWindow);
				mWindow = NULL;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Grab window identifier
				mWindowID = SDL_GetWindowID(mWindow);

				//Flag as opened
				mShown = true;
			}
		}
		else
		{
			std::cout << "Window could not be created! SDL Error: %s\n" << SDL_GetError() << std::endl;
		}
	}
	else
	{
		flags += SDL_WINDOW_OPENGL;

		mWindow = SDL_CreateWindow(windowName.c_str(), winPosX, winPosY, winDimentW, winDimentH, flags);
		if (mWindow != NULL)
		{
			gContext = SDL_GL_CreateContext(mWindow);
			if (gContext != NULL)
			{

				GLenum glewError = glewInit();
				if (glewError != GLEW_OK)
				{
					printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
				}
				
				if (SDL_GL_SetSwapInterval(vsync) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}

				mMouseFocus = true;
				mKeyboardFocus = true;
				mWidth = winDimentW;
				mHeight = winDimentH;

				mWindowID = SDL_GetWindowID(mWindow);

				mShown = true;

				isOpengl = true;

				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

				return mWindow != NULL;
				
			}
			else
			{
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				return false;
			}
		}
	}

	return mWindow != NULL && mRenderer != NULL;
}

int quebic::window::GWindow::getHeight()
{
	return mHeight;
}

int quebic::window::GWindow::getWidth()
{
	return mWidth;
}

SDL_Renderer* quebic::window::GWindow::getRenderer()
{
	if (isOpengl)
	{
		return NULL;
	}
	else
	{
		return mRenderer;
	}
}

bool quebic::window::GWindow::hasMouseFocus()
{
	return mMouseFocus;
}

bool quebic::window::GWindow::hasKeyboardFocus()
{
	return mKeyboardFocus;
}

bool quebic::window::GWindow::isMinimized()
{
	return mMinimized;
}

bool quebic::window::GWindow::isShown()
{
	return mShown;
}

void quebic::window::GWindow::handleEvents(SDL_Event e)
{
	if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID)
	{
		switch (e.window.event)
		{
			//Window appeared
		case SDL_WINDOWEVENT_SHOWN:
			mShown = true;
			break;

			//Window disappeared
		case SDL_WINDOWEVENT_HIDDEN:
			mShown = false;
			break;

			//Get new dimensions and repaint
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			if (isOpengl)
			{
				mWidth = e.window.data1;
				mHeight = e.window.data2;
				SDL_GL_MakeCurrent(mWindow, gContext);
				glViewport(0, 0, mWidth, mHeight);
			}
			else
			{
				mWidth = e.window.data1;
				mHeight = e.window.data2;
				SDL_RenderPresent(mRenderer);
			}
			break;

			//Repaint on expose
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(mRenderer);
			break;

			//Mouse enter
		case SDL_WINDOWEVENT_ENTER:
			mMouseFocus = true;
			break;

			//Mouse exit
		case SDL_WINDOWEVENT_LEAVE:
			mMouseFocus = false;
			break;

			//Keyboard focus gained
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			break;

			//Keyboard focus lost
		case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			break;

			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			mMinimized = true;
			break;

			//Window maxized
		case SDL_WINDOWEVENT_MAXIMIZED:
			mMinimized = false;
			break;

			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			mMinimized = false;
			break;
			//Hide on close
		case SDL_WINDOWEVENT_CLOSE:
			SDL_HideWindow(mWindow);
			break;
		}
	}
}

void quebic::window::GWindow::focus()
{
	//Restore window if needed
	if (!mShown)
	{
		SDL_ShowWindow(mWindow);
	}

	//Move window forward
	SDL_RaiseWindow(mWindow);
}

void quebic::window::GWindow::destroyWindow()
{
	if (mWindow != NULL)
	{
		SDL_DestroyWindow(mWindow);
		mWindow = NULL;
	}

	if (mRenderer != NULL)
	{
		if (mShown)
			SDL_DestroyRenderer(mRenderer);
		mRenderer = NULL;
	}
}

void quebic::window::GWindow::clearRenderer(GLbitfield clearType)
{
	if (isOpengl)
	{
		SDL_GL_MakeCurrent(mWindow, gContext);
		glClear(clearType);
	}
	else
	{
		SDL_RenderClear(mRenderer);
	}
}

void quebic::window::GWindow::renderContents()
{
	if (isOpengl)
	{
		SDL_GL_SwapWindow(mWindow);
	}
	else
	{
		SDL_RenderPresent(mRenderer);
	}
}

bool quebic::window::GWindow::getIsOpengl()
{
	return isOpengl;
}

void quebic::window::GWindow::setAsCurrent()
{
	SDL_GL_MakeCurrent(mWindow, gContext);
}

void quebic::window::GWindow::resizeWindow(int w, int h)
{
	SDL_SetWindowSize(mWindow, w, h);
	mWidth = w;
	mHeight = h;

	if (isOpengl)
	{
		SDL_GL_MakeCurrent(mWindow, gContext);
		glViewport(0, 0, mWidth, mHeight);
	}
}