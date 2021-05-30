#pragma once
#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;
typedef void* SDL_GLContext;

class Texture;
/**
 * Simple RAII wrapper for the SDL renderer
 */
class Renderer final : public Singleton<Renderer>
{
	friend class Singleton;
public:
	void Init(SDL_Window* window);
	void Render() const;
	void Destroy();

	int GetWindowWidth()const;
	int GetWindowHeight()const;

	//SDL_Renderer* GetSDLRenderer() const { return m_pRenderer; }
private:
	Renderer();

	void RenderImgui()const;
	void RenderDebugImGui()const;

	//SDL_Renderer* m_pRenderer;
	SDL_Window* m_pWindow;
	SDL_GLContext m_pContext;
	bool m_RenderImGui;
	bool m_ShowDemo;
	bool m_ShowDebug;
};


