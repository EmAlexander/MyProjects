#include "texture.h"
#include "stringS.h"

quebic::texture::GTexture::GTexture()
{
	
}

quebic::texture::GTexture::~GTexture()
{
	free();
}

void quebic::texture::GTexture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
	}
}

int quebic::texture::GTexture::getWidth()
{
	return mWidth;
}

int quebic::texture::GTexture::getHeight()
{
	return mHeight;
}

bool quebic::texture::GTexture::loadFromFile(std::string file, quebic::window::GWindow* mwin)
{
	free();
	std::vector<std::string> parcedF = quebic::stringS::split(file, '.');

	if (parcedF.size() >= 2)
	{
		SDL_Surface* tempSurface = NULL;

		parcedF.at(1) = quebic::stringS::stringToLower(parcedF.at(1));

		if (parcedF.at(1) == "png" || parcedF.at(1) == "jpg" || parcedF.at(1) == "bmp" || parcedF.at(1) == "gif")
		{
			tempSurface = IMG_Load(file.c_str());
			SDL_Texture* returnTexture = NULL;

			returnTexture = SDL_CreateTextureFromSurface(mwin->getRenderer(), tempSurface);
			if (returnTexture == NULL)
			{
				std::cout << "Unable to load texture: " << file.c_str() << " " << IMG_GetError() << std::endl;
			}

			mHeight = tempSurface->h;
			mWidth = tempSurface->w;

			SDL_FreeSurface(tempSurface);
			mTexture = returnTexture;
		}
		else
		{
			std::cout << "Unsupported file type ." << parcedF.at(1) << "!" << std::endl;
		}
	}
	else
	{
		std::cout << "loadFromFile\'s file needs a name and a type!" << std::endl;
	}

	return mTexture != NULL;
}

void quebic::texture::GTexture::render(int x, int y, quebic::window::GWindow* mwin, primitives::QRect* clip)
{
	if (mTexture != NULL)
	{
		SDL_Rect renderQuad = { x, y, mWidth, mHeight };

		if (clip != NULL)
		{
			renderQuad.h = clip->h;
			renderQuad.w = clip->w;
		}

		SDL_RenderCopy(mwin->getRenderer(), mTexture, clip, &renderQuad);
	}
}