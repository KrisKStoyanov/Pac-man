#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

struct WINDOW_DESC
{
	const char* title = "Pacman";
	int posX = SDL_WINDOWPOS_UNDEFINED, posY = SDL_WINDOWPOS_UNDEFINED;
	int resX = 1024, resY = 768;
	Uint32 flags = SDL_WINDOW_OPENGL;
};

class Window
{
public:
	Window(WINDOW_DESC window_desc);
	~Window();

	bool Init();

	inline SDL_Window* GetWindow() { return m_pWindow; }
private:
	SDL_Window* m_pWindow;
	WINDOW_DESC m_desc;
};

