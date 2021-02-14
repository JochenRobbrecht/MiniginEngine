#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "Components.h"


using namespace std;
using namespace std::chrono;

void Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void Minigin::LoadGame() 
{
	Scene* scene = SceneManager::GetInstance().CreateScene("Demo");

	auto gameObject = new GameObject();
	gameObject->AddRenderComponent("background.jpg", scene->GetRenderComponentsVec());
	scene->AddGameObject(gameObject);

	gameObject = new GameObject();
	gameObject->AddRenderComponent("logo.png", scene->GetRenderComponentsVec());
	gameObject->SetPosition(216, 180);
	scene->AddGameObject(gameObject);
	

	gameObject = new GameObject();
	auto renderComp = gameObject->AddRenderComponent(scene->GetRenderComponentsVec());
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textComp = gameObject->AddTextComponent("Programming 4 Assignment", font, renderComp);
	textComp->SetPosition(80, 20);
	scene->AddGameObject(gameObject);

	//add textComponent
	gameObject = new GameObject();
	renderComp = gameObject->AddRenderComponent(scene->GetRenderComponentsVec());
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textComponent = gameObject->AddTextComponent("0", font, renderComp);
	textComponent->SetPosition(50, 80);
	gameObject->AddFpsComponent(textComponent);
	scene->AddGameObject(gameObject);
}

void Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		
		sceneManager.SetScene("Demo");

		bool doContinue = true;
		nanoseconds lag {};
		float secondsPerFrame{ MsPerFrame / 1000.f };
		auto prevTime = high_resolution_clock::now();
		while (doContinue)
		{
			const auto currentTime = high_resolution_clock::now();
			auto elapsedSec = currentTime - prevTime;
			prevTime = currentTime;

			lag += elapsedSec;

			//INPUT
			doContinue = input.ProcessInput();

			//UPDATE
			while (lag >= milliseconds(MsPerFrame))
			{
				sceneManager.Update(secondsPerFrame);
				lag -= milliseconds(MsPerFrame);
			}
			//RENDER
			renderer.Render();// pass ratio for moving objs?

			//FPS CAP
			//auto sleepTime{duration_cast<nanoseconds>(milliseconds( MsPerFrame) - lag)};
			//this_thread::sleep_for(sleepTime);

		}
	}

	Cleanup();
}

