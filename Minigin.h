#pragma once
struct SDL_Window;

class Minigin final
{
public:
	Minigin();
	void Initialize();
	void LoadGame();
	void Cleanup();
	void Run();
private:

	const int m_MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
	SDL_Window* m_pWindow{};
};
