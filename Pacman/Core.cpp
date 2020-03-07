#include "Core.h"

Core::Core(const CORE_DESC& core_desc) : 
	m_pRenderer(nullptr), 
	m_pWindow(nullptr)
{
	m_pWindow = new Window(core_desc.window_desc);
	m_pRenderer = new Renderer(core_desc.renderer_desc);
}

bool Core::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	if (m_pWindow->Init())
	{
		m_pRenderer->Init(*m_pWindow->GetWindow());
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	if (TTF_Init() != 0)
	{
		SDL_Log("Unable to initialize TTF: %s", SDL_GetError());
		return false;
	}
	return true;
}

void Core::Shutdown()
{
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	SAFE_DELETE(m_pRenderer);
	SAFE_DELETE(m_pWindow);
}
