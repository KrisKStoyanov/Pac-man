#include "Window.h"

Window::Window(WINDOW_DESC window_desc)
{
	m_pWindow = SDL_CreateWindow(window_desc.title, window_desc.posX, window_desc.posY, window_desc.resX, window_desc.resY, window_desc.flags);
}

Window::~Window()
{
}
