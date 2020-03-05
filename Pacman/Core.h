#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "assert.h"
#include "pacman.h"
#include "drawer.h"
#include <iostream>

class Core
{
public:
	Core();
	~Core();
	bool Init();
	int Run();
	void Shutdown();
private:
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
};

