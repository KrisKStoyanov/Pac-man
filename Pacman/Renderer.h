#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "DrawTextEntity.h"

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

	//Can be redesigned with a <template>/<concept>
	DrawEntity* CreateDrawEntity(const char* imageFilepath);
	DrawTextEntity* CreateDrawTextEntity(const char* text, const char* fontFilepath, int posX, int posY);

	void SetDrawEntityText(DrawTextEntity& drawEntity, const char* aText, const char* aFontFile);

	void DrawObject(DrawEntity& drawEntity, int posX = 0, int posY = 0);
private:
	SDL_Renderer* m_pRenderer;
	RENDERER_DESC m_desc;
};

