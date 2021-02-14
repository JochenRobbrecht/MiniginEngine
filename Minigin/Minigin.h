#pragma once
struct SDL_Window;

class Scene;
class GameObject;



class Minigin final
{
public:
	void Initialize();
	void LoadGame();
	void Cleanup();
	void Run();
private:

	const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
	SDL_Window* m_Window{};
};
