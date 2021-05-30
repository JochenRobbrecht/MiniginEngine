#include "MiniginPCH.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Components.h"
#include "ComponentsNoInheritance.h"
#include "Font.h"
#include "Scene.h"


GameObject::GameObject()
	: m_pRenderComponents {}
	, m_pComponents{}
	, m_MarkedDead{false}
	, m_Transform{ std::make_shared<Transform>() }
{
}

GameObject::GameObject(std::shared_ptr<Transform> transform)
	: m_pRenderComponents{}
	, m_pComponents{}
	, m_MarkedDead{ false }
	, m_Transform{ transform }
{
}

GameObject::~GameObject()
{
	for (RenderComponent* comp : m_pRenderComponents)
	{
		comp->MarkDead();
	}
	for (Component* comp : m_pComponents)
	{
		delete comp;
	}
}

void GameObject::Update()
{
	for (Component* comp : m_pComponents)
	{
		comp->Update();
	}
}

void GameObject::SetPosition(float x, float y)
{
	m_Transform.get()->SetPosition(x, y);
}


RenderComponent* GameObject::AddRenderComponent(const std::string& filename, Scene* currentScene, unsigned int drawOrder, float scale)
{
	RenderComponent* renderComp = new RenderComponent(filename, this, drawOrder, scale);

	m_pRenderComponents.push_back(renderComp);
	currentScene->AddRenderComponent(renderComp);

	return renderComp;
}

RenderComponent* GameObject::AddRenderComponent( Scene* currentScene, unsigned int drawOrder, float scale)
{
	RenderComponent* renderComp = new RenderComponent(this, drawOrder, scale);

	m_pRenderComponents.push_back(renderComp);
	currentScene->AddRenderComponent(renderComp);

	return renderComp;
}



void GameObject::AddComponent(Component* component)
{
	m_pComponents.push_back(component);
}	


std::shared_ptr<Transform> GameObject::GetTransform() const
{
	return m_Transform;
}


RenderComponent* GameObject::GetRenderComponent() const
{
	if(!m_pRenderComponents.empty())
		return m_pRenderComponents[0];
	return nullptr;
}

bool GameObject::GetMarkedDead() const
{
	return m_MarkedDead;
}

void GameObject::MarkDead()
{
	m_MarkedDead = true;
}

