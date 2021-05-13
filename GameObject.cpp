#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Components.h"
#include "Font.h"

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
	m_Transform.get()->SetPosition(x, y, 0.0f);
}


RenderComponent* GameObject::AddRenderComponent(const std::string& filename, std::vector<RenderComponent*>& sceneVec)
{
	RenderComponent* renderComp = new RenderComponent(filename, this);
	m_pRenderComponents.push_back(renderComp);
	sceneVec.push_back(renderComp);
	return renderComp;
}

RenderComponent* GameObject::AddRenderComponent(std::vector<RenderComponent*>& sceneVec)
{
	RenderComponent* renderComp = new RenderComponent(this);
	m_pRenderComponents.push_back(renderComp);
	sceneVec.push_back(renderComp);
	return renderComp;
}


void GameObject::AddComponent(Component* component)
{
	m_pComponents.push_back(component);
}	


std::shared_ptr<Transform> GameObject::GetTransform()
{
	return m_Transform;
}


bool GameObject::GetMarkedDead()
{
	return m_MarkedDead;
}

void GameObject::MarkDead()
{
	m_MarkedDead = true;
}

