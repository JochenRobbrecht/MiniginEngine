#pragma once
#include "Observe.h"
#include "Vector2f.h"
#include "Transform.h"
#include <memory>
#include <map>

class GameObject;
class RenderComponent;
class Texture;
typedef struct _TTF_Font TTF_Font;
struct Color4f;
class Scene;

class Component
{
public:
	Component(GameObject* parent);

	virtual ~Component() = default;
	virtual void Update() = 0;

protected:
	GameObject* m_pParent;
};

class ObserverComponent : public Component, public Observer
{
public:
	ObserverComponent(GameObject* parent);
	virtual ~ObserverComponent() = default;
};

class ObservableComponent : public Component, public Observable
{
public:
	ObservableComponent(GameObject* parent);
	virtual ~ObservableComponent() = default;
};

/// 
/// InputComponent
///

class InputComponent final : public Component
{
public:
	explicit InputComponent(GameObject* parent, unsigned int playerIndex, bool isPlayer);
	~InputComponent();

	void Update() override;

	unsigned int GetPlayerIndex() const;
	bool IsPlayer()const;

	InputComponent(const InputComponent& other) = delete;
	InputComponent(InputComponent&& other) = delete;
	InputComponent& operator=(const InputComponent& other) = delete;
	InputComponent& operator=(InputComponent&& other) = delete;
private:
	unsigned int m_PlayerIndex;
	bool m_IsPlayer;
};

/// 
/// TextComponent
///

class TextComponent final : public Component
{
public:
	explicit TextComponent(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor
		, GameObject* parent, RenderComponent* renderComponent);
	~TextComponent();

	void Update() override;

	void SetText(const std::string& text);
	void SetPosition(float x, float y);
	
	TextComponent(const TextComponent& other) = delete;
	TextComponent(TextComponent&& other) = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) = delete;
private:
	RenderComponent* m_pRenderComponent;
	TTF_Font* m_pFont;
	Color4f* m_pColor;
};


class LifeComponent final : public ObserverComponent
{
public:
	explicit LifeComponent( GameObject* parent, unsigned int maxLives, TextComponent* textComponent, unsigned int playerIndex);
	~LifeComponent() = default;

	void Update() override;

	virtual void OnNotify(GameObject* gameObject, Event event, unsigned int playerIndex) override;

	unsigned int GetCurrentLives()const;

	LifeComponent(const LifeComponent& other) = delete;
	LifeComponent(LifeComponent&& other) = delete;
	LifeComponent& operator=(const LifeComponent& other) = delete;
	LifeComponent& operator=(LifeComponent&& other) = delete;
private:
	TextComponent* m_pTextComponent;
	unsigned int m_PlayerIndex;
	unsigned int m_CurrentLives;
	unsigned int m_MaxLives;
};

class ScoreComponent final : public ObserverComponent
{
public:
	explicit ScoreComponent(GameObject* parent, TextComponent* textComponent, unsigned int playerIndex);
	~ScoreComponent() = default;

	void Update() override;

	virtual void OnNotify(GameObject* gameObject, Event event, unsigned int playerIndex) override;

	unsigned int GetCurrentScore()const;

	ScoreComponent(const ScoreComponent& other) = delete;
	ScoreComponent(ScoreComponent&& other) = delete;
	ScoreComponent& operator=(const ScoreComponent& other) = delete;
	ScoreComponent& operator=(ScoreComponent&& other) = delete;
private:
	TextComponent* m_pTextComponent;
	unsigned int m_PlayerIndex;
	unsigned int m_CurrentScore;
};

/// 
/// MoveComponent
///

class MoveComponent final : public ObserverComponent
{
public:
	explicit MoveComponent(GameObject* parent, unsigned int playerIndex, float speedX, float speedYUp, float speedYDown, float acceleration = 0.f);
	~MoveComponent() = default;

	void Update() override;

	virtual void OnNotify(GameObject * gameObject, Event event, unsigned int playerIndex) override;

	Vector2f GetVelocity()const;

	MoveComponent(const MoveComponent & other) = delete;
	MoveComponent(MoveComponent && other) = delete;
	MoveComponent& operator=(const MoveComponent & other) = delete;
	MoveComponent& operator=(MoveComponent && other) = delete;
private:
	unsigned int m_PlayerIndex;
	Vector2f m_Velocity;
	float m_Acceleration;
	float m_SpeedX;
	float m_SpeedYUp;
	float m_SpeedYDown;
	bool m_CanUpdate;
};

/// 
/// LevelComponent
///

class LevelComponent final : public Component
{
public:
	explicit LevelComponent(GameObject* parent);//vec of weak ptr later
	~LevelComponent() = default;

	void LoadLevel(Scene* scene, GameObject* qBert, const Rectf& qBertOffsets, ScoreComponent* scoreComponent);
	void Update() override;

	LevelComponent(const LevelComponent& other) = delete;
	LevelComponent(LevelComponent&& other) = delete;
	LevelComponent& operator=(const LevelComponent& other) = delete;
	LevelComponent& operator=(LevelComponent&& other) = delete;
private:
	//std::vector<GameObject*> m_pTiles;
};

/// 
/// CollisionComponent
///
 
class CollisionComponent final : public ObservableComponent
{
public:
	explicit CollisionComponent(GameObject* parent, float xOffset, float yOffset);
	~CollisionComponent() = default;

	void Update() override;

	void AddElement(GameObject* gameObject);
	void SetReverseTextures(bool reverseTextures);

	CollisionComponent(const CollisionComponent& other) = delete;
	CollisionComponent(CollisionComponent&& other) = delete;
	CollisionComponent& operator=(const CollisionComponent& other) = delete;
	CollisionComponent& operator=(CollisionComponent&& other) = delete;
private:
	std::vector<GameObject*> m_pElements;
	bool m_ReverseTextures;
	float m_Value;
	float m_ElementsXOffset;
	float m_ElementsYOffset;
};

class IndexComponent final : public Component
{
public:
	IndexComponent(GameObject* parent, unsigned int index);
	unsigned int GetIndex()const;
	void Update()override;
	void SetIndex(unsigned int index);

	IndexComponent(const IndexComponent& other) = delete;
	IndexComponent(IndexComponent&& other) = delete;
	IndexComponent& operator=(const IndexComponent& other) = delete;
	IndexComponent& operator=(IndexComponent&& other) = delete;
private:
	unsigned int m_Index;
};

class KillBoxComponent final : public ObservableComponent
{
public:
	KillBoxComponent(GameObject* parent, const Rectf& shape);

	void Update() override;

	KillBoxComponent(const KillBoxComponent& other) = delete;
	KillBoxComponent(KillBoxComponent&& other) = delete;
	KillBoxComponent& operator=(const KillBoxComponent& other) = delete;
	KillBoxComponent& operator=(KillBoxComponent&& other) = delete;
private:
	Rectf m_Shape;
};

class FComponent final : public ObserverComponent
{
public:
	FComponent(GameObject* parent, Point2f spawnPos);

	void Update() override;

	virtual void OnNotify(GameObject* gameObject, Event event, unsigned int playerIndex) override;

	FComponent(const FComponent& other) = delete;
	FComponent(FComponent&& other) = delete;
	FComponent& operator=(const FComponent& other) = delete;
	FComponent& operator=(FComponent&& other) = delete;
private:
	Point2f m_SpawnPos;
};