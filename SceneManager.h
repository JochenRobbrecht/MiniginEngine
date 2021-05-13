#pragma once
#include "Singleton.h"


class Scene;
class SceneManager final : public Singleton<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

	void SetScene(const std::string& name);
	Scene* CreateScene(const std::string& name);
	Scene* GetScene(const std::string& name);

	void Update();
	void Render();
private:
	std::vector<Scene*> m_pScenes;
	Scene* m_pCurrentScene;
};

