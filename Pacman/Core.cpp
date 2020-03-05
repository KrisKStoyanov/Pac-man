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

int Core::Run()
{
	Pacman* pacman = Pacman::Create();

	float lastFrame = (float)SDL_GetTicks() * 0.001f;
	SDL_Event event;
	while (SDL_PollEvent(&event) >= 0)
	{
		float currentFrame = (float)SDL_GetTicks() * 0.001f;
		float elapsedTime = currentFrame - lastFrame;

		if (!pacman->Update(elapsedTime))
			break;

		SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
		SDL_RenderClear(m_pRenderer);

		pacman->Draw(this);

		lastFrame = currentFrame;

		SDL_RenderPresent(m_pRenderer);
		SDL_Delay(1);
	}

	delete pacman;

	Shutdown();
	return EXIT_SUCCESS;
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

void Core::Shutdown()
{
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
