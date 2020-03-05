#pragma once
#include "SDL.h"

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
private:
	SDL_Window* m_pWindow;
};

