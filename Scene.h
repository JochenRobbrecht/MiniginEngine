#pragma once
#include "SceneManager.h"

class GameObject;
class RenderComponent;

class Scene final
{
	friend Scene* SceneManager::CreateScene(const std::string& name);
public:
	void AddGameObject(GameObject* object);
	
	void Update();
	void Render() const; // update render components
	void SortRenderComponents();

	std::string GetName()const;
	std::vector<RenderComponent*>& GetRenderComponentsVec();

	void AddRenderComponent(RenderComponent* renderComp);

	~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

private:
	explicit Scene(const std::string& name);

	std::string m_Name;
	//vec of objects
	std::vector<GameObject*> m_pObjects{};
	//vec of render components
	std::vector<RenderComponent*> m_pRenderComponents{};
	//static unsigned int m_IdCounter;
};


