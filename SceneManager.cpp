#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SoundSystem.h"

void SceneManager::Update()
{
	if (m_pCurrentScene)m_pCurrentScene->Update();
}

void SceneManager::Render()
{
	if (m_pCurrentScene)m_pCurrentScene->Render();
}

SceneManager::SceneManager()
	:m_pCurrentScene{nullptr}
{
}

SceneManager::~SceneManager()
{
	for (Scene* scene : m_pScenes)
	{
		delete scene;
	}
}

void SceneManager::SetScene(const std::string& name)
{
	Scene* scene = GetScene(name);
	m_pCurrentScene = scene;
}

Scene* SceneManager::CreateScene(const std::string& name)
{
	Scene* scene = new Scene(name);
	m_pScenes.push_back(scene);
	return scene;
}

Scene* SceneManager::GetScene(const std::string& name)
{
	auto sceneIt = std::find_if(m_pScenes.begin(), m_pScenes.end(), [name](const Scene* scene)
		{
			return scene->GetName() == name;
		});
	if (sceneIt == m_pScenes.end())return CreateScene(name);
	
	return (*sceneIt);
}


