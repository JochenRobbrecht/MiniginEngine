#include "MiniginPCH.h"
#include "Components.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Texture.h"
#include "GameObject.h"
#include "Font.h"
#include "Time.h"
#include "InputManager.h"
#include "Commands.h"
#include "Transform.h"
#include "SoundSystem.h"
#include "ComponentsNoInheritance.h"
#include "Singleton.h"
#include "Scene.h"


Component::Component(GameObject* parent)
	:m_pParent{ parent }
{
}

ObserverComponent::ObserverComponent(GameObject* parent)
	: Component{ parent }
	, Observer{}
{
}

ObservableComponent::ObservableComponent(GameObject* parent)
	: Component{ parent }
	, Observable{}
{
}


TextComponent::TextComponent(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor
	, GameObject* parent, RenderComponent* renderComponent)
	: Component{ parent }
	, m_pFont{ TTF_OpenFont(fontPath.c_str(), ptSize) }
	, m_pRenderComponent(renderComponent)
	, m_pColor{ new Color4f(textColor) }
{
	m_pRenderComponent->AddTexture(text, m_pFont, *m_pColor, 0);
}

TextComponent::~TextComponent()
{
	if (m_pFont)TTF_CloseFont(m_pFont);
	if (m_pColor)delete m_pColor;
}

void TextComponent::Update()
{
}

void TextComponent::SetText(const std::string& text)
{
	m_pRenderComponent->ChangeTexture(text, m_pFont, *m_pColor, 0);
}

void TextComponent::SetPosition(const float x, const float y)
{
	m_pRenderComponent->SetPosition(x, y);
}


InputComponent::InputComponent(GameObject* parent, unsigned int playerIndex, bool isPlayer)
	: Component{parent}
	, m_PlayerIndex{playerIndex}
	, m_IsPlayer{isPlayer}
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

bool InputComponent::IsPlayer() const
{
	return m_IsPlayer;
}

LifeComponent::LifeComponent( GameObject* parent, unsigned int maxLives, TextComponent* textComponent, unsigned int playerIndex)
	: ObserverComponent{ parent }
	, m_PlayerIndex{ playerIndex }
	, m_MaxLives{maxLives}
	, m_CurrentLives{ maxLives }
	, m_pTextComponent{textComponent}
{
	m_pTextComponent->SetText("Lives: " + std::to_string(m_CurrentLives));
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
			m_pTextComponent->SetText("Lives: " + std::to_string(m_CurrentLives));
			std::cout << m_CurrentLives << std::endl;
		}
		if(m_CurrentLives == 0)
		{
			//m_pTextComponent->SetText("Lives: " + std::to_string(m_CurrentLives));
		}
		break;
	}
}

unsigned int LifeComponent::GetCurrentLives() const
{
	return m_CurrentLives;
}


ScoreComponent::ScoreComponent( GameObject* parent, TextComponent* textComponent, unsigned int playerIndex)
	: ObserverComponent{parent}
	, m_PlayerIndex{ playerIndex }
	, m_CurrentScore{}
	, m_pTextComponent{textComponent}
{
	m_pTextComponent->SetText("Score: " + std::to_string(m_CurrentScore));
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
		m_pTextComponent->SetText("Score: " + std::to_string(m_CurrentScore));
		break;
	case Event::LevelFinish:
		m_CurrentScore += finishLevel;
		m_pTextComponent->SetText("Score: " + std::to_string(m_CurrentScore));
		break;
	case Event::DefeatCoily:
		m_CurrentScore += defeatCoily;
		m_pTextComponent->SetText("Score: " + std::to_string(m_CurrentScore));
		break;
	case Event::CatchSAndS:
		m_CurrentScore += catchSAndS;
		m_pTextComponent->SetText("Score: " + std::to_string(m_CurrentScore));
		break;
	}
}

unsigned int ScoreComponent::GetCurrentScore() const
{
	return m_CurrentScore;
}

MoveComponent::MoveComponent(GameObject* parent, unsigned int playerIndex, float speedX, float speedYUp, float speedYDown, float acceleration)
	: ObserverComponent{ parent }
	, m_PlayerIndex{playerIndex}
	, m_Velocity{}
	, m_SpeedX{ speedX }
	, m_SpeedYUp{ speedYUp }
	, m_SpeedYDown{ speedYDown }
	, m_Acceleration{ acceleration }
	, m_CanUpdate{ true }
{
}

