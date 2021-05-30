#include "MiniginPCH.h"
#include "ComponentsNoInheritance.h"
#include "Renderer.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Texture.h"
#include "GameObject.h"
#include "Time.h"


RenderComponent::RenderComponent(const std::string& filename, GameObject* parent, unsigned int drawOrder, float scale)
	: m_pParent{ parent }
	, m_pTextures{}
	, m_pCurrentTexture{}
	, m_MarkedDead{ false }
	, m_DrawOrder{drawOrder}
	, m_Scale{scale}
{
	AddTexture(filename, 0);
}

RenderComponent::RenderComponent(GameObject* parent, unsigned int drawOrder, float scale)
	: m_pParent{ parent }
	, m_pTextures{}
	, m_pCurrentTexture{}
	, m_MarkedDead{ false }
	, m_DrawOrder{ drawOrder }
	, m_Scale{ scale }
{
}

RenderComponent::~RenderComponent()
{
	for (std::pair<const TextureId, Texture*>& p : m_pTextures)
	{
		delete p.second;
	}
}

void RenderComponent::Render() const
{
	if (m_pCurrentTexture.second)
	{
		const Point2f pos = m_pParent->GetTransform().get()->GetPosition();
		Rectf destRect{};
		destRect.leftBottom = pos;
		destRect.width = m_pCurrentTexture.second->GetWidth() * m_Scale;
		destRect.height = m_pCurrentTexture.second->GetHeight() * m_Scale;
		m_pCurrentTexture.second->Draw(destRect);
	}
}

void RenderComponent::SetPosition(float x, float y)
{
	m_pParent->GetTransform().get()->SetPosition(x, y);
}

void RenderComponent::SetScale(float scale)
{
	m_Scale = scale;
}

void RenderComponent::AddTexture(const std::string& filename, TextureId uniqueId)
{
	Texture* newTexture = new Texture(filename);
	if (!m_pCurrentTexture.second)
	{
		m_pCurrentTexture.second = newTexture;
		m_pCurrentTexture.first = uniqueId;
	}
	m_pTextures.insert(std::pair<TextureId, Texture*>(uniqueId, newTexture));
}

void RenderComponent::AddTexture(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor, TextureId uniqueId)
{
	Texture* newTexture = new Texture(text, fontPath, ptSize, textColor);
	if (!m_pCurrentTexture.second)
	{
		m_pCurrentTexture.second = newTexture;
		m_pCurrentTexture.first = uniqueId;
	}
	m_pTextures.insert(std::pair<TextureId, Texture*>(uniqueId, newTexture));
}

void RenderComponent::AddTexture(const std::string& text, TTF_Font* pFont, const Color4f& textColor, TextureId uniqueId)
{
	Texture* newTexture = new Texture(text, pFont, textColor);
	if (!m_pCurrentTexture.second)
	{
		m_pCurrentTexture.second = newTexture;
		m_pCurrentTexture.first = uniqueId;
	}
	m_pTextures.insert(std::pair<TextureId, Texture*>(uniqueId, newTexture));
}

void RenderComponent::ChangeTexture(const std::string& filename, TextureId uniqueId)
{
	auto it = m_pTextures.find(uniqueId);
	if (it != m_pTextures.end())
	{
		delete (*it).second;
		(*it).second = new Texture(filename);
		m_pCurrentTexture = (*it);
	}
}

void RenderComponent::ChangeTexture(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor, TextureId uniqueId)
{
	auto it = m_pTextures.find(uniqueId);
	if (it != m_pTextures.end())
	{
		delete (*it).second;
		(*it).second = new Texture(text, fontPath, ptSize, textColor);
		m_pCurrentTexture = (*it);
	}
}

void RenderComponent::ChangeTexture(const std::string& text, TTF_Font* pFont, const Color4f& textColor, TextureId uniqueId)
{
	auto it = m_pTextures.find(uniqueId);
	if (it != m_pTextures.end())
	{
		delete (*it).second;
		(*it).second = new Texture(text, pFont, textColor);
		m_pCurrentTexture = (*it);
	}
}

void RenderComponent::SetTexture(TextureId uniqueId)
{
	if (m_pCurrentTexture.first == uniqueId) return;
	auto it = m_pTextures.find(uniqueId);
	if (it != m_pTextures.end())
	{
		m_pCurrentTexture = (*it);
	}
}

void RenderComponent::SetTextureNext(bool reverse)
{
	if (m_pTextures.size() < 2)return;
	auto it = m_pTextures.find(m_pCurrentTexture.first + 1);
	if (it != m_pTextures.end())
	{
		m_pCurrentTexture = (*it);
	}
	else if(reverse)
	{
		m_pCurrentTexture = (*m_pTextures.begin());
	}

}

void RenderComponent::MarkDead()
{
	m_MarkedDead = true;
}

float RenderComponent::GetWidth() const
{
	return m_pCurrentTexture.second->GetWidth() * m_Scale;
}

float RenderComponent::GetHeight() const
{
	return m_pCurrentTexture.second->GetHeight() * m_Scale;
}

TextureId RenderComponent::GetCurrentTextureId() const
{
	return m_pCurrentTexture.first;
}

bool RenderComponent::GetMarkedDead()const
{
	return m_MarkedDead;
}

unsigned int RenderComponent::GetDrawOrder() const
{
	return m_DrawOrder;
}

FpsComponent::FpsComponent()
	: m_AverageFps{ -1 }
	, m_AccumulatedTime{ 0.f }
	, m_FrameUpdateInterval{ 2.f }
	, m_FrameCount{ 0 }
{
}

void FpsComponent::Update()
{
	m_AccumulatedTime += Time::GetInstance().GetElapsedSec();
	m_FrameCount++;

	if (m_AccumulatedTime > m_FrameUpdateInterval)
	{
		m_AverageFps = { int(1 / (m_FrameUpdateInterval / m_FrameCount)) };

		m_AccumulatedTime -= m_AccumulatedTime;
		m_FrameCount = 0;

		size_t fpsListCap{ 20 };

		//int fps{ int(1 / Time::GetInstance().GetElapsedSec()) };
		
		if (m_FpsList.size() >= fpsListCap)
			m_FpsList.pop_front();

		m_FpsList.push_back(m_AverageFps);
	}
}

int FpsComponent::GetAverageFps() const
{
	return m_AverageFps;
}

const int FpsComponent::GetFpsListSize() const
{
	return int(m_FpsList.size());
}

const std::list<int>& FpsComponent::GetFpsList() const
{
	return m_FpsList;
}
