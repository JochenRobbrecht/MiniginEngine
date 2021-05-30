#pragma once
#include "Transform.h"

class RenderComponent;
class TextComponent;
class Component;
class FpsComponent;
class Font;
class Scene;
typedef struct _TTF_Font TTF_Font;


class GameObject final
{
public:
	GameObject();
	GameObject(std::shared_ptr<Transform> transform);
	~GameObject();

	void Update();// update all non render comps

	void SetPosition(float x, float y);
	RenderComponent* AddRenderComponent(const std::string& filename, Scene* currentScene, unsigned int drawOrder = 0, float scale = 1.f);
	RenderComponent* AddRenderComponent( Scene* currentScene, unsigned int drawOrder = 0, float scale = 1.f);

	
	void AddComponent(Component* component);

	template<typename Type>
	Type* GetComponent()const;
	std::shared_ptr<Transform> GetTransform()const;
	//get components
	RenderComponent* GetRenderComponent() const;
	bool GetMarkedDead()const;

	void MarkDead();

	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

private:
	//vec of non render components
	std::vector<Component*> m_pComponents;
	//render comp (doesnt update but holds ptrs so it can change its contents)
	std::vector<RenderComponent*> m_pRenderComponents;

	bool m_MarkedDead;
	std::shared_ptr<Transform> m_Transform;
};

template<typename Type>
inline Type* GameObject::GetComponent() const
{
	for (Component* comp : m_pComponents)
	{
		if (typeid(*comp) == typeid(Type))
		{
			return static_cast<Type*>(comp);
		}
	}
	return nullptr;
}
