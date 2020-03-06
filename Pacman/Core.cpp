#include "Core.h"

Core::Core(): 
	m_pRenderer(nullptr), 
	m_pWindow(nullptr)
{

}

Core::~Core()
{
	Shutdown();
}

bool Core::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	m_pWindow = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);
	if (!m_pWindow) 
	{
		SDL_Log("Unable to create a window: %s", SDL_GetError());
		return false;
	}
	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	if (TTF_Init() != 0)
	{
		SDL_Log("Unable to initialize TTF: %s", SDL_GetError());
		return false;
	}
	return true;
}

void Core::Draw(const char* anImage, int aCellX, int aCellY)
{
	SDL_Surface* surface = IMG_Load(anImage);

	if (!surface)
		return;

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(m_pRenderer, surface);

	SDL_Rect sizeRect;
	sizeRect.x = 0;
	sizeRect.y = 0;
	sizeRect.w = surface->w;
	sizeRect.h = surface->h;

	SDL_Rect posRect;
	posRect.x = aCellX;
	posRect.y = aCellY;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(m_pRenderer, optimizedSurface, &sizeRect, &posRect);
	SDL_DestroyTexture(optimizedSurface);
	SDL_FreeSurface(surface);
}

void Core::OnStartFrameRender()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);
}

void Core::OnEndFrameRender()
{
	SDL_RenderPresent(m_pRenderer);
	SDL_Delay(1);
}

void Core::Update(const Uint8*& keystate)
{
	SDL_Event curEvent;
	while (SDL_PollEvent(&curEvent) > 0)
	{
		keystate = SDL_GetKeyboardState(NULL);
	}
}

void Core::DrawText(const char* aText, const char* aFontFile, int aX, int aY)
{
	TTF_Font* font = TTF_OpenFont(aFontFile, 24);

	SDL_Color fg = { 255,0,0,255 };
	SDL_Surface* surface = TTF_RenderText_Solid(font, aText, fg);

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(m_pRenderer, surface);

	SDL_Rect sizeRect;
	sizeRect.x = 0;
	sizeRect.y = 0;
	sizeRect.w = surface->w;
	sizeRect.h = surface->h;

	SDL_Rect posRect;
	posRect.x = aX;
	posRect.y = aY;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(m_pRenderer, optimizedSurface, &sizeRect, &posRect);
	SDL_DestroyTexture(optimizedSurface);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
}

void Core::DrawObject(DrawEntity& drawEntity, int posX, int posY)
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

void Core::Shutdown()
{
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
}
