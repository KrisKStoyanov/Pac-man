#include "DrawEntity.h"

DrawEntity::DrawEntity(SDL_Renderer* renderer, const char* anImage, int posX, int posY) :
	mySurface(nullptr),
	myTexture(nullptr),
	myPosRect(), mySizeRect()
{
	mySurface = IMG_Load(anImage);

	if (!mySurface)
		return;

	myTexture = SDL_CreateTextureFromSurface(renderer, mySurface);

	mySizeRect.x = 0;
	mySizeRect.y = 0;
	mySizeRect.w = mySurface->w;
	mySizeRect.h = mySurface->h;

	myPosRect.x = posX;
	myPosRect.y = posY;
	myPosRect.w = mySizeRect.w;
	myPosRect.h = mySizeRect.h;
}

DrawEntity::DrawEntity() :
	mySurface(nullptr),
	myTexture(nullptr),
	myPosRect(), mySizeRect()
{
}

DrawEntity::~DrawEntity()
{
	SDL_DestroyTexture(myTexture);
	SDL_FreeSurface(mySurface);
}

void DrawEntity::SetPosition(int posX, int posY)
{
	myPosRect.x = posX;
	myPosRect.y = posY;
}

void DrawEntity::SetImage(SDL_Renderer* renderer, const char* anImage)
{
	mySurface = IMG_Load(anImage);

	if (!mySurface)
		return;
	SDL_DestroyTexture(myTexture);
	myTexture = SDL_CreateTextureFromSurface(renderer, mySurface);
}

SDL_Surface* DrawEntity::GetSurface()
{
	return mySurface;
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
