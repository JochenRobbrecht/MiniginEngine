#pragma once

class GameObject;


class Component
{
public:
	Component(GameObject* parent);
	virtual ~Component() = default;
	virtual void Update(float elapsedSec) = 0;

protected:
	GameObject* m_pParent;
};


class Texture2D;


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

	void SetTexture(const std::string& filename);
	void SetTexture(Texture2D* texture2D);

	RenderComponent(const RenderComponent& other) = delete;
	RenderComponent(RenderComponent&& other) = delete;
	RenderComponent& operator=(const RenderComponent& other) = delete;
	RenderComponent& operator=(RenderComponent&& other) = delete;
private:
	Texture2D* m_pTexture;
	GameObject* m_pParent;
};


/// 
/// TextComponent
///
class Font;

class TextComponent final : public Component
{
public:
	explicit TextComponent(const std::string& text, Font* font, GameObject* parent, RenderComponent* renderComponent);
	~TextComponent();

	void Update(float elapsedSec) override;

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
	~FpsComponent();

	void Update(float elapsedSec) override;

	void SetPosition(float x, float y);


	FpsComponent(const FpsComponent& other) = delete;
	FpsComponent(FpsComponent&& other) = delete;
	FpsComponent& operator=(const FpsComponent& other) = delete;
	FpsComponent& operator=(FpsComponent&& other) = delete;
private:
	TextComponent* m_pTextComponent;
	int M_FrameCount;
	float m_AccumulatedTime;
};