void MoveComponent::Update()
{
	if (!m_CanUpdate)return;

	Point2f newPos{ m_pParent->GetTransform().get()->GetPosition() };
	float elapsedSec{ Time::GetInstance().GetElapsedSec() };

	newPos += m_Velocity * elapsedSec;
	m_Velocity.y -= (m_Acceleration * elapsedSec);
	//std::cout << m_Velocity.y << std::endl;

	m_pParent->GetTransform().get()->SetPosition(newPos);
}

void MoveComponent::OnNotify(GameObject* , Event event, unsigned int playerIndex)
{
	if (m_PlayerIndex != playerIndex)return;
	
	if (m_Velocity == Vector2f{ })
	{
		switch (event)
		{
		case Event::MoveUp:
			std::cout << "Move up - right\n";
			m_Velocity.x = m_SpeedX;
			m_Velocity.y = m_SpeedYUp;
			m_CanUpdate = true;
			m_pParent->GetRenderComponent()->SetTexture(TextureId(3));
			break;
		case Event::MoveDown:
			std::cout << "Move down - left\n";
			m_Velocity.x = -m_SpeedX;
			m_Velocity.y = m_SpeedYDown;
			m_CanUpdate = true;
			m_pParent->GetRenderComponent()->SetTexture(TextureId(1));
			break;
		case Event::MoveLeft:
			std::cout << "Move up - left \n";
			m_Velocity.x = -m_SpeedX;
			m_Velocity.y = m_SpeedYUp;
			m_CanUpdate = true;
			m_pParent->GetRenderComponent()->SetTexture(TextureId(2));
			break;
		case Event::MoveRight:
			std::cout << "Move down - right\n";
			m_Velocity.x = m_SpeedX;
			m_Velocity.y = m_SpeedYDown;
			m_CanUpdate = true;
			m_pParent->GetRenderComponent()->SetTexture(TextureId(0));
			break;
		}
	}
	else
	{
		switch (event)
		{
		case Event::Collided:
			m_CanUpdate = false;
			m_Velocity = Vector2f{};
			break;
		case Event::PlayerDied:
			m_Velocity.x = 0;
			m_Velocity.y = m_SpeedYDown;
			break;
		}
	}
}

Vector2f MoveComponent::GetVelocity() const
{
	return m_Velocity;
}


CollisionComponent::CollisionComponent(GameObject* parent, float xOffset, float yOffset)
	: ObservableComponent{ parent }
	, m_pElements{}
	, m_Value{ 5.f }//scale?
	, m_ElementsXOffset{ xOffset }
	, m_ElementsYOffset{ yOffset }
	, m_ReverseTextures{false}
{
}

void CollisionComponent::Update()
{
	//if not moving return?
	
	//calculate pos of where qBert should end up
	Point2f comparePos{ m_pParent->GetTransform().get()->GetPosition() };
	comparePos.x += m_pParent->GetRenderComponent()->GetWidth() / 2;
	comparePos.y += m_pParent->GetRenderComponent()->GetHeight() / 2;
	comparePos.x += m_ElementsXOffset;
	comparePos.y += m_ElementsYOffset;

	for (GameObject* go : m_pElements)
	{
		auto indexCompParent = m_pParent->GetComponent<IndexComponent>()->GetIndex();
		auto indexCompGO = go->GetComponent<IndexComponent>();
		if (indexCompGO->GetIndex() == indexCompParent)continue;

		Point2f elementPos{ go->GetTransform().get()->GetPosition() };
		
		float Distance = sqrtf(powf(comparePos.x - elementPos.x, 2) + powf(comparePos.y - elementPos.y, 2));
		if (Distance < m_Value)
		{
			auto inputCompGo = go->GetComponent<InputComponent>();
			Notify(m_pParent, Event::Collided, inputCompGo->GetPlayerIndex());

			//correct pos + last index collided with
			go->SetPosition(comparePos.x, comparePos.y);
			go->GetComponent<IndexComponent>()->SetIndex(indexCompParent);

			if (inputCompGo->IsPlayer())
			{
				auto rendComp = m_pParent->GetRenderComponent();
				TextureId textureIdBefore = rendComp->GetCurrentTextureId();
				m_pParent->GetRenderComponent()->SetTextureNext(m_ReverseTextures);

				if (textureIdBefore < rendComp->GetCurrentTextureId())
					Notify(m_pParent, Event::ColorChange, inputCompGo->GetPlayerIndex());
			}
		}
	}
}

