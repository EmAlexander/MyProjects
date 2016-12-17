#include "AdvTexture.h"

AdvTexture::AdvTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	mPixels = NULL;
	mPitch = 0;
}

AdvTexture::~AdvTexture()
{
	//Deallocate
	free();
}

bool AdvTexture::loadFromFile(std::string path, SDL_Renderer* mren, SDL_Window* winRef)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "Unable to load image! SDL_image Error: " << IMG_GetError() << endl;
	}
	else
	{
		//Convert surface to display format
		SDL_Surface* formattedSurface = SDL_ConvertSurface(loadedSurface, SDL_GetWindowSurface(winRef)->format, NULL);
		if (formattedSurface == NULL)
		{
			cout << "Unable to convert loaded surface to display format! SDL Error: " << SDL_GetError() << endl;
		}
		else
		{
			//Create blank streamable texture
			newTexture = SDL_CreateTexture(mren, SDL_GetWindowPixelFormat(winRef), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
			if (newTexture == NULL)
			{
				cout << "Unable to create blank texture! SDL Error: " << SDL_GetError() << endl;
			}
			else
			{
				//Lock texture for manipulation
				SDL_LockTexture(newTexture, NULL, &mPixels, &mPitch);

				//Copy loaded/formatted surface pixels
				memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

				//Unlock texture to update
				SDL_UnlockTexture(newTexture);
				mPixels = NULL;

				//Get image dimensions
				mWidth = formattedSurface->w;
				mHeight = formattedSurface->h;
			}

			//Get rid of old formatted surface
			SDL_FreeSurface(formattedSurface);
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool AdvTexture::loadFromRenderedText(std::string textureText, SDL_Renderer* mren, SDL_Color textColor, TTF_Font* font)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(mren, textSurface);
		if (mTexture == NULL)
		{
			cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << endl;
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
	}


	//Return success
	return mTexture != NULL;
}

void AdvTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
		mPixels = NULL;
		mPitch = 0;
	}
}

void AdvTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void AdvTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void AdvTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void AdvTexture::render(int x, int y, SDL_Renderer* mren, int width, int height, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	if (width != 0)
	{
		renderQuad.w = width;
	}
	if (height != 0)
	{
		renderQuad.h = height;
	}

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(mren, mTexture, clip, &renderQuad, angle, center, flip);
}

int AdvTexture::getWidth()
{
	return mWidth;
}

int AdvTexture::getHeight()
{
	return mHeight;
}

bool AdvTexture::lockTexture()
{
	bool success = true;

	//Texture is already locked
	if (mPixels != NULL)
	{
		cout << "Texture is already locked!" << endl;
		success = false;
	}
	//Lock texture
	else
	{
		if (SDL_LockTexture(mTexture, NULL, &mPixels, &mPitch) != 0)
		{
			cout << "Unable to lock texture! " << SDL_GetError() << endl;
			success = false;
		}
	}

	return success;
}

bool AdvTexture::unlockTexture()
{
	bool success = true;

	//Texture is not locked
	if (mPixels == NULL)
	{
		cout << "Texture is not locked!" << endl;
		success = false;
	}
	//Unlock texture
	else
	{
		SDL_UnlockTexture(mTexture);
		mPixels = NULL;
		mPitch = 0;
	}

	return success;
}

void* AdvTexture::getPixels()
{
	return mPixels;
}

int AdvTexture::getPitch()
{
	return mPitch;
}

bool AdvTexture::createFromPx(Uint32* px, int width, int height, SDL_Renderer* mren, SDL_Window* winRef)
{
	free();
	SDL_Texture* newTexture = NULL;
	newTexture = SDL_CreateTexture(mren, SDL_GetWindowPixelFormat(winRef), SDL_TEXTUREACCESS_STREAMING, width, height);
	if (newTexture == NULL)
	{
		cout << "Unable to create texture from pixels! " << SDL_GetError() << endl;
		return false;
	}
	else
	{
		void* pixels;
		int pitch;
		SDL_LockTexture(newTexture, NULL, &pixels, &pitch);

		Uint32* createdTxPx = (Uint32*)pixels;

		for (int i = 0; i < width * height; i++)
		{
			createdTxPx[i] = px[i];
		}

		SDL_UnlockTexture(newTexture);
	}

	mTexture = newTexture;
	return true;
}

bool AdvTexture::createBlank(SDL_Renderer* mren, SDL_Window* winref, int width, int height, SDL_TextureAccess access)
{
	free();
	mTexture = SDL_CreateTexture(mren, SDL_GetWindowPixelFormat(winref), access, width, height);
	if (mTexture == NULL)
	{
		cout << "Unable to create blank texture! SDL Error: " << SDL_GetError() << endl;
	}
	else
	{
		mWidth = width;
		mHeight = height;
	}

	return mTexture != NULL;
}

void AdvTexture::setAsRenderTarget(SDL_Renderer* mren)
{
	//Make self render target
	SDL_SetRenderTarget(mren, mTexture);
}