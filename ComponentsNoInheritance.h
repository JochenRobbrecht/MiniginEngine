#pragma once
#include <list>
#include "Singleton.h"
#include "Structs.h"
#include <map>

class GameObject;
class Texture;
typedef struct _TTF_Font TTF_Font;
/// 
/// RenderComponent
///
using TextureId = unsigned int;

class RenderComponent final
{
public:
	RenderComponent(const std::string& filename, GameObject* parent, unsigned int drawOrder = 0, float scale = 1.f);
	RenderComponent(GameObject* parent, unsigned int drawOrder = 0, float scale = 1.f);

	~RenderComponent();

	void Render()const;

	void SetPosition(float x, float y);
	void SetScale(float scale);
	void AddTexture(const std::string& filename, TextureId uniqueId);
	void AddTexture(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor, TextureId uniqueId);
	void AddTexture(const std::string& text, TTF_Font* pFont, const Color4f& textColor, TextureId uniqueId);
	void ChangeTexture(const std::string& filename, TextureId uniqueId);
	void ChangeTexture(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor, TextureId uniqueId);
	void ChangeTexture(const std::string& text, TTF_Font* pFont, const Color4f& textColor, TextureId uniqueId);
	void SetTexture(TextureId uniqueId);
	void SetTextureNext(bool reverse);
	void MarkDead();

	float GetWidth()const;
	float GetHeight()const;
	TextureId GetCurrentTextureId()const;
	bool GetMarkedDead()const;
	unsigned int GetDrawOrder()const;

	RenderComponent(const RenderComponent& other) = delete;
	RenderComponent(RenderComponent&& other) = delete;
	RenderComponent& operator=(const RenderComponent& other) = delete;
	RenderComponent& operator=(RenderComponent&& other) = delete;
private:
	std::map<TextureId, Texture*> m_pTextures;
	std::pair<TextureId, Texture*> m_pCurrentTexture;
	
	GameObject* m_pParent;
	bool m_MarkedDead;
	unsigned int m_DrawOrder;
	float m_Scale;
};

/// 
/// Fps component
///
class FpsComponent final : public Singleton<FpsComponent>
{
	friend class Singleton;
public:
	~FpsComponent() = default;

	void Update();
	
	int GetAverageFps()const;
	const int GetFpsListSize()const;
	const std::list<int>& GetFpsList()const;

	FpsComponent(const FpsComponent& other) = delete;
	FpsComponent(FpsComponent&& other) = delete;
	FpsComponent& operator=(const FpsComponent& other) = delete;
	FpsComponent& operator=(FpsComponent&& other) = delete;
private:
	FpsComponent();

	int m_AverageFps;
	float m_AccumulatedTime;
	float m_FrameUpdateInterval;
	int m_FrameCount;
	std::list<int> m_FpsList;
};
