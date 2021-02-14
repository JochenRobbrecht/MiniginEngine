#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "Components.h"


//unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name)
	: m_Name(name)
{}

Scene::~Scene()
{
	for (GameObject* object: m_pObjects)
	{
		delete object;
	}
};

void Scene::AddGameObject( GameObject* object)
{
	m_pObjects.push_back(object);
}

void Scene::Update(float elapsedSec)
{
	for(auto& object : m_pObjects)
	{
		object->Update(elapsedSec);
	}
}

void Scene::Render() const
{
	for (RenderComponent* renderComp : m_pRenderComponents)
	{
		renderComp->Render();
	}
}

std::string Scene::GetName() const
{
	return m_Name;
}

std::vector<RenderComponent*>& Scene::GetRenderComponentsVec() 
{
	return m_pRenderComponents;
}

