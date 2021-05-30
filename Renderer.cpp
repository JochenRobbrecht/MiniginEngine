#include "MiniginPCH.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture.h"//includes opengl
#include <imgui.h>
#include <imgui_impl_opengl2.h>
#include <imgui_impl_sdl.h>
#include "ComponentsNoInheritance.h"

#include <SDL.h>
//#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <SDL_image.h>
#include <SDL_ttf.h> 
#pragma comment (lib,"Glu32.lib")


//int GetOpenGLDriverIndex()
//{
//	auto openglIndex = -1;
//	const auto driverCount = SDL_GetNumRenderDrivers();
//	for (auto i = 0; i < driverCount; i++)
//	{
//		SDL_RendererInfo info;
//		if (!SDL_GetRenderDriverInfo(i, &info))
//			if (!strcmp(info.name, "opengl"))
//				openglIndex = i;
//	}
//	return openglIndex;
//}

void Renderer::Init(SDL_Window * window)
{
	m_pWindow = window;

	m_pContext = SDL_GL_CreateContext(m_pWindow);
	if (m_pContext == nullptr)
	{
		std::cerr << "Core::Initialize( ), error when calling SDL_GL_CreateContext: " << SDL_GetError() << std::endl;
		return;
	}
	// Set the swap interval for the current OpenGL context
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		std::cerr << "Core::Initialize( ), error when calling SDL_GL_SetSwapInterval: " << SDL_GetError() << std::endl;
		return;
	}

	// Set the Projection matrix to the identity matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set up a two-dimensional orthographic viewing region.
	gluOrtho2D(0, GetWindowWidth(), 0, GetWindowHeight()); // y from bottom to top

	// Set the viewport to the client window area
	// The viewport is the rectangular region of the window where the image is drawn.
	glViewport(0, 0, GetWindowWidth(), GetWindowHeight());

	// Set the Modelview matrix to the identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Enable color blending and use alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// InitializeGame PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cerr << "Core::Initialize( ), error when calling IMG_Init: " << IMG_GetError() << std::endl;
		return;
	}

	// InitializeGame SDL_ttf
	if (TTF_Init() == -1)
	{
		std::cerr << "Core::Initialize( ), error when calling TTF_Init: " << TTF_GetError() << std::endl;
		return;
	}

	//m_pRenderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	/*if (m_pRenderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}*/
	
	IMGUI_CHECKVERSION();
	
	ImGui::SetCurrentContext(ImGui::CreateContext());
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void Renderer::Render() const
{
	//SDL_RenderClear(m_pRenderer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	SceneManager::GetInstance().Render();

	if(m_RenderImGui)
		RenderImgui();

	//std::cout << FpsComponent::GetInstance().GetAverageFps() << std::endl;

	//SDL_RenderPresent(m_pRenderer);
	SDL_GL_SwapWindow(m_pWindow);
}

void Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(m_pContext);

	/*if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}*/
}

int Renderer::GetWindowWidth() const
{
	int* width{new int()};
	SDL_GetWindowSize(m_pWindow, width, nullptr);
	if(width)
		return (*width);
	return 0;
}

int Renderer::GetWindowHeight() const
{
	int* heigth{ new int() };
	SDL_GetWindowSize(m_pWindow, nullptr, heigth);
	if (heigth)
		return (*heigth);
	return 0;
}

Renderer::Renderer()
	: m_pWindow{}
	//, m_pRenderer{}
	, m_RenderImGui{ true }
	, m_ShowDebug{ true }
	, m_ShowDemo{ false }
	, m_pContext{ nullptr }
{
}

void Renderer::RenderImgui() const
{
	//ImGui
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_pWindow);
	ImGui::NewFrame();

	if (m_ShowDemo)
		ImGui::ShowDemoWindow();
	if (m_ShowDebug)
		RenderDebugImGui();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::RenderDebugImGui() const
{
	//if gameplay ui with imgui:
	//make ui component, update those here, make components for certain functionalities
	
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(250, 400), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("MiniginEngine"))
	{
		if (ImGui::CollapsingHeader("Debug Tool"))
		{
			ImGui::Text("STATS");
			ImGui::Indent();
			ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
			ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
			ImGui::Text("%.0f AVERAGE FPS", float(FpsComponent::GetInstance().GetAverageFps()));
			ImGui::Unindent();

			ImGui::PushItemWidth(230);

			auto list = FpsComponent::GetInstance().GetFpsList();
			//get array size
			const int sizeArr{ 20 };
			float arr[sizeArr] {};
			int index{};
			std::list<int>::const_iterator it{ list.begin() };

			int sizeDiff{ int(list.size()) - sizeArr };
			while (sizeDiff > 0)
			{
				it++;
				sizeDiff--;
			}

			while (it != list.end())
			{
				arr[index] = float(*it);
					index++;
					it++;
			}

			ImGui::PlotLines("", arr, IM_ARRAYSIZE(arr));
		}
	}
	ImGui::End();
}
