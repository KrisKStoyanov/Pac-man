#include "DrawTextEntity.h"

DrawTextEntity::DrawTextEntity(SDL_Renderer* renderer, const char* aText, const char* aFontFile, int posX, int posY)
{
	myFont = TTF_OpenFont(aFontFile, 24);

	myFontColor = { 255,0,0,255 };
	SDL_Surface* surface = TTF_RenderText_Solid(myFont, aText, myFontColor);

	if (!surface)
	{
		return;
	}

	myTexture = SDL_CreateTextureFromSurface(renderer, surface);

	mySizeRect.x = 0;
	mySizeRect.y = 0;
	mySizeRect.w = surface->w;
	mySizeRect.h = surface->h;

	myPosRect.x = posX;
	myPosRect.y = posY;
	myPosRect.w = mySizeRect.w;
	myPosRect.h = mySizeRect.h;

	SDL_FreeSurface(surface);
}

DrawTextEntity::~DrawTextEntity()
{
	TTF_CloseFont(myFont);
}

void DrawTextEntity::SetText(SDL_Renderer* renderer, const char* aText, const char* aFontFile)
{
	SDL_Surface* surface = TTF_RenderText_Solid(myFont, aText, myFontColor);
	
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

SDL_Color DrawTextEntity::GetSDLColor()
{
	return myFontColor;
}

TTF_Font* DrawTextEntity::GetTTFFont()
{
	return myFont;
}
