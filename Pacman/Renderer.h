#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "GameEntity.h"

struct RENDERER_DESC
{
	int driverIndex = -1;
	Uint32 flags = SDL_RENDERER_ACCELERATED;
};

class Renderer
{
public:
	Renderer(RENDERER_DESC renderer_desc);
	~Renderer();

	bool Init(SDL_Window& window);
	void OnStartFrameRender();
	void OnEndFrameRender();

	void DrawObject(DrawEntity& drawEntity, int posX = 0, int posY = 0);

	inline SDL_Renderer* GetRenderer() { return m_pRenderer; }
private:
	SDL_Renderer* m_pRenderer;
	RENDERER_DESC m_desc;
};

