#include "Renderer.h"

Renderer::Renderer(RENDERER_DESC renderer_desc) : 
	m_desc(renderer_desc), m_pRenderer(nullptr)
{

}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(m_pRenderer);
}

bool Renderer::Init(SDL_Window& window)
{
	m_pRenderer = SDL_CreateRenderer(&window, m_desc.driverIndex, m_desc.flags);
	if (!m_pRenderer)
	{
		SDL_Log("Unable to create a renderer: %s", SDL_GetError());
		return false;
	}
	return true;
}

void Renderer::OnStartFrameRender()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);
}

void Renderer::DrawObject(DrawEntity& drawEntity, int posX, int posY)
{
	if (posX == 0 && posY == 0)
	{
		SDL_RenderCopy(m_pRenderer, drawEntity.GetTexture(), &drawEntity.GetSizeRect(), &drawEntity.GetPosRect());
	}
	else
	{
		SDL_Rect sizeRect = drawEntity.GetSizeRect();

		SDL_Rect posRect;
		posRect.x = posX;
		posRect.y = posY;
		posRect.w = sizeRect.w;
		posRect.h = sizeRect.h;
		SDL_RenderCopy(m_pRenderer, drawEntity.GetTexture(), &sizeRect, &posRect);
	}
}

void Renderer::OnEndFrameRender()
{
	SDL_RenderPresent(m_pRenderer);
	SDL_Delay(1);
}

DrawEntity* Renderer::CreateDrawEntity(const char* imageFilepath)
{
	return new DrawEntity(m_pRenderer, imageFilepath);
}

DrawTextEntity* Renderer::CreateDrawTextEntity(const char* text, const char* fontFilepath, int posX, int posY)
{
	return new DrawTextEntity(m_pRenderer, text, fontFilepath, posX, posY);
}

void Renderer::SetDrawEntityText(DrawTextEntity& drawEntity, const char* aText, const char* aFontFile)
{
	drawEntity.SetText(m_pRenderer, aText, aFontFile);
}
