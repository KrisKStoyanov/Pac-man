#include "Window.h"

Window::Window(WINDOW_DESC window_desc) : 
	m_desc(window_desc), m_pWindow(nullptr)
{

}

Window::~Window()
{
	SDL_DestroyWindow(m_pWindow);
}

bool Window::Init()
{
	m_pWindow = SDL_CreateWindow(m_desc.title, m_desc.posX, m_desc.posY, m_desc.resX, m_desc.resY, m_desc.flags);
	if (!m_pWindow)
	{
		SDL_Log("Unable to create a window: %s", SDL_GetError());
		return false;
	}
	return true;
}