void CollisionComponent::AddElement(GameObject* go)
{
	m_pElements.push_back(go);
}

void CollisionComponent::SetReverseTextures(bool reverseTextures)
{
	m_ReverseTextures = reverseTextures;
}

LevelComponent::LevelComponent(GameObject* parent)
	: Component{parent}
	//, m_pTiles{}
{
}

void LevelComponent::Update()
{

}

void LevelComponent::LoadLevel(Scene* scene, GameObject* qBert, const Rectf& qBertOffsets, ScoreComponent* scoreComponent)
{
	auto& renderer = Renderer::GetInstance();

	unsigned int rows{ 7 };
	int centerCol{ 3 };

	Point2f startPos{ renderer.GetWindowWidth() / 2.f, renderer.GetWindowHeight() * 3.f / 4.f };
	MoveComponent* qBertMoveComp{ qBert->GetComponent<MoveComponent>() };
	//create tiles
	unsigned int tileIndex{};
	for (unsigned int rowIdx = 0; rowIdx < rows; rowIdx++)
	{
		unsigned int startCol{ centerCol - ((rowIdx + 1) / 2) };
		for (unsigned int colIdx = startCol; colIdx < startCol + rowIdx + 1; colIdx++)
		{
			GameObject* tile{ new GameObject() };
			RenderComponent* rendComp{ tile->AddRenderComponent("../Data/Images/Cube1_1.png", scene, 0, 2.f) };
			rendComp->AddTexture("../Data/Images/Cube1_2.png", 1);
			rendComp->AddTexture("../Data/Images/Cube1_3.png", 2);
			float xPos{ startPos.x - rendComp->GetWidth() / 2.f };
			float yPos{ startPos.y - rendComp->GetHeight() / 2.f };
			xPos += (int(colIdx) - centerCol) * rendComp->GetWidth() + rowIdx % 2 * rendComp->GetWidth() / 2.f;
			yPos -= rendComp->GetHeight() * 3.f / 4.f * rowIdx;
			rendComp->SetPosition(xPos, yPos);

			CollisionComponent* collisionComp{ new CollisionComponent(tile, qBertOffsets.width, qBertOffsets.height) };
			collisionComp->AddElement(qBert);
			collisionComp->AddObserver(qBertMoveComp);
			collisionComp->AddObserver(scoreComponent);
			collisionComp->SetReverseTextures(true);
			tile->AddComponent(collisionComp);

			IndexComponent* indexComp{ new IndexComponent(tile, tileIndex) };
			tile->AddComponent(indexComp);

			scene->AddGameObject(tile);
			//m_pTiles.push_back(tile);
			tileIndex++;
		}
	}
}

IndexComponent::IndexComponent(GameObject* parent, unsigned int index)
	: Component{ parent }
	, m_Index{ index }
{
}

unsigned int IndexComponent::GetIndex() const
{
	return m_Index;
}

void IndexComponent::Update()
{

}

void IndexComponent::SetIndex(unsigned int index)
{
	m_Index = index;
}

KillBoxComponent::KillBoxComponent(GameObject* parent, const Rectf& shape)
	: ObservableComponent{parent}
	, m_Shape{ shape }
	
{
}

void KillBoxComponent::Update()
{
	//if transform of parent is in rect, send event player died
	Point2f parentPos{ m_pParent->GetTransform().get()->GetPosition() };
	if ((parentPos.x > m_Shape.leftBottom.x && parentPos.x < m_Shape.leftBottom.x + m_Shape.width) &&
		(parentPos.y > m_Shape.leftBottom.y && parentPos.y < m_Shape.leftBottom.y + m_Shape.height))
	{
		std::cout << "trigger killbox" << std::endl;
		Notify(m_pParent, Event::PlayerDied, m_pParent->GetComponent<InputComponent>()->GetPlayerIndex());
	}
}

FComponent::FComponent(GameObject* parent, Point2f spawnPos)
	: ObserverComponent{ parent }
	, m_SpawnPos{ spawnPos }
{
}

void FComponent::Update()
{

}

void FComponent::OnNotify(GameObject* gameObject, Event event, unsigned int playerIndex)
{
	if (playerIndex != m_pParent->GetComponent<InputComponent>()->GetPlayerIndex())return;
	
	switch (event)
	{
	case Event::PlayerDied:
		gameObject->SetPosition(m_SpawnPos.x, m_SpawnPos.y);
		break;
	}
}