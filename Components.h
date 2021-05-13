#pragma once
#include "Observe.h"

class GameObject;


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

class Texture2D;

class InputComponent final : public Component
{
public:
	explicit InputComponent(GameObject* parent, unsigned int playerIndex);
	~InputComponent();

	void Update() override;

	unsigned int GetPlayerIndex() const;

	InputComponent(const InputComponent& other) = delete;
	InputComponent(InputComponent&& other) = delete;
	InputComponent& operator=(const InputComponent& other) = delete;
	InputComponent& operator=(InputComponent&& other) = delete;
private:
	unsigned int m_PlayerIndex;
};
/// 
/// RenderComponent
///
class RenderComponent final
{
public:
	RenderComponent(const std::string& filename, GameObject* parent);
	RenderComponent(GameObject* parent);
	~RenderComponent();

	void Render()const;

	void SetPosition(float x, float y);
	void SetTexture(const std::string& filename);
	void SetTexture(Texture2D* texture2D);
	void MarkDead();

	bool GetMarkedDead();

	RenderComponent(const RenderComponent& other) = delete;
	RenderComponent(RenderComponent&& other) = delete;
	RenderComponent& operator=(const RenderComponent& other) = delete;
	RenderComponent& operator=(RenderComponent&& other) = delete;
private:
	Texture2D* m_pTexture;
	GameObject* m_pParent;
	bool m_MarkedDead;
};


/// 
/// TextComponent
///
class Font;

class TextComponent final : public ObserverComponent
{
public:
	explicit TextComponent(const std::string& text, Font* font, GameObject* parent, RenderComponent* renderComponent);
	~TextComponent();

	void Update() override;

	virtual void OnNotify(GameObject* gameObject, Event event, unsigned int playerIndex) override;

	void SetText(const std::string& text);
	void SetPosition(float x, float y);
	
	TextComponent(const TextComponent& other) = delete;
	TextComponent(TextComponent&& other) = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) = delete;
private:
	RenderComponent* m_pRenderComponent;
	bool m_NeedsUpdate;
	std::string m_Text;
	Font* m_pFont;
};

/// 
/// Fps component
///

class FpsComponent final : public Component
{
public:
	explicit FpsComponent(TextComponent* textComponent, GameObject* parent);
	~FpsComponent() = default;

	void Update() override;

	void SetPosition(float x, float y);

	FpsComponent(const FpsComponent& other) = delete;
	FpsComponent(FpsComponent&& other) = delete;
	FpsComponent& operator=(const FpsComponent& other) = delete;
	FpsComponent& operator=(FpsComponent&& other) = delete;
private:
	TextComponent* m_pTextComponent;
	float m_AccumulatedTime;
	float m_FrameUpdateInterval;
	int m_FrameCount;
	int m_PrevAverageFps;
};

class LifeComponent final : public ObserverComponent
{
public:
	explicit LifeComponent( GameObject* parent, unsigned int maxLives, unsigned int playerIndex);
	~LifeComponent() = default;

	void Update() override;

	virtual void OnNotify(GameObject* gameObject, Event event, unsigned int playerIndex) override;

	unsigned int GetCurrentLives()const;

	LifeComponent(const LifeComponent& other) = delete;
	LifeComponent(LifeComponent&& other) = delete;
	LifeComponent& operator=(const LifeComponent& other) = delete;
	LifeComponent& operator=(LifeComponent&& other) = delete;
private:
	unsigned int m_CurrentLives;
	unsigned int m_MaxLives;
	unsigned int m_PlayerIndex;
};

class ScoreComponent final : public ObserverComponent
{
public:
	explicit ScoreComponent(GameObject* parent, unsigned int playerIndex);
	~ScoreComponent() = default;

	void Update() override;

	virtual void OnNotify(GameObject* gameObject, Event event, unsigned int playerIndex) override;

	unsigned int GetCurrentScore()const;

	ScoreComponent(const ScoreComponent& other) = delete;
	ScoreComponent(ScoreComponent&& other) = delete;
	ScoreComponent& operator=(const ScoreComponent& other) = delete;
	ScoreComponent& operator=(ScoreComponent&& other) = delete;
private:
	unsigned int m_CurrentScore;
	unsigned int m_PlayerIndex;
};