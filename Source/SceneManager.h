#pragma once
#include "Scene.h"
#include "InputMouse.h"
#include <memory>

class SceneManager
{
private:
	SceneManager() {};
	~SceneManager() {};

public:
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	void Update(float elapsedTime, InputMouse* mouse);
	void Render(float elapsedTime);
	void Clear();

	void ChangeScene(Scene* scene);
	void DrawGUI() { currentScene->DrawGUI(); }

private:
	std::unique_ptr<Scene> currentScene = nullptr;
	std::unique_ptr<Scene> nextScene = nullptr;
};