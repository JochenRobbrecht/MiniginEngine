#pragma once
#include "Transform.h"


class RenderComponent;
class TextComponent;
class Component;
class FpsComponent;
class Font;

class GameObject final
{
public:
	GameObject();
	~GameObject();

	void Update(float elapsedSec);// update all non render comps

	void SetTexture(const std::string& filename);
	void SetPosition(float x, float y);
	RenderComponent* AddRenderComponent(const std::string& filename, std::vector<RenderComponent*>& renderComponents);
	RenderComponent* AddRenderComponent(std::vector<RenderComponent*>& renderComponents);
	TextComponent* AddTextComponent(const std::string& text, Font* font, RenderComponent* renderComponent);
	FpsComponent* AddFpsComponent(TextComponent* textComponent);

	Transform GetTransform()const;
	
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

private:
	Transform m_Transform;
	//vec of non render components
	std::vector<Component*> m_pComponents;
	//render comp (doesnt update but holds it so it can change its contents)
	RenderComponent* m_pRenderComp;//make into vec?
};

