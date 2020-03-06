#include "DrawTextEntity.h"

DrawTextEntity::DrawTextEntity(SDL_Renderer* renderer, const char* aText, const char* aFontFile, int posX, int posY)
{
	myFont = TTF_OpenFont(aFontFile, 24);

	myFontColor = { 255,0,0,255 };
	mySurface = TTF_RenderText_Solid(myFont, aText, myFontColor);

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

DrawTextEntity::~DrawTextEntity()
{
	TTF_CloseFont(myFont);
}

void DrawTextEntity::SetText(SDL_Renderer* renderer, const char* aText, const char* aFontFile)
{
	if (mySurface)
	{
		SDL_FreeSurface(mySurface);
	}
	mySurface = TTF_RenderText_Solid(myFont, aText, myFontColor);
	
	if (!mySurface)
	{
		return;
	}
		
	if (myTexture)
	{
		SDL_DestroyTexture(myTexture);
	}
	myTexture = SDL_CreateTextureFromSurface(renderer, mySurface);
}

SDL_Color DrawTextEntity::GetSDLColor()
{
	return myFontColor;
}

TTF_Font* DrawTextEntity::GetTTFFont()
{
	return myFont;
}
