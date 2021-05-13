#include "MiniginPCH.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "Font.h"
#include "Time.h"
#include "InputManager.h"
#include "Commands.h"
#include "Transform.h"
#include "SoundSystem.h"

Component::Component(GameObject* parent)
	:m_pParent{ parent }
{
}


RenderComponent::RenderComponent(const std::string& filename, GameObject* parent)
	: m_pParent{parent}
	, m_pTexture{ nullptr }
	, m_MarkedDead{false}
{
	SetTexture(filename);
}

RenderComponent::RenderComponent(GameObject* parent)
	: m_pParent{ parent }
	, m_pTexture{nullptr}
	, m_MarkedDead{ false }
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
		const glm::vec3 pos = m_pParent->GetTransform().get()->GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

void RenderComponent::SetPosition(float x, float y)
{
	m_pParent->GetTransform().get()->SetPosition(x, y, 0);
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

void RenderComponent::MarkDead()
{
	m_MarkedDead = true;
}

bool RenderComponent::GetMarkedDead()
{
	return m_MarkedDead;
}


TextComponent::TextComponent(const std::string& text, Font* font, GameObject* parent, RenderComponent* renderComponent)
	: ObserverComponent{ parent }
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

void TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
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

void TextComponent::OnNotify(GameObject*, Event event, unsigned int)
{
	switch (event)
	{
	case Event::PlayerDied:
	{LifeComponent* lifeComp = m_pParent->GetComponent<LifeComponent>();
	if (lifeComp)SetText(std::to_string(lifeComp->GetCurrentLives()) + " lives"); }
		break;
	case Event::ColorChange:
	case Event::LevelFinish:
	case Event::DefeatCoily:
	case Event::CatchSAndS:
	{ScoreComponent* scoreComp = m_pParent->GetComponent<ScoreComponent>();
	if (scoreComp)SetText("Score: " + std::to_string(scoreComp->GetCurrentScore())); }
		break;
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
	m_pRenderComponent->SetPosition(x, y);
}

FpsComponent::FpsComponent(TextComponent* textComponent, GameObject* parent)
	:Component{parent}
	,m_pTextComponent{textComponent}
	,m_AccumulatedTime{0}
	,m_FrameUpdateInterval{ 1.f }
	,m_FrameCount{0}
	,m_PrevAverageFps{0}
{
}

void FpsComponent::Update()
{
	m_AccumulatedTime += Time::GetInstance().GetElapsedSec();
	m_FrameCount++;
	if (m_AccumulatedTime > m_FrameUpdateInterval)
	{
		int averageFps{ int(1 / (m_FrameUpdateInterval / m_FrameCount)) };
		if (averageFps != m_PrevAverageFps)
		{
			m_pTextComponent->SetText(std::to_string(averageFps));
			m_PrevAverageFps = averageFps;
		}
		m_AccumulatedTime -= m_AccumulatedTime;
		m_FrameCount = 0;
	}
}

void FpsComponent::SetPosition(float x, float y)
{
	m_pTextComponent->SetPosition(x, y);
}

InputComponent::InputComponent(GameObject* parent, unsigned int playerIndex)
	:Component{parent}
	,m_PlayerIndex{playerIndex}
{
}

InputComponent::~InputComponent()
{
}

void InputComponent::Update()
{
	auto& inputManager = InputManager::GetInstance();
	auto vec = inputManager.GetCommandsToFire();
	for (std::shared_ptr<Command> command : vec)
	{
		if (command->GetPlayerIndex() == m_PlayerIndex)
		{
			command->Execute(m_pParent);
		}
	}
}

unsigned int InputComponent::GetPlayerIndex() const
{
	return m_PlayerIndex;
}

LifeComponent::LifeComponent( GameObject* parent, unsigned int maxLives, unsigned int playerIndex)
	: ObserverComponent{ parent }
	, m_MaxLives{maxLives}
	, m_CurrentLives{ maxLives }
	,m_PlayerIndex{playerIndex}
{
}

void LifeComponent::Update()
{
}

void LifeComponent::OnNotify(GameObject*, Event event, unsigned int playerIndex)
{
	if (m_PlayerIndex != playerIndex)return;

	switch (event)
	{
	case Event::PlayerDied:
		if(m_CurrentLives > 0)
		{
			m_CurrentLives--;
		}
		if(m_CurrentLives == 0)
		{
			//gameObject->MarkDead();
		}
		break;
	}
}

unsigned int LifeComponent::GetCurrentLives() const
{
	return m_CurrentLives;
}

ObserverComponent::ObserverComponent(GameObject* parent)
	:Component{ parent }
	,Observer{}
{
}

ScoreComponent::ScoreComponent( GameObject* parent, unsigned int playerIndex)
	:ObserverComponent{parent}
	,m_CurrentScore{}
	,m_PlayerIndex{playerIndex}
{

}

void ScoreComponent::Update()
{
}

void ScoreComponent::OnNotify(GameObject*, Event event, unsigned int playerIndex)
{
	if (m_PlayerIndex != playerIndex)return;

	int colorChangeValue{ 25 }, finishLevel{ 50 * 28 }, defeatCoily{ 500 }, catchSAndS{ 300 };
	switch (event)
	{
	case Event::ColorChange:
		m_CurrentScore += colorChangeValue;
		break;
	case Event::LevelFinish:
		m_CurrentScore += finishLevel;
		break;
	case Event::DefeatCoily:
		m_CurrentScore += defeatCoily;
		break;
	case Event::CatchSAndS:
		m_CurrentScore += catchSAndS;
		break;
	}
}

unsigned int ScoreComponent::GetCurrentScore() const
{
	return m_CurrentScore;
}
