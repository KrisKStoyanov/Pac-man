#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "assert.h"
#include <iostream>
#include "DrawTextEntity.h"
//#include "GameEntity.h"

class Core
{
public:
	Core();
	~Core();
	bool Init();
	void Draw(const char* anImage, int aCellX = 0, int aCellY = 0);
	void OnStartFrameRender();
	void OnEndFrameRender();
	void Update(const Uint8*& keystate);
	void DrawText(const char* aText, const char* aFontFile, int aX, int aY);

	void DrawObject(DrawEntity& drawEntity, int posX = 0, int posY = 0);
	void Shutdown();

	inline SDL_Window* GetWindow() 
	{
		return m_pWindow;
	};
	inline SDL_Renderer* GetRenderer()
	{
		return m_pRenderer;
	}
private:
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
};

