#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Components.h"
#include "Font.h"

GameObject::GameObject()
	:m_Transform{}
	,m_pRenderComp {nullptr}
{
}

GameObject::~GameObject()
{
	if (m_pRenderComp)delete m_pRenderComp;
	//if we delete object during game, remove render comp from vec in minigin(mark dead?)
	for (Component* comp : m_pComponents)
	{
		delete comp;
	}
}

void GameObject::Update(float elapsedSec)
{
	for (Component* comp : m_pComponents)
	{
		comp->Update(elapsedSec);
	}
}

void GameObject::SetTexture(const std::string& filename)
{
	m_pRenderComp->SetTexture(filename);
}

void GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}


RenderComponent* GameObject::AddRenderComponent(const std::string& filename, std::vector<RenderComponent*>& renderComponents)
{
	m_pRenderComp = new RenderComponent(filename, this);
	renderComponents.push_back(m_pRenderComp);
	return m_pRenderComp;
}

RenderComponent* GameObject::AddRenderComponent(std::vector<RenderComponent*>& renderComponents)
{
	m_pRenderComp = new RenderComponent(this);
	renderComponents.push_back(m_pRenderComp);
	return m_pRenderComp;
}

TextComponent* GameObject::AddTextComponent(const std::string& text, Font* font, RenderComponent* renderComponent)
{
	TextComponent* textComp{ new TextComponent(text, font, this, renderComponent) };
	m_pComponents.push_back(textComp);
	return textComp;
}

FpsComponent* GameObject::AddFpsComponent(TextComponent* textComponent)
{
	FpsComponent* fpsComponent{ new FpsComponent(textComponent,this) };
	m_pComponents.push_back(fpsComponent);
	return fpsComponent;
}

Transform GameObject::GetTransform() const
{
	return m_Transform;
}

