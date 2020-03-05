#pragma once
#include "DrawEntity.h"

class DrawTextEntity : public DrawEntity
{
public:
	DrawTextEntity(SDL_Renderer* renderer, const char* aText, const char* aFontFile, int posX, int posY);
	/*DrawTextEntity(const DrawTextEntity& other);*/
	~DrawTextEntity();

	void SetText(SDL_Renderer* renderer, const char* aText, const char* aFontFile);

	SDL_Color GetSDLColor();
	TTF_Font* GetTTFFont();
private:
	TTF_Font* myFont;
	SDL_Color myFontColor;
};

