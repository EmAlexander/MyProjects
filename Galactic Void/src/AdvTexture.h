#ifndef __ADVTEXTURE_H_INCLUDED
#define __ADVTEXTURE_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

class AdvTexture
{
public:
	AdvTexture();
	~AdvTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path, SDL_Renderer* mren, SDL_Window* winRef);

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Renderer* mren, SDL_Color textColor, TTF_Font* font);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Renderer* mren, int width = 0, int height = 0, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	bool createFromPx(Uint32* px, int width, int height, SDL_Renderer* mren, SDL_Window* winRef);

	bool createBlank(SDL_Renderer* mren, SDL_Window* winref, int width, int height, SDL_TextureAccess access = SDL_TEXTUREACCESS_STREAMING);

	//Set self as render target
	void setAsRenderTarget(SDL_Renderer* mren);

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//Pixel manipulators
	bool lockTexture();
	bool unlockTexture();
	void* getPixels();
	int getPitch();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;
	void* mPixels;
	int mPitch;

	//Image dimensions
	int mWidth;
	int mHeight;
};

#endif //__ADVTEXTURE_H_INCLUDED