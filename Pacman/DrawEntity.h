#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

class DrawEntity
{
public:
	DrawEntity(SDL_Renderer* renderer, const char* anImage, int posX, int posY);
	/*DrawEntity(const DrawEntity& other);*/
	~DrawEntity();
	
	void SetPosition(int posX, int posY);
	void SetImage(SDL_Renderer* renderer, const char* anImage);

	SDL_Surface* GetSurface();
	SDL_Texture* GetTexture();

	SDL_Rect GetPosRect();
	SDL_Rect GetSizeRect();

protected:
	DrawEntity();
	
	SDL_Surface* mySurface;
	SDL_Texture* myTexture;

	SDL_Rect myPosRect;
	SDL_Rect mySizeRect;

	const char* imageFile;
};

