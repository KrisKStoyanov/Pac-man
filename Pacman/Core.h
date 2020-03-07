#pragma once
#include "Window.h"
#include "Renderer.h"

struct CORE_DESC
{
	WINDOW_DESC window_desc;
	RENDERER_DESC renderer_desc;
};

class Core
{
public:
	Core(const CORE_DESC& core_desc);
	bool Init();
	void Shutdown();

	inline Window* GetWindow() 
	{
		return m_pWindow;
	};
	inline Renderer* GetRenderer()
	{
		return m_pRenderer;
	}
private:
	Window* m_pWindow;
	Renderer* m_pRenderer;
};

