#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Scene.h"
#include "Components.h"
#include "ComponentsNoInheritance.h"
#include "Time.h"
#include "Commands.h"
#include "SoundSystem.h"
#include <thread>
#include <SDL.h>

Minigin::Minigin()
	:m_MsPerFrame{}
{
}

void Minigin::Initialize()
{
	_putenv("SDL_AUDIODRIVER=DirectSound");

	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	m_pWindow = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1080,
		640,
		SDL_WINDOW_OPENGL
	);

	if (m_pWindow == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
	
	Renderer::GetInstance().Init(m_pWindow);
}

/**
 * Code constructing the scene world starts here
 */
void Minigin::LoadGame()
{
	//auto& inputManager = InputManager::GetInstance();
	//inputManager.AddCommand(SDL({})
	/*
	//SCENES
	Scene* scene = SceneManager::GetInstance().CreateScene("Demo");

	//GAMEOBJECTS
	GameObject* backGround = new GameObject();
	backGround->AddRenderComponent("background.jpg", scene);
	scene->AddGameObject(backGround);

	GameObject* logo = new GameObject();
	RenderComponent* renderComp = logo->AddRenderComponent("logo.png", scene);
	renderComp->SetPosition(216, 180);
	scene->AddGameObject(logo);


	GameObject* title = new GameObject();
	renderComp = title->AddRenderComponent(scene);
	Font* font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	TextComponent* textComp{ new TextComponent("Programming 4 Assignment", font, title, renderComp) };
	title->AddComponent(textComp);
	textComp->SetPosition(80, 20);
	scene->AddGameObject(title);

	//add FpsComponent
	
	//FpsComponent* fpsComponent{ &FpsComponent::GetInstance()};



	unsigned int nrPlayers{ 3 };
	for (unsigned int playerIndex = 0; playerIndex < nrPlayers; playerIndex++)
	{
		//Qbert 
		GameObject* Qbert = new GameObject();
		scene->AddGameObject(Qbert);

		renderComp = Qbert->AddRenderComponent(scene);
		font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		textComp = new TextComponent("Qbert " + std::to_string(playerIndex), font, Qbert, renderComp);
		textComp->SetPosition(50 , 300 + float(50 * playerIndex));
		Qbert->AddComponent(textComp);

		InputComponent* inputComponent{ new InputComponent(Qbert, playerIndex) };
		Qbert->AddComponent(inputComponent);

		//Qbert lifecomp
		int maxLives{ 3 };
		GameObject* lifeQbert = new GameObject();
		scene->AddGameObject(lifeQbert);

		renderComp = lifeQbert->AddRenderComponent(scene);
		font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		textComp = new TextComponent(std::to_string(maxLives) + " lives", font, lifeQbert, renderComp);
		textComp->SetPosition(200, 300 + float(50 * playerIndex));
		lifeQbert->AddComponent(textComp);
		LifeComponent* lifeComp{ new LifeComponent( lifeQbert, maxLives, playerIndex) };
		lifeQbert->AddComponent(lifeComp);

		//Qbert scoreComp
		GameObject* scoreQbert = new GameObject();
		scene->AddGameObject(scoreQbert);

		renderComp = scoreQbert->AddRenderComponent(scene);
		font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		textComp = new TextComponent("Score: 0", font, scoreQbert, renderComp);
		textComp->SetPosition(350, 300 + float(50 * playerIndex));
		scoreQbert->AddComponent(textComp);
		ScoreComponent* scoreComp{ new ScoreComponent(scoreQbert, playerIndex) };
		scoreQbert->AddComponent(scoreComp);

		//COMMANDS

		auto& inputManager = InputManager::GetInstance();
		std::shared_ptr<KillCommand> killCommand{ std::make_shared<KillCommand> (playerIndex) };
		std::shared_ptr<ColorChange> colorChangeCommand{ std::make_shared<ColorChange>(playerIndex) };
		std::shared_ptr<LevelFinish> levelFinishCommand{ std::make_shared<LevelFinish>(playerIndex) };
		std::shared_ptr<DefeatCoily> defeatCoilyCommand{ std::make_shared<DefeatCoily>(playerIndex) };
		std::shared_ptr<CatchSAndS> catchSAndSCommand{ std::make_shared<CatchSAndS>(playerIndex) };
		if (playerIndex == 2)// both keyboard/mouse and controller for same commands
		{

			inputManager.AddCommand({ SDL_KEYDOWN, SDLK_c, playerIndex }, colorChangeCommand);
			inputManager.AddCommand({ SDL_KEYDOWN, SDLK_l, playerIndex }, levelFinishCommand);
			inputManager.AddCommand({ SDL_KEYDOWN, SDLK_d, playerIndex }, defeatCoilyCommand);
			inputManager.AddCommand({ SDL_KEYDOWN, SDLK_r, playerIndex }, catchSAndSCommand);

			inputManager.AddCommand({ SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT, playerIndex }, killCommand);

			inputManager.AddCommand({ ButtonType::ButtonDown, ControllerButtonValue::A }, killCommand);
			inputManager.AddCommand({ ButtonType::ButtonDown, ControllerButtonValue::Left }, colorChangeCommand);
			inputManager.AddCommand({ ButtonType::ButtonDown, ControllerButtonValue::Up }, levelFinishCommand);
			inputManager.AddCommand({ ButtonType::ButtonDown, ControllerButtonValue::Down }, defeatCoilyCommand);
			inputManager.AddCommand({ ButtonType::ButtonDown, ControllerButtonValue::Right }, catchSAndSCommand);
		}
		else
		{
			inputManager.AddCommand({ ButtonType::ButtonDown, ControllerButtonValue::A }, killCommand);
			inputManager.AddCommand({ ButtonType::ButtonDown, ControllerButtonValue::Left }, colorChangeCommand);
			inputManager.AddCommand({ ButtonType::ButtonDown, ControllerButtonValue::Up }, levelFinishCommand);
			inputManager.AddCommand({ ButtonType::ButtonDown, ControllerButtonValue::Down }, defeatCoilyCommand);
			inputManager.AddCommand({ ButtonType::ButtonDown, ControllerButtonValue::Right }, catchSAndSCommand);
		}
		killCommand->AddObserver(lifeComp);
		colorChangeCommand->AddObserver(scoreComp);
		levelFinishCommand->AddObserver(scoreComp);
		defeatCoilyCommand->AddObserver(scoreComp);
		catchSAndSCommand->AddObserver(scoreComp);

		killCommand->AddObserver(lifeQbert->GetComponent<TextComponent>());
		colorChangeCommand->AddObserver(scoreQbert->GetComponent<TextComponent>());
		levelFinishCommand->AddObserver(scoreQbert->GetComponent<TextComponent>());
		defeatCoilyCommand->AddObserver(scoreQbert->GetComponent<TextComponent>());
		catchSAndSCommand->AddObserver(scoreQbert->GetComponent<TextComponent>());
	}
	*/
}

void Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
	SDL_Quit();
}

void Minigin::Run()
{
	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& inputManager = InputManager::GetInstance();

		bool doContinue = true;
		auto prevTime = std::chrono::high_resolution_clock::now();

		//THREADS
		auto audioThread = std::thread(&AudioServiceLocator::Update);

		while (doContinue)
		{
			const auto currentTime = std::chrono::high_resolution_clock::now();
			auto elapsedSec = currentTime - prevTime;
			prevTime = currentTime;

			//INPUT
			inputManager.ProcessInput(doContinue);

			//UPDATE
			//float secondsPerFrame{ m_MsPerFrame / 1000.f };

			Time::GetInstance().SetElapsedSec(std::chrono::duration<float>(elapsedSec).count());//if fps capped, pass secondsPerFrame
			sceneManager.Update();


			//RENDER
			renderer.Render();

			//FPS CAP
			//auto sleepTime{ milliseconds(m_MsPerFrame) - elapsedSec };
			//this_thread::sleep_for(sleepTime);
		}

		if (audioThread.joinable())audioThread.join();
	}

	Cleanup();
}

