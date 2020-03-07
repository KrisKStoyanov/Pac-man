#include "DrawEntity.h"

DrawEntity::DrawEntity(SDL_Renderer* renderer, const char* anImage) :
	myTexture(nullptr),
	myPosRect(), mySizeRect()
{
	SDL_Surface* surface = IMG_Load(anImage);
	
	if (!surface)
	{
		return;
	}

	myTexture = SDL_CreateTextureFromSurface(renderer, surface);

	mySizeRect.x = 0;
	mySizeRect.y = 0;
	mySizeRect.w = surface->w;
	mySizeRect.h = surface->h;

	myPosRect.x = 0;
	myPosRect.y = 0;
	myPosRect.w = mySizeRect.w;
	myPosRect.h = mySizeRect.h;

	SDL_FreeSurface(surface);
}

DrawEntity::DrawEntity() :
	myTexture(nullptr),
	myPosRect(), mySizeRect()
{
}

DrawEntity::~DrawEntity()
{
	SDL_DestroyTexture(myTexture);
}

void DrawEntity::SetPosition(int posX, int posY)
{
	myPosRect.x = posX;
	myPosRect.y = posY;
}

void DrawEntity::SetImage(SDL_Renderer* renderer, const char* anImage)
{
	SDL_Surface* surface = IMG_Load(anImage);

	if (!surface)
	{
		return;
	}

	if (myTexture)
	{
		SDL_DestroyTexture(myTexture);
	}
	myTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

SDL_Texture* DrawEntity::GetTexture()
{
	return myTexture;
}

SDL_Rect DrawEntity::GetPosRect()
{
	return myPosRect;
}

SDL_Rect DrawEntity::GetSizeRect()
{
	return mySizeRect;
}
