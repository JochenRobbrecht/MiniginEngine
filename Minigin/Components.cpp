#include "MiniginPCH.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "Font.h"

Component::Component(GameObject* parent)
	:m_pParent{ parent }
{
}


RenderComponent::RenderComponent(const std::string& filename, GameObject* parent)
	: m_pParent{parent}
	, m_pTexture{ nullptr }
{
	SetTexture(filename);
}

RenderComponent::RenderComponent(GameObject* parent)
	: m_pParent{ parent }
	,m_pTexture{nullptr}
{
}

RenderComponent::~RenderComponent()
{
	if(m_pTexture)delete m_pTexture;
}

void RenderComponent::Render() const
{
	if (m_pTexture)
	{
		const glm::vec3 pos = m_pParent->GetTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

void RenderComponent::SetTexture(const std::string& filename)
{
	if (m_pTexture)delete m_pTexture;
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void RenderComponent::SetTexture(Texture2D* texture2D)
{
	if (m_pTexture)delete m_pTexture;
	m_pTexture = texture2D;
}


TextComponent::TextComponent(const std::string& text, Font* font, GameObject* parent, RenderComponent* renderComponent)
	: Component{ parent }
	, m_NeedsUpdate(true)
	, m_Text(text)
	, m_pFont(font)
	, m_pRenderComponent(renderComponent)
{

}

TextComponent::~TextComponent()
{
	if (m_pFont)delete m_pFont;
}

void TextComponent::Update(float elapsedSec)
{
	if (m_NeedsUpdate)
	{
		elapsedSec;
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_pRenderComponent->SetTexture(new Texture2D(texture));
		m_NeedsUpdate = false;
	}
}

// This implementation uses the "dirty flag" pattern
void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void TextComponent::SetPosition(const float x, const float y)
{
	m_pParent->SetPosition(x, y);
}

FpsComponent::FpsComponent(TextComponent* textComponent, GameObject* parent)
	:Component{parent}
	,m_pTextComponent{textComponent}
	,M_FrameCount{0}
	,m_AccumulatedTime{0}
{
}

FpsComponent::~FpsComponent()
{
}

void FpsComponent::Update(float elapsedSec)
{
	m_AccumulatedTime += elapsedSec;
	if (m_AccumulatedTime > 1)
	{
		m_pTextComponent->SetText(std::to_string(M_FrameCount));
		m_AccumulatedTime -= m_AccumulatedTime;
		M_FrameCount = 0;
	}
	M_FrameCount++;
}
