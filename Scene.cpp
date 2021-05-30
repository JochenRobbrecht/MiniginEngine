#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "Components.h"
#include "ComponentsNoInheritance.h"
#include <algorithm>

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
	for (RenderComponent* renderComp : m_pRenderComponents)
	{
		delete renderComp;
	}
};

void Scene::AddGameObject( GameObject* object)
{
	m_pObjects.push_back(object);
}

void Scene::Update()
{
	FpsComponent::GetInstance().Update();

	for(GameObject* object : m_pObjects)
	{
		object->Update();
	}

	//check delete game objects
	auto itGameObj = std::remove_if(m_pObjects.begin(), m_pObjects.end(), [](GameObject* gameObject)
		{
			if (gameObject->GetMarkedDead())
			{
				delete gameObject;
				return true;
			}
			return false;
		});
		m_pObjects.erase(itGameObj, m_pObjects.end());

	//check delete render comps
	auto itRender = std::remove_if(m_pRenderComponents.begin(), m_pRenderComponents.end(), [](RenderComponent* renderComp)
		{
			if (renderComp->GetMarkedDead())
			{
				delete renderComp;
				return true;
			}
			return false;
		});
		m_pRenderComponents.erase(itRender, m_pRenderComponents.end());
		//Sort
		SortRenderComponents();
}

void Scene::Render() const
{
	for (RenderComponent* renderComp : m_pRenderComponents)
	{
		renderComp->Render(); 
	}
}

void Scene::SortRenderComponents()
{
	std::sort(m_pRenderComponents.begin(), m_pRenderComponents.end(), [](const RenderComponent* renderComp1, const RenderComponent* renderComp2)
		{
			return renderComp1->GetDrawOrder() < renderComp2->GetDrawOrder();
		});
}

std::string Scene::GetName() const
{
	return m_Name;
}

std::vector<RenderComponent*>& Scene::GetRenderComponentsVec() 
{
	return m_pRenderComponents;
}

void Scene::AddRenderComponent(RenderComponent* renderComp)
{
	m_pRenderComponents.push_back(renderComp);
	//Sort
	SortRenderComponents();
}

